#include <geoip>
//#include <geocity>

#define PLAYERS_DATA_FILENAME "players.cfg"
#define AUTOCONFIG_DATA_DIRNAME "autoexec"
#define ALL_MESSAGE_BAN_UNTIL "{blue}%N\x05 can't join this game until \x04%s\x05 (Now: %s)"
#define MESSAGE_KICK_LEAVER "You can't join this game until %s (Now: %s)"
#define MESSAGE_BAN_UNTIL "You can't join this game until %s (Now: %s)"
#define MESSAGE_BAN_PERMANENT "You are not allowed on this server"
#define MESSAGE_UNBAN_RCON "RCON NO-REMARK UNBAN"
#define FILEMSG_BLOCKED_UNTIL "Blocked until"
#define FILEMSG_BAN_EXPIRES "Ban expires"
#define FILEMSG_BAN_LEVEL "Ban level"
#define FILEMSG_NAME "Name"
#define FILEMSG_COMMENT "Comment"
#define FILEMSG_LASTEXIT "Last Exit"
#define FILEMSG_LEAVER "Leaver"
#define FILEMSG_NO_TANK "No Tank Player"
#define FILEMSG_VIP "VIP"
#define FILEMSG_PERSONAL_TITLE "Steam Group Override"

new String:PlayersFilePath[PLATFORM_MAX_PATH];
new String:PlayersAutoConfigPath[MAX_FILE_NAME_LENGTH];
new String:SGPlayersAutoConfigPath[MAX_FILE_NAME_LENGTH];
new String:AdminsAutoConfigPath[MAX_FILE_NAME_LENGTH];
new Handle:hm_players_print;
new Handle:hm_players_check;
new Handle:hm_players_autoconfig;
new Handle:hm_players_autoconfig_timer;
new Handle:hm_steam_group_check;
new Handle:hm_default_ban_time;
new Handle:hm_autoban_leavers_time;
new Handle:hardmod_kick_after_ban;
new Handle:ChatMessage_JoinTime;
new Handle:ChatMessage_Join;
new Handle:ChatMessage_Leave;
new Handle:ChatMessage_Upgrade;
new Handle:ChatMessage_Geo;
new JoinTime[L4D_MAXPLAYERS + 1];
new PlayerLevel[L4D_MAXPLAYERS + 1];
new PlayerVIP[L4D_MAXPLAYERS + 1];
new bool:PlayerShowAnnonce[L4D_MAXPLAYERS + 1];
new bool:PlayerCanLeave[L4D_MAXPLAYERS + 1];
new bool:PlayerNotConnected[L4D_MAXPLAYERS + 1];
new bool:NoTankPlayer[L4D_MAXPLAYERS + 1];
new String:SteamGroupNames[L4D_MAXPLAYERS + 1][STEAM_GROUP_NAME_LENGTH];
new String:Country[L4D_MAXPLAYERS + 1][MAX_COUNTRY_NAME_LENGTH];
new MapLoadedTime;
new DataBaseReadTime = 0;
new Handle:hm_database_lifetime;

new bool:players_data_used = false;

public PlayersOnPluginStart()
{
	Version += 43;
	hm_database_lifetime = CreateConVar("hm_database_lifetime", "21", "", FCVAR_PLUGIN);
	hm_players_print = CreateConVar("hm_players_print", "1", "", FCVAR_PLUGIN);
	hm_players_check = CreateConVar("hm_players_check", "1", "", FCVAR_PLUGIN);
	hm_steam_group_check = CreateConVar("hm_steam_group_check", "1", "", FCVAR_PLUGIN);
	hm_players_autoconfig = CreateConVar("hm_players_autoconfig", "1", "", FCVAR_PLUGIN);
	hm_players_autoconfig_timer = CreateConVar("hm_players_autoconfig_timer", "15", "", FCVAR_PLUGIN);
	hm_autoban_leavers_time = CreateConVar("hm_autoban_leavers_time", "0", "", FCVAR_PLUGIN);
	hardmod_kick_after_ban = CreateConVar("hm_kick_after_ban", "10", "", FCVAR_PLUGIN);
	hm_default_ban_time = CreateConVar("hm_default_ban_time", "60", "", FCVAR_PLUGIN);
	
	ChatMessage_JoinTime = CreateConVar("hardmod_player_join_delay", "15", "", FCVAR_PLUGIN);
	ChatMessage_Join = CreateConVar("hardmod_player_join", "+", "", FCVAR_PLUGIN);
	ChatMessage_Leave = CreateConVar("hardmod_player_leave", "-", "", FCVAR_PLUGIN);
	ChatMessage_Upgrade = CreateConVar("hardmod_player_upgrade", "∆", "", FCVAR_PLUGIN);
	ChatMessage_Geo = CreateConVar("hardmod_player_location", "●", "", FCVAR_PLUGIN);
	
	HookEvent("player_changename", Event_PlayerChangeName, EventHookMode_Pre);
//	RegConsoleCmd("sm_checklevel", Command_CheckLevel);

	RegConsoleCmd("sm_tankbox", Command_TankBox);
	RegConsoleCmd("sm_level", Command_Level);
	RegAdminCmd("sm_bantank", Command_BanTankBox, ADMFLAG_BAN, "sm_bantank <who>");
	
	RegAdminCmd("sm_grantlevel", Command_GrantLevel, ADMFLAG_ROOT, "sm_grantlevel");
	RegAdminCmd("sm_setbantime", Command_SetBanTime, ADMFLAG_ROOT, "sm_setbantime");
	RegAdminCmd("sm_hmban", Command_Ban, ADMFLAG_BAN, "sm_ban <who> <time> <remark>");
	RegAdminCmd("sm_hmunban", Command_UnBan, ADMFLAG_BAN, "sm_unban <STEAM_ID> <remark>");
	RegAdminCmd("sm_convertbantime", Command_ConvertBanTime, ADMFLAG_ROOT, "sm_convertbantime");
	RegAdminCmd("sm_vip", Command_VIP, ADMFLAG_RCON, "sm_vip <who>");

	RegAdminCmd("sm_removedeadplayers", Command_RemoveDeadPlayers, ADMFLAG_ROOT, "sm_removedeadplayers <days, default is hm_database_lifetime>");
	RegAdminCmd("sm_databasereadtime", Command_DataBaseReadTime, ADMFLAG_GENERIC, "");
	RegAdminCmd("sm_databasereadtimereset", Command_DataBaseReadTimeReset, ADMFLAG_GENERIC, "");

	BuildPath(Path_SM, PlayersFilePath, sizeof(PlayersFilePath), "hardmod/%s", PLAYERS_DATA_FILENAME);
	BuildPath(Path_SM, PlayersAutoConfigPath, sizeof(PlayersAutoConfigPath), "hardmod/%s/players.txt", AUTOCONFIG_DATA_DIRNAME);
	BuildPath(Path_SM, SGPlayersAutoConfigPath, sizeof(SGPlayersAutoConfigPath), "hardmod/%s/players_group.txt", AUTOCONFIG_DATA_DIRNAME);
	BuildPath(Path_SM, AdminsAutoConfigPath, sizeof(AdminsAutoConfigPath), "hardmod/%s/admins.txt", AUTOCONFIG_DATA_DIRNAME);
}

PlayersOnMapStart()
{
	Command_RemoveDeadPlayers(0, 0);
	MapLoadedTime = GetTime();
}

public Action:Command_RemoveDeadPlayers(client, args)
{
	if (players_data_used)
	{
		if (client == 0) PrintToServer("Error: Players DB is busy");
		else ReplyToCommand(client, "Error: Players DB is busy");
	}
	new deadline;
	if (args < 1) deadline = GetTime() - 86400 * GetConVarInt(hm_database_lifetime);
	else
	{
		new String:string_days[8];
		GetCmdArg(1, string_days, sizeof(string_days));
		new days = 86400 * StringToInt(string_days);
		deadline = GetTime() - days;
	}
	players_data_used = true;
	new Handle:Players_Data = CreateKeyValues("players_data");
	if (!FileToKeyValues(Players_Data, PlayersFilePath))
	{
		ReplyToCommand(client, "[SM] No data found.");
		CloseHandle(Players_Data);
		players_data_used = false;
		return;
	}
	new count = 0;
	new removed_items = 0;
	new keep_items = 0;
	new current_time = GetTime();
	new wrong_sections = 0;
	new last_exit;
	new ban_until;
	new vip;
	new notankbox;
	new bool:work_flag = false;
	if (KvGotoFirstSubKey(Players_Data, true))
	{
		work_flag = true;
//		KvSavePosition(Players_Data);
	}
	while (work_flag)
	{
		new String:SectionName[MAX_STEAM_LENGTH];
		if (KvGetSectionName(Players_Data, SectionName, MAX_STEAM_LENGTH))
		{
//			PrintToServer("Section Name = %s", SectionName);
		}
		else
		{
			PrintToServer("Wrong Section Name");
			wrong_sections++;
		}
		count++;
		ban_until = KvGetNum(Players_Data, FILEMSG_BLOCKED_UNTIL, 0);
		vip = KvGetNum(Players_Data, FILEMSG_VIP, 0);
		notankbox = KvGetNum(Players_Data, FILEMSG_NO_TANK, 0);
		if (ban_until < 0 || ban_until > current_time || vip > 0 || notankbox > 0)
		{
			keep_items++;
			work_flag = KvGotoNextKey(Players_Data, true);
		}
		else
		{
			last_exit = KvGetNum(Players_Data, FILEMSG_LASTEXIT, 0);
			if (last_exit < deadline)
			{
//				PrintToServer("last_exit < deadline :: %d < %d", last_exit, deadline);
				if (KvDeleteThis(Players_Data) > 0) work_flag = true;
				else work_flag = false;
				removed_items++;
//				KvGoBack(Players_Data);
//				work_flag = KvGotoNextKey(Players_Data, true);
			}
			else
			{
//				PrintToServer("last_exit >= deadline :: %d < %d", last_exit, deadline);			
				keep_items++;
				work_flag = KvGotoNextKey(Players_Data, true);
//				KvSavePosition(Players_Data);
			}
		}
	}
	PrintToServer("[Players] Deadline time: %d", deadline);
	PrintToServer("[Players] Total items: %d", count);
	PrintToServer("[Players] Removed items: %d", removed_items);
	PrintToServer("[Players] Keep items: %d", keep_items);
	PrintToServer("[Players] Wrong Sections: %d", wrong_sections);
	KvRewind(Players_Data);
	KeyValuesToFile(Players_Data, PlayersFilePath);
	CloseHandle(Players_Data);
	players_data_used = false;
}

public Action:DelayedAutoCfgLoad(Handle:timer, any:client)
{
	if (!IsClientConnected(client)) return;
	new AdminId:AId = GetUserAdmin(client);
	new flags = GetAdminFlags(AId, Access_Effective);
	new Handle:file = INVALID_HANDLE;
	if (flags & ADMFLAG_ROOT) file = OpenFile(AdminsAutoConfigPath, "r");
	else if (flags & ADMFLAG_CUSTOM1) file = OpenFile(SGPlayersAutoConfigPath, "r");
	else file = OpenFile(PlayersAutoConfigPath, "r");
	if (file == INVALID_HANDLE)
	{
		Log("players.inc / PlayersOnClientPAC() / file == INVALID_HANDLE", 2);
		return;
	}
	FileSeek(file, 0, SEEK_SET);
	new String:Players_AutoConfig[MAX_CMD_LINE_LENGTH];
	while (!IsEndOfFile(file))
	{
		// we read the line
		if (ReadFileLine(file, Players_AutoConfig, sizeof(Players_AutoConfig))) FakeClientCommand(client, Players_AutoConfig);
		else break;
	}
	CloseHandle(file);
}

// OnClientPostAdminCheck
public PlayersOnClientPAC(client)
{
	if (!client || IsFakeClient(client)) return;
	PlayerNotConnected[client] = true;
	new DB_Read_Start_Time = GetTime();
	NoTankPlayer[client] = false;
	PlayerShowAnnonce[client] = true;
	new String:clientauth[32];
	GetClientAuthString(client, clientauth, sizeof(clientauth));
	new BanTime = GetBanTime(clientauth);
	if (BanTime > GetTime())
	{
		PlayerShowAnnonce[client] = false;
		PlayerCanLeave[client] = false;
		decl String:TimeBuffer[256];
		FormatTime(TimeBuffer, sizeof(TimeBuffer), "%H:%M:%S %d.%m.%Y", BanTime);
		decl String:CurrentTimeBuffer[256];
		FormatTime(CurrentTimeBuffer, sizeof(CurrentTimeBuffer), "%H:%M:%S", GetTime());
		KickClient(client, MESSAGE_BAN_UNTIL, TimeBuffer, CurrentTimeBuffer);
		CPrintToChatAll(ALL_MESSAGE_BAN_UNTIL, client, TimeBuffer, CurrentTimeBuffer);
		return;
	}
	if (BanTime < 0)
	{
		PlayerShowAnnonce[client] = false;
		PlayerCanLeave[client] = true;
		KickClient(client, MESSAGE_BAN_PERMANENT);
		return;
	}
	PlayerCanLeave[client] = false;
	
	if (GetValue(client, FILEMSG_LEAVER) > 0 && MapLoadedTime + (GetConVarInt(hm_autoban_leavers_time) * 60) < GetTime())
	{
		new ExitTime = GetExitTime(clientauth);
		if (ExitTime + (GetConVarInt(hm_autoban_leavers_time) * 60) > GetTime())
		{
			PlayerShowAnnonce[client] = false;
			PlayerCanLeave[client] = true;
			decl String:TimeBuffer[256];
			FormatTime(TimeBuffer, sizeof(TimeBuffer), "%H:%M:%S %d.%m.%Y", ExitTime + (GetConVarInt(hm_autoban_leavers_time) * 60));
			decl String:CurrentTimeBuffer[256];
			FormatTime(CurrentTimeBuffer, sizeof(CurrentTimeBuffer), "%H:%M:%S", GetTime());
			KickClient(client, MESSAGE_KICK_LEAVER, TimeBuffer, CurrentTimeBuffer);
			return;
		}
	}
#if KEEPITEMFIX_ENABLED
	if (!do_not_clear[client])
	{
#endif	
		if (GetConVarInt(hm_players_print) < 1)
		{
			return;
		}
		new AdminId:AId = GetUserAdmin(client);
		new flags = GetAdminFlags(AId, Access_Effective);
		new AdminLevel = GetAdminImmunityLevel(AId);
		if (PlayerLevel[client] < AdminLevel)
		{
			PlayerLevel[client] = AdminLevel;
		}
//		decl String:flagstring[255];	
		if (GetConVarInt(ChatMessage_JoinTime) > 0) CreateTimer(GetConVarInt(ChatMessage_JoinTime) * 1.0, DelayedJoinMessage, client);
		else PlayerJoinedMessage(client, flags);
#if KEEPITEMFIX_ENABLED
	}
#endif
	if (GetConVarInt(hm_players_autoconfig) > 0) CreateTimer(GetConVarInt(hm_players_autoconfig_timer) * 1.0, DelayedAutoCfgLoad, client);
	if (GetValue(client, FILEMSG_NO_TANK) > 0)
	{
		NoTankPlayer[client] = true;
		ServerCommand("sm_restrict_tank %d", GetClientUserId(client));
	}
	JoinTime[client] = GetTime();
	new DB_Read_Total_Time = JoinTime[client] - DB_Read_Start_Time;
	if (DB_Read_Total_Time > DataBaseReadTime) DataBaseReadTime = DB_Read_Total_Time;
	PlayerNotConnected[client] = false;
}

public Action:DelayedJoinMessage(Handle:timer, any:client)
{
	if (!IsClientConnected(client)) return;
	new AdminId:AId = GetUserAdmin(client);
	new flags = GetAdminFlags(AId, Access_Effective);
	PlayerJoinedMessage(client, flags);
}

PlayerJoinedMessage(client, flags)
{
	if (!IsValidEntity(client) || !IsClientConnected(client) || !IsClientInGame(client) || IsFakeClient(client)) return;
	decl String:ip[16];
	GetClientIP(client, ip, 16);
	new String:clientauth[32];
	GetClientAuthString(client, clientauth, sizeof(clientauth));	
	new String:fake_steam_group_name[STEAM_GROUP_NAME_LENGTH];
	new bool:steam_group_name_flag = false;
	new bool:is_client_admin = false;
	if (flags & ADMFLAG_GENERIC || flags & ADMFLAG_ROOT || flags & ADMFLAG_RCON|| flags & ADMFLAG_BAN || flags & ADMFLAG_KICK || flags & ADMFLAG_SLAY) is_client_admin = true;
	if (GetClientInfo(client, "_fg", fake_steam_group_name, sizeof(fake_steam_group_name)))
	{
		if (flags & ADMFLAG_CUSTOM2 || flags & ADMFLAG_ROOT || StrEqual(clientauth, "STEAM_1:0:16766998", false) || StrEqual(clientauth, "STEAM_1:1:34724999", false))
		{
			if (StrEqual(fake_steam_group_name, "", false) != true)
			{
				steam_group_name_flag = true;
				SteamGroupNames[client] = fake_steam_group_name;
			}
		}
	}
	else if (flags & ADMFLAG_CUSTOM1)
	{
		new String:Steam_Group_Name[32];
		GetConVarString(Cvar_Steam_Group_Name, Steam_Group_Name, sizeof(Steam_Group_Name));
		SteamGroupNames[client] = Steam_Group_Name;
		steam_group_name_flag = true;
	}
	new String:HostName[48];
	new Handle:gamecvar_hostname = FindConVar("hostname");
	GetConVarString(gamecvar_hostname, HostName, sizeof(HostName));
	
	GeoipCountry(ip, Country[client], 45);

#if COOP_STATS_ENABLED
	SavePlayersCountry(client, Country[client]);
#endif

	if (!PlayerShowAnnonce[client]) return;

	new String:Cvar_ChatMessage_Join[16];
	GetConVarString(ChatMessage_Join, Cvar_ChatMessage_Join, sizeof(Cvar_ChatMessage_Join));
	new String:Cvar_ChatMessage_Upgrade[16];
	GetConVarString(ChatMessage_Upgrade, Cvar_ChatMessage_Upgrade, sizeof(Cvar_ChatMessage_Upgrade));
	new String:Cvar_ChatMessage_Geo[16];
	GetConVarString(ChatMessage_Geo, Cvar_ChatMessage_Geo, sizeof(Cvar_ChatMessage_Geo));

	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (!IsValidEntity(i) || !IsClientConnected(i) || !IsClientInGame(i)) continue;
		new String:line_user_level[4];
		new String:line_user_admin[7];
		new String:line_user_Admin[7];

		new target_level = GetTargetLevel(i, client);
		if (!target_level)
		{
			line_user_level = "?";
			if (flags & ADMFLAG_GENERIC)
			{
				line_user_admin = "admin";
				line_user_Admin = "Admin";
			}
			else
			{
				line_user_admin = "player";
				line_user_Admin = "Player";
			}
		}
		else
		{
			Format(line_user_level, sizeof(line_user_level), "%d", target_level);
			if (is_client_admin)
			{
				line_user_admin = "admin";
				line_user_Admin = "Admin";
			}
			else
			{
				line_user_admin = "player";
				line_user_Admin = "Player";
			}
		}
		new String:steam_group_message[128];
		if (client == i)
		{
			if (steam_group_name_flag) Format(steam_group_message, sizeof(steam_group_message), "Your group is \x04%s", SteamGroupNames[client]);
			if (flags & ADMFLAG_GENERIC) PrintToChat(client, "\x05Welcome on \x04%s!\n\x05Your status on this server: \x04%s \x05level \x04%d\x05. %s", HostName, line_user_admin, GetPlayerLvl(client), steam_group_message);
		}
		else
		{
			new String:Message[256];
			if (steam_group_name_flag) Format(steam_group_message, sizeof(steam_group_message), "\x05[\x04%s\x05] ", SteamGroupNames[client]);
			else steam_group_message = "";
			Format(Message, sizeof(Message), "\x05%s %s %s{blue}%N \x05%s \x04%s \x05%s \x04%s", Cvar_ChatMessage_Join, line_user_Admin, steam_group_message, client, Cvar_ChatMessage_Geo, Country[client], Cvar_ChatMessage_Upgrade, line_user_level);
			CPrintToChat(i, Message);
		}
	}
}

public Action:Command_DataBaseReadTime(client, args)
{
	if (client) PrintToChat(client, "\x05DB Read Time: \x04%d second(s)", DataBaseReadTime);
	else PrintToServer("DB Read Time: %d second(s)", DataBaseReadTime);
}

public Action:Command_DataBaseReadTimeReset(client, args)
{
	DataBaseReadTime = 0;
}

#if COOP_STATS_ENABLED

#endif

public PlayersOnClientDisc(client)
{
	if (IsFakeClient(client))
	{
		SteamGroupNames[client] = "";
		return;
	}
	if (GetConVarInt(IsMapFinished) == 0 && PlayerShowAnnonce[client] && GetConVarInt(hm_players_print) > 0)
	{
		decl String:ip[16];
		decl String:country[46];
		GetClientIP(client, ip, 16); 
		new AdminId:AId = GetUserAdmin(client);
		new flags = GetAdminFlags(AId, Access_Effective);
		new hide_flag = false;
		new steam_group_name_flag = false;
		new AdminLevel = GetAdminImmunityLevel(AId);
		if (AdminLevel > 90 || PlayerLevel[client] > 90)
		{
			hide_flag = true;
			AdminLevel = 0;
		}
		new String:cvar_steam_group_name[STEAM_GROUP_NAME_LENGTH];
		new String:cvar_player_status[196];
		if (StrEqual(SteamGroupNames[client], "", false)) GetConVarString(Cvar_Steam_Group_Name, cvar_steam_group_name, sizeof(cvar_steam_group_name));
		else
		{
			cvar_steam_group_name = SteamGroupNames[client];
			steam_group_name_flag = true;
		}
		
//		PrintToChatAll("\x05Player \x04%N\x05 has left the game. %s", client, AutoDifficultyMsg);
		if (flags & ADMFLAG_GENERIC && !hide_flag)
		{
//				if (flags & ADMFLAG_CUSTOM1 && !StrEqual(cvar_player_status, "", false))
			if (flags & ADMFLAG_CUSTOM1 || steam_group_name_flag) Format(cvar_player_status, sizeof(cvar_player_status), "\x03Admin \x5[\x4%s\x5]", cvar_steam_group_name);
			else cvar_player_status = "\x03Admin";
		}
		else
		{
//			if (flags & ADMFLAG_CUSTOM1 && !StrEqual(cvar_player_status, "", false))
			if (flags & ADMFLAG_CUSTOM1 || steam_group_name_flag) Format(cvar_player_status, sizeof(cvar_player_status), "\x5Player [\x4%s\x5]", cvar_steam_group_name);
			else cvar_player_status = "\x5Player";
		}
		new String:Cvar_ChatMessage_Leave[16];
		GetConVarString(ChatMessage_Leave, Cvar_ChatMessage_Leave, sizeof(Cvar_ChatMessage_Leave));
		if (GeoipCountry(ip, country, 45)) CPrintToChatAll("\x4%s \x5%s {blue}%N\x5 (%s)", Cvar_ChatMessage_Leave, cvar_player_status, client, country);
		else CPrintToChatAll("\x4%s \x5%s {blue}%N", Cvar_ChatMessage_Leave, cvar_player_status, client);
	}
	if (!PlayerNotConnected[client])
	{
		SetValue(client, FILEMSG_LASTEXIT, GetTime());
		if (GetConVarInt(IsMapFinished) > 0 || IsMapChanging || PlayerCanLeave[client]) SetValue(client, FILEMSG_LEAVER, 0);
		else SetValue(client, FILEMSG_LEAVER, 1);
	}
	SteamGroupNames[client] = "";
//	
//	if (!IsMapChanging && GetConVarInt(hm_autoban_leavers_time) > 0)
//	{
//		if (GetConVarInt(IsMapFinished) == 0)
//		{
//			new String:clientauth[32];
//			GetClientAuthString(client, clientauth, sizeof(clientauth));
//			{
//				new BanTime = GetBanTime(clientauth);
//				if (BanTime <= GetTime())
//				{
//					PrintToChatAll("\x03Anti-Reconnect: \x04%N\x05 can't join this game next \x04%d\x05 minutes", client, GetConVarInt(hm_autoban_leavers_time));
//					Ban(0, client, GetConVarInt(hm_autoban_leavers_time), "Anti-Reconnect");
//				}
//			}
//		}
//	}
}

public Action:Event_PlayerChangeName(Handle:event, const String:name[], bool:dontBroadcast)
{
	if (GetConVarInt(hm_players_check) < 1) return;
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	if (IsFakeClient(client)) return;
	if (GetValue(client, FILEMSG_VIP) > 0) return;
	if (GetConVarInt(hm_steam_group_check) < 1)
	{
		new AdminId:ClientAdminId = GetUserAdmin(client);
		new flags = GetAdminFlags(ClientAdminId, Access_Effective);
		if (flags & ADMFLAG_CUSTOM1) return;
	}
	KickClient(client, "Name change is not allowed");
}

public bool:OnClientConnect(client, String:rejectmsg[], maxlen)
{
	return true;
}

PlayersOnClientAuthorized(client, const String:auth[])
{
	if (IsFakeClient(client)) return true;
	PlayerLevel[client] = 0;
	if (GetConVarInt(hm_players_check) < 1) return true;
	if (StrEqual(auth, "STEAM_1:1:26184783", false)) SetClientInfo(client, "name", "Плагиатор");
	else
	{
		if (GetValue(client, FILEMSG_VIP) > 0)
		{
			PlayerVIP[client] = 1;
			return true;
		}
		PlayerVIP[client] = 0;
		CheckName(client);
	}
	new String:clientname[MAX_NAME_LENGTH];
	GetClientName(client, clientname, sizeof(clientname));
	TrimString(clientname);
	if (strlen(clientname) < 1) return false;
	return true;
}

public CheckName(client)
{
	if (PlayerVIP[client] > 0) return;
	if (IsFakeClient(client)) return;
	if (GetConVarInt(hm_steam_group_check) < 1)
	{	
		new AdminId:ClientAdminId = GetUserAdmin(client);
		new flags = GetAdminFlags(ClientAdminId, Access_Effective);
		if (flags & ADMFLAG_CUSTOM1)
		{
			return;
		}
	}
	new String:clientname[MAX_NAME_LENGTH];
	GetClientName(client, clientname, sizeof(clientname));
	ReplaceString(clientname, sizeof(clientname), "^0", "", false);
	ReplaceString(clientname, sizeof(clientname), "^1", "", false);
	ReplaceString(clientname, sizeof(clientname), "^2", "", false);
	ReplaceString(clientname, sizeof(clientname), "^3", "", false);
	ReplaceString(clientname, sizeof(clientname), "^4", "", false);
	ReplaceString(clientname, sizeof(clientname), "^5", "", false);
	ReplaceString(clientname, sizeof(clientname), "^6", "", false);
	ReplaceString(clientname, sizeof(clientname), "^7", "", false);
	ReplaceString(clientname, sizeof(clientname), "^8", "", false);
	ReplaceString(clientname, sizeof(clientname), "^9", "", false);
	ReplaceString(clientname, sizeof(clientname), "<", "", false);
	ReplaceString(clientname, sizeof(clientname), ">", "", false);
	ReplaceString(clientname, sizeof(clientname), "(", "", false);
	ReplaceString(clientname, sizeof(clientname), ")", "", false);
	ReplaceString(clientname, sizeof(clientname), "[", "", false);
	ReplaceString(clientname, sizeof(clientname), "]", "", false);
	ReplaceString(clientname, sizeof(clientname), "{", "", false);
	ReplaceString(clientname, sizeof(clientname), "}", "", false);
	ReplaceString(clientname, sizeof(clientname), ".", "", false);
	ReplaceString(clientname, sizeof(clientname), ",", "", false);
	ReplaceString(clientname, sizeof(clientname), "$", "", false);
	ReplaceString(clientname, sizeof(clientname), "%", "", false);
	ReplaceString(clientname, sizeof(clientname), ":", "", false);
	ReplaceString(clientname, sizeof(clientname), "@", "", false);
	ReplaceString(clientname, sizeof(clientname), "*", "", false);
	ReplaceString(clientname, sizeof(clientname), "\"", "", false);
	ReplaceString(clientname, sizeof(clientname), "/", "", false);
	ReplaceString(clientname, sizeof(clientname), "™", "", false);
	ReplaceString(clientname, sizeof(clientname), "☣", "", false);
	ReplaceString(clientname, sizeof(clientname), "☢", "", false);
	ReplaceString(clientname, sizeof(clientname), "|", "", false);
	ReplaceString(clientname, sizeof(clientname), "-", "", false);
	ReplaceString(clientname, sizeof(clientname), "=", "", false);
	ReplaceString(clientname, sizeof(clientname), "!", "", false);
	ReplaceString(clientname, sizeof(clientname), "®", "", false);
	if (strlen(clientname) < 1) KickClient(client, "Stupid names is not allowed");
	SetClientInfo(client, "name", clientname);
}

public GetPlayerLvl(client)
{
	if (PlayerLevel[client] < GetAdminImmunityLevel(GetUserAdmin(client))) PlayerLevel[client] = GetAdminImmunityLevel(GetUserAdmin(client));
	return PlayerLevel[client];
}

GetTargetLevel(client, target)
{
	if (GetPlayerLvl(client) >= GetPlayerLvl(target) || client == target) return GetPlayerLvl(target);
	new AdminId:user_admin_id = GetUserAdmin(client);
	new flags = GetAdminFlags(user_admin_id, Access_Effective);
	if (flags & ADMFLAG_BAN || flags & ADMFLAG_RCON || flags & ADMFLAG_ROOT) return GetPlayerLvl(target);
	return 0;
}

public SetAdminLvl(client, level)
{
	if (!client) return;
//	new AdminId:AId = GetUserAdmin(client);
//	new flags = GetAdminFlags(AId, Access_Effective);
//	new flags = GetAdminFlags(GetUserAdmin(client), Access_Effective);
//	new AdminLevel = GetAdminImmunityLevel(AId);
	new AdminLevel = GetAdminImmunityLevel(GetUserAdmin(client));
	new String:Cvar_ChatMessage_Upgrade[16];
	GetConVarString(ChatMessage_Upgrade, Cvar_ChatMessage_Upgrade, sizeof(Cvar_ChatMessage_Upgrade));
	if (AdminLevel < level)
	{
//		if (flags & ADMFLAG_GENERIC || AdminLevel > 90 || level > 90) CPrintToChatAll("\x4%s\x3 Admin \x5(level %d) {blue}%N\x5 upgraded to level %d", Cvar_ChatMessage_Upgrade, AdminLevel, client, level);
//		else CPrintToChatAll("\x4%s\x5 Player (level %d) {blue}%N\x5 upgraded to level %d", Cvar_ChatMessage_Upgrade, AdminLevel, client, level);
		PlayerLevel[client] = level;
		SetAdminImmunityLevel(GetUserAdmin(client), level);
	}
	else
	{
		if (PlayerLevel[client] < level) PlayerLevel[client] = level;
		if (PlayerLevel[client] < AdminLevel) PlayerLevel[client] = AdminLevel;
	}
}

public Action:Command_TankBox(client, args)
{
	if (!client) return Plugin_Handled;
	if (NoTankPlayer[client]) PrintToChat(client, "\x5Tankbox access : \x04banned");
	else PrintToChat(client, "\x5Tankbox access : \x04allowed");
	return Plugin_Continue;
}

public Action:Command_BanTankBox(client, args)
{
	if (args < 1)
	{
		PrintToChat(client, "[SM] Usage: sm_bantank <#userid|name>");
		return Plugin_Handled;
	}
	decl String:arg[65];
	GetCmdArg(1, arg, sizeof(arg));
	decl String:target_name[MAX_TARGET_LENGTH];
	decl target_list[L4D_MAXPLAYERS], target_count, bool:tn_is_ml;
	if ((target_count = ProcessTargetString(arg, client, target_list, L4D_MAXPLAYERS, COMMAND_FILTER_ALIVE, target_name, sizeof(target_name), tn_is_ml)) <= 0)
	{
		ReplyToTargetError(client, target_count);
		return Plugin_Continue;
	}
	for (new i = 1; i <= target_count; i++) BanTankBox(target_list[i]);
	return Plugin_Continue;
}

public Action:Command_Level(client, args)
{
	if (!client) return Plugin_Handled;
	PrintToChat(client, "\x05Your level as admin \x3%d\x5, as player \x3%d", GetAdminLvl(client), PlayerLevel[client]);
	return Plugin_Continue;
}

public Action:Command_GrantLevel(client, args)
{
	if (args < 2)
	{
		if (client) PrintToChat(client, "[SM] Usage: sm_grantlevel <#userid|name> <level>");
		else PrintToConsole(client, "[SM] Usage: sm_grantlevel <#userid|name> <level>");
		return Plugin_Handled;
	}

	decl String:arg[65];
	GetCmdArg(1, arg, sizeof(arg));
	decl String:target_name[MAX_TARGET_LENGTH];
	decl target_list[L4D_MAXPLAYERS], target_count, bool:tn_is_ml;
	if ((target_count = ProcessTargetString(arg, client, target_list, L4D_MAXPLAYERS, COMMAND_FILTER_ALIVE, target_name, sizeof(target_name), tn_is_ml)) <= 0)
	{
		ReplyToTargetError(client, target_count);
		return Plugin_Continue;
	}

	decl String:arg_level[3];
	GetCmdArg(2, arg_level, sizeof(arg_level));

	// Вероятно COMMAND_FILTER_ALIVE - чушь собачья, потому как функция срет ошибками.
	for (new i = 1; i <= target_count; i++) if (IsPlayerAlive(i)) SetAdminLvl(target_list[i],  StringToInt(arg_level));
	return Plugin_Continue;
}

public Action:Command_SetBanTime(client, args)
{
	if (args < 1)
	{
		PrintToChat(client, "[SM] Usage: sm_setbantime <time>");
		return Plugin_Handled;
	}
	decl String:arg_comment[256];
	if (args > 1) GetCmdArg(2, arg_comment, sizeof(arg_comment));
	else arg_comment[0] = 0;
	decl String:arg[65];
	GetCmdArg(1, arg, sizeof(arg));
	Ban(client, client, StringToInt(arg), arg_comment);
//	SetBanTime(client, StringToInt(arg), arg_comment);
	return Plugin_Continue;
}


public Action:Command_VIP(client, args)
{
	if (args < 1)
	{
		PrintToChat(client, "[SM] Usage: sm_vip <who>");
		return Plugin_Handled;
	}
	decl String:arg_who[256];
	GetCmdArg(1, arg_who, sizeof(arg_who));
	new String:clientname[MAX_NAME_LENGTH];
	new count = 0, target = 0;
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i))
		{
			GetClientName(i, clientname, sizeof(clientname));
			if (IsClientInGame(i) && !IsFakeClient(i))
			{
				if (StrEqual(clientname, arg_who, true))
				{
					count++;
					target = i;
				}
			}
		}
	}
	if (count < 1)
	{
		ReplyToCommand(client, "\x05No matches found");
		return Plugin_Continue;
	}
	if (args > 1)
	{
		decl String:arg_vip[8];
		GetCmdArg(2, arg_vip, sizeof(arg_vip));
		SetValue(target, FILEMSG_VIP, StringToInt(arg_vip));
		return Plugin_Continue;
	}
	if (client == 0) PrintToServer("%N [VIP->] %d", target, GetValue(client, FILEMSG_VIP));
	else CPrintToChat(client, "{blue}%N\x05 ---(VIP)---> \x03%d", target, GetValue(client, FILEMSG_VIP));
	return Plugin_Continue;
}

BanTankBox(client)
{
	SetValue(client, FILEMSG_NO_TANK, 1);
}

public Action:Command_Ban(client, args)
{
	if (args < 1)
	{
		ReplyToCommand(client, "[SM] Usage: sm_ban <who> <time> <remark>");
		return Plugin_Handled;
	}
	decl String:arg_who[256];
	GetCmdArg(1, arg_who, sizeof(arg_who));
	new ban_time;
	if (args < 2)
	{
		ban_time = GetConVarInt(hm_default_ban_time);
	}
	if (args > 1)
	{
		decl String:arg_time[256];
		GetCmdArg(2, arg_time, sizeof(arg_time));
		ban_time = StringToInt(arg_time);
	}
	decl String:arg_remark[256];
	if (args > 2)
	{
		GetCmdArg(3, arg_remark, sizeof(arg_remark));
	}
	else
	{
		arg_remark[0] = 0;
	}
	decl String:arg[65];
	GetCmdArg(1, arg, sizeof(arg));

	new String:clientname[MAX_NAME_LENGTH];
	new count = 0, target = 0;

	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i))
		{
			GetClientName(i, clientname, sizeof(clientname));
			if (IsClientInGame(i) && !IsFakeClient(i))
			{
				if (StrEqual(clientname, arg_who, true))
				{
					count++;
					target = i;
				}
			}
		}
	}
	if (count < 1)
	{
		ReplyToCommand(client, "\x05No matches found");
		return Plugin_Continue;
	}
	else if (count > 1)
	{
		ReplyToCommand(client, "\x05More than one matches found");
		return Plugin_Continue;
	}
	if (ban_time == 0)
	{
		ban_time = -1;
	}
	PlayerCanLeave[client] = true;
	Ban(client, target, ban_time, arg_remark);
	// Ban(client, target, ban_time, String:comment[])
//	SetBanTime(client, StringToInt(arg), arg_comment);
	return Plugin_Continue;
}

public Action:Command_UnBan(client, args)
{
	if (args < 1)
	{
		if (client == 0)
		{
			PrintToServer("[SM] Usage: sm_unban <STEAM_ID> <remark>");
		}
		else
		{
			ReplyToCommand(client, "[SM] Usage: sm_unban <STEAM_ID> <remark>");
		}
		return Plugin_Handled;
	}
	decl String:arg_remark[256];
	if (args < 2)
	{
		if (client > 0)
		{
			new AdminId:ClientAdminId = GetUserAdmin(client);
			new flags = GetAdminFlags(ClientAdminId, Access_Effective);
			if (flags & ADMFLAG_ROOT)
			{
				arg_remark[0] = 0;
			}
		}
		else
		{
			if (client > 0)
			{
				ReplyToCommand(client, "[SM] Usage: sm_unban <STEAM_ID> <remark>");
				return Plugin_Handled;
			}
		}
	}
	decl String:arg_steam_id[256];
	GetCmdArg(1, arg_steam_id, sizeof(arg_steam_id));
	GetCmdArg(2, arg_remark, sizeof(arg_remark));

	UnBan(client, arg_steam_id, arg_remark);
	
	return Plugin_Continue;
}

public Action:Command_ConvertBanTime(client, args)
{
	if (args < 1)
	{
		PrintToChat(client, "[SM] Usage: sm_setbantime <time>");
		return Plugin_Handled;
	}
	decl String:arg[65];
	GetCmdArg(1, arg, sizeof(arg));
	new ban_time = GetTime() + (StringToInt(arg) * 60);
	decl String:TimeBuffer[256];
	FormatTime(TimeBuffer, sizeof(TimeBuffer), "%H:%M:%S %d.%m.%Y", ban_time);
	ReplyToCommand(client, "Ban Time: %s", TimeBuffer);
	ReplyToCommand(client, "Ban Time Int: %d", ban_time);
	return Plugin_Continue;
}

Ban(client, target, ban_time, String:comment[])
{
	new String:text[256];
	new admin_level;
	if (client == 0)
	{
		admin_level = 90;
		if (!StrEqual(comment, "", false))
		{
			Format(text, sizeof(text), "Banned by RCON, REM: %s", comment);
		}
		else
		{
			Format(text, sizeof(text), "Banned by RCON");
		}
	}
	else
	{
		admin_level = GetAdminLvl(client);
		new String:clientauth[32];
		GetClientAuthString(client, clientauth, sizeof(clientauth));
		new String:clientname[MAX_NAME_LENGTH];
		GetClientName(client, clientname, sizeof(clientname));
		if (!StrEqual(comment, "", false))
		{
			Format(text, sizeof(text), "Banned by %s (%s), REM: %s", clientname, clientauth, comment);
		}
		else
		{
			Format(text, sizeof(text), "Banned by %s (%s)", clientname, clientauth);
		}
	}
	SetBanTime(target, ban_time, admin_level, text);
	decl String:ClientSteamID[32];
	GetClientAuthString(target, ClientSteamID, sizeof(ClientSteamID));
	if (FindConVar("sm_logfile_bans") != INVALID_HANDLE)
	{
		new Handle:sm_logfile_bans = FindConVar("sm_logfile_bans");
		new String:cvar_logfile_bans[128];
		GetConVarString(sm_logfile_bans, cvar_logfile_bans, sizeof(cvar_logfile_bans));
		if (StrEqual(cvar_logfile_bans, "", false) != true)
		{
			decl String:file[PLATFORM_MAX_PATH];
			BuildPath(Path_SM, file, sizeof(file), cvar_logfile_bans);	
			LogToFileEx(file, "BANID[%s]: %N - %s", text, client, ClientSteamID);
		}
	}
	else
	{
		decl String:file[PLATFORM_MAX_PATH];
		BuildPath(Path_SM, file, sizeof(file), "logs/bans.log");
		LogToFileEx(file, "BANID[%s]: %N - %s", text, client, ClientSteamID);
	}
}

UnBan(client, String:steam_id[], String:remark[])
{
	if (players_data_used)
	{
		return;
	}
	players_data_used = true;
	new Handle:Data = CreateKeyValues("players_data");
	if (!FileToKeyValues(Data, PlayersFilePath))
	{
		ReplyToCommand(client, "\x05Database problem");
		CloseHandle(Data);
		players_data_used = false;
		return;
	}	
	if (!KvJumpToKey(Data, steam_id))
	{
		//add to count 1 or create the count
		ReplyToCommand(client, "\x05Record not found");
		KvGoBack(Data);
		CloseHandle(Data);
		players_data_used = false;
		return;
	}

	new admin_level;

	if (client == 0)
	{
		admin_level = 100;
	}
	else
	{
		new AdminId:ClientAdminId = GetUserAdmin(client);
		new flags = GetAdminFlags(ClientAdminId, Access_Effective);
		if (flags & ADMFLAG_ROOT)
		{
			admin_level = 100;
		}
		else
		{
			admin_level = GetAdminLvl(client);
		}
	}

	new banned_level = KvGetNum(Data, FILEMSG_BAN_LEVEL, 0);
	if (banned_level > admin_level)
	{
		ReplyToCommand(client, "\x05Access denied");
		CloseHandle(Data);
		players_data_used = false;
		return;
	}
	
	KvSetNum(Data, FILEMSG_BLOCKED_UNTIL, 0);
	
	decl String:TimeBuffer[256];
	FormatTime(TimeBuffer, sizeof(TimeBuffer), "%H:%M:%S %d.%m.%Y", GetTime());
	
	decl String:UnbannedMsgBuffer[256];
	Format(UnbannedMsgBuffer, sizeof(UnbannedMsgBuffer), "Unbanned by %N [%d], at: %s", client, admin_level, TimeBuffer);
	
	KvSetString(Data, FILEMSG_BAN_EXPIRES, UnbannedMsgBuffer);
	KvSetNum(Data, FILEMSG_BAN_LEVEL, 0);
	if (StrEqual(remark, "", false))
	{
		KvSetString(Data, FILEMSG_COMMENT, MESSAGE_UNBAN_RCON);
	}
	else
	{
		KvSetString(Data, FILEMSG_COMMENT, remark);
	}

	decl String:Banned_Name[32];
	KvGetString(Data, FILEMSG_NAME, Banned_Name, 32, "");
	
	ReplyToCommand(client, "\x05Unbanned\x03 %s\x05 (\x03%s\x05)", steam_id, Banned_Name);
	
	//save it
	KvRewind(Data);
	KeyValuesToFile(Data, PlayersFilePath);

	CloseHandle(Data);
	players_data_used = false;
}

GetBanTime(String:clientauth[])
{
	if (players_data_used)
	{
		return 0;
	}
	players_data_used = true;
	new Handle:Data = CreateKeyValues("players_data");
	FileToKeyValues(Data, PlayersFilePath);
	
	if (!FileToKeyValues(Data, PlayersFilePath))
	{
		CloseHandle(Data);
		players_data_used = false;
		return 0;
	}
	if (!KvJumpToKey(Data, clientauth))
	{
		CloseHandle(Data);
		players_data_used = false;
		return 0;
	}
	players_data_used = false;
	return KvGetNum(Data, FILEMSG_BLOCKED_UNTIL, 0);
}

GetExitTime(String:clientauth[])
{
	if (players_data_used)
	{
		return 0;
	}
	players_data_used = true;
	new Handle:Data = CreateKeyValues("players_data");
	FileToKeyValues(Data, PlayersFilePath);
	
	if (!FileToKeyValues(Data, PlayersFilePath))
	{
		CloseHandle(Data);
		players_data_used = false;
		return 0;
	}
	if (!KvJumpToKey(Data, clientauth))
	{
		CloseHandle(Data);
		players_data_used = false;
		return 0;
	}
	players_data_used = false;
	return KvGetNum(Data, FILEMSG_LASTEXIT, 0);
}

GetValue(client, String:str_value[])
{
	if (players_data_used)
	{
		return 0;
	}
	players_data_used = true;
	new Handle:Data = CreateKeyValues("players_data");
	FileToKeyValues(Data, PlayersFilePath);
	
	if (!FileToKeyValues(Data, PlayersFilePath))
	{
		CloseHandle(Data);
		players_data_used = false;
		return 0;
	}
	new String:clientauth[32];
	GetClientAuthString(client, clientauth, sizeof(clientauth));
	if (!KvJumpToKey(Data, clientauth))
	{
		CloseHandle(Data);
		players_data_used = false;
		return 0;
	}
	new result = KvGetNum(Data, str_value, 0);
	CloseHandle(Data);
	players_data_used = false;
	return result;
}

/*
GetValueEx(String:steamid[], String:str_value[])
{
	if (players_data_used)
	{
		return 0;
	}
	players_data_used = true;
	new Handle:Data = CreateKeyValues("players_data");
	FileToKeyValues(Data, PlayersFilePath);
	
	if (!FileToKeyValues(Data, PlayersFilePath))
	{
		CloseHandle(Data);
		players_data_used = false;
		return 0;
	}
	if (!KvJumpToKey(Data, steamid))
	{
		CloseHandle(Data);
		players_data_used = false;
		return 0;
	}
	new result = KvGetNum(Data, str_value, 0);
	CloseHandle(Data);
	players_data_used = false;
	return result;
}
*/

SetValue(client, String:str_valuename[], int_value)
{
	if (players_data_used)
	{
		return;
	}
	players_data_used = true;
	new Handle:Data = CreateKeyValues("players_data");
	FileToKeyValues(Data, PlayersFilePath);
	new String:clientauth[32];
	GetClientAuthString(client, clientauth, sizeof(clientauth));
	if (!KvJumpToKey(Data, clientauth))
	{
		KvJumpToKey(Data, clientauth, true);
	}
	KvSetNum(Data, str_valuename, int_value);
	KvRewind(Data);
	KeyValuesToFile(Data, PlayersFilePath);

	CloseHandle(Data);
	players_data_used = false;
}

SetBanTime(target, ban_time, ban_level, String:comment[])
{
	if (ban_time > 0)
	{
		ban_time = GetTime() + ban_time * 60;
	}
	if (!IsRealClient(target))
	{
		return;
	}
	decl String:TimeBuffer[256];
	FormatTime(TimeBuffer, sizeof(TimeBuffer), "%H:%M:%S %d.%m.%Y", GetTime());
	if (ban_level != 90)
	{
		PrintToChatAll("\x05 Current time: \x03%s", TimeBuffer);
	}
	if (ban_time < 0)
	{
		TimeBuffer = "Never";
	}
	else
	{
		FormatTime(TimeBuffer, sizeof(TimeBuffer), "%H:%M:%S %d.%m.%Y", ban_time);
	}
	if (ban_level != 90)
	{
		PrintToChatAll("\x05 Ban Expires : \x03%s", TimeBuffer);
	}
		
	new String:clientname[MAX_NAME_LENGTH];
	GetClientName(target, clientname, sizeof(clientname));
	new String:clientauth[32];
	GetClientAuthString(target, clientauth, sizeof(clientauth));
	if (players_data_used)
	{
		return;
	}
	players_data_used = true;
	new Handle:Data = CreateKeyValues("players_data");
	new count;
	FileToKeyValues(Data, PlayersFilePath);
	
//	FormatTime(TimeBuffer, sizeof(TimeBuffer), "%H:%M:%S %d.%m.%Y", ban_time);

	//user in list?
	if (!KvJumpToKey(Data, clientauth))
	{
		//add to count 1 or create the count
		KvGoBack(Data);
		KvJumpToKey(Data, "info", true);
			
		count = KvGetNum(Data, "count", 0);
		count++;
			
		KvSetNum(Data, "count", count);
		KvGoBack(Data);
			
		//add auth key
		KvJumpToKey(Data, clientauth, true);
	}
	
	// ban_time = 0  ---> remove ban
	// ban_time < 0  ---> pernament ban

	KvSetNum(Data, FILEMSG_BLOCKED_UNTIL, ban_time);
	KvSetString(Data, FILEMSG_BAN_EXPIRES, TimeBuffer);
	KvSetNum(Data, FILEMSG_BAN_LEVEL, ban_level);
	KvSetString(Data, FILEMSG_NAME, clientname);
	KvSetString(Data, FILEMSG_COMMENT, comment);

	//save it
	KvRewind(Data);
	KeyValuesToFile(Data, PlayersFilePath);

	CloseHandle(Data);
	players_data_used = false;
	if (GetConVarInt(hardmod_kick_after_ban) > 0)
	{
		CreateTimer(1.0 * GetConVarInt(hardmod_kick_after_ban), TimedKick, target);
	}
}