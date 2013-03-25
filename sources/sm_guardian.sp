#include <sourcemod>

#define PLUGIN_NAME "Guardian"
#define PLUGIN_VERSION "1.4"

new Handle:sm_logfile_players;
new Handle:sm_logfile_chat;
new Handle:sm_logfile_commands;
new Handle:sm_logfile_filter;
new Handle:sm_logfile_bans;
new Handle:sm_block_attack;

public Plugin:myinfo =
{
	name = PLUGIN_NAME,
	author = "Jonny",
	description = "",
	version = PLUGIN_VERSION,
	url = "http://www.sourcemod.net/"
};

public OnPluginStart()
{
	CreateConVar("sm_antigay_version", PLUGIN_VERSION, "AntiGay Plugin Version", FCVAR_PLUGIN|FCVAR_SPONLY|FCVAR_UNLOGGED|FCVAR_DONTRECORD|FCVAR_REPLICATED|FCVAR_NOTIFY);
	sm_logfile_players = CreateConVar("sm_logfile_players", "", "LOG STEAM_ID + IP + NICKNAME to file", FCVAR_PLUGIN|FCVAR_SPONLY);
	sm_logfile_chat = CreateConVar("sm_logfile_chat", "", "LOG chat to file", FCVAR_PLUGIN|FCVAR_SPONLY);
	sm_logfile_commands = CreateConVar("sm_logfile_commands", "", "LOG Player commands to file", FCVAR_PLUGIN|FCVAR_SPONLY);
	sm_logfile_filter = CreateConVar("sm_logfile_filter", "0", "Filter", FCVAR_PLUGIN|FCVAR_SPONLY);
	sm_logfile_bans = CreateConVar("sm_logfile_bans", "", "LOG Player bans to file", FCVAR_PLUGIN|FCVAR_SPONLY);
	sm_block_attack = CreateConVar("sm_block_attack", "", "Block attack", FCVAR_PLUGIN|FCVAR_SPONLY);
	RegConsoleCmd("say", Command_Say);
}

public OnClientPutInServer(client)
{
	new String:cvar_logfile_players[128];
	GetConVarString(sm_logfile_players, cvar_logfile_players, sizeof(cvar_logfile_players));
	if (StrEqual(cvar_logfile_players, "", false) != true)
	{
		if (!IsFakeClient(client))
		{
			decl String:file[PLATFORM_MAX_PATH], String:steamid[24], String:ClientIP[24];
			BuildPath(Path_SM, file, sizeof(file), cvar_logfile_players);
			GetClientAuthString(client, steamid, sizeof(steamid));
			GetClientIP(client, ClientIP, sizeof(ClientIP), false);      

			LogToFileEx(file, "%N - %s - %s", client, steamid, ClientIP);
		}
	}
}

public KickClientID(client)
{
	decl String:ClientSteamID[32];
	GetClientAuthString(client, ClientSteamID, sizeof(ClientSteamID));
	ServerCommand("kickid %d", GetClientUserId(client));
	new String:cvar_logfile_bans[128];
	GetConVarString(sm_logfile_bans, cvar_logfile_bans, sizeof(cvar_logfile_bans));
	if (StrEqual(cvar_logfile_bans, "", false) != true)
	{
		decl String:file[PLATFORM_MAX_PATH];
		BuildPath(Path_SM, file, sizeof(file), cvar_logfile_bans);	
		LogToFileEx(file, "KICKID: %N - %s", client, ClientSteamID);
	}
}

public BanClientID(client, time)
{
	decl String:ClientSteamID[32];
	GetClientAuthString(client, ClientSteamID, sizeof(ClientSteamID));
	ServerCommand("banid %d %s", time, ClientSteamID);
	ServerCommand("writeid");
	ServerCommand("kickid %d", GetClientUserId(client));
	new String:cvar_logfile_bans[128];
	GetConVarString(sm_logfile_bans, cvar_logfile_bans, sizeof(cvar_logfile_bans));
	if (StrEqual(cvar_logfile_bans, "", false) != true)
	{
		decl String:file[PLATFORM_MAX_PATH];
		BuildPath(Path_SM, file, sizeof(file), cvar_logfile_bans);	
		LogToFileEx(file, "BANID: %N - %s", client, ClientSteamID);
	}
}

public BanClientIP(client, time)
{
	decl String:ClientIP[24];
	GetClientIP(client, ClientIP, sizeof(ClientIP), true);
	ServerCommand("addip %d %s", time, ClientIP);
	ServerCommand("writeip");
	ServerCommand("kickid %d", GetClientUserId(client));	
	new String:cvar_logfile_bans[128];
	GetConVarString(sm_logfile_bans, cvar_logfile_bans, sizeof(cvar_logfile_bans));
	if (StrEqual(cvar_logfile_bans, "", false) != true)
	{
		decl String:file[PLATFORM_MAX_PATH];
		BuildPath(Path_SM, file, sizeof(file), cvar_logfile_bans);	
		LogToFileEx(file, "BANIP: %N - %s", client, ClientIP);
	}
}

public Action:OnClientCommand(client, args)
{
	decl String:CommandName[50];
	GetCmdArg(0, CommandName, sizeof(CommandName));
	
	if (GetConVarInt(sm_block_attack) > 0)
	{
		if (StrEqual(CommandName, "developer", false) || StrEqual(CommandName, "fps_modem", false) || StrEqual(CommandName, "fps_max", false))
		{
			switch (GetConVarInt(sm_block_attack))
			{
				case 1: BanClientID(client, 0);
				case 2: BanClientIP(client, 0);
			}
		}
		if (StrEqual(CommandName, "+speeding", false) || StrEqual(CommandName, "-speeding", false) || StrEqual(CommandName, "fps_max_override", false) || StrEqual(CommandName, "+nxhsON", false))
		{
			switch (GetConVarInt(sm_block_attack))
			{
				case 1: BanClientID(client, 180);
				case 2: BanClientIP(client, 180);
			}
		}
		if (StrEqual(CommandName, "hldj_playaudio", false) || StrEqual(CommandName, "net1", false) || StrEqual(CommandName, "zoom.in", false) || StrEqual(CommandName, "togglezoom", false) || StrEqual(CommandName, "viewmodel_fov_override", false))
		{
			if (GetConVarInt(sm_block_attack) > 0)
			{
				KickClientID(client);
			}
		}		
		
	}

	if (GetConVarInt(sm_logfile_filter) > 0)
	{
		if (GetConVarInt(sm_logfile_filter) > 0)
		{
			if (StrEqual(CommandName, "vocalize", false) || StrEqual(CommandName, "choose_opendoor", false) || StrEqual(CommandName, "choose_closedoor", false) || StrEqual(CommandName, "menuselect", false))
			{
				return Plugin_Continue;
			}
		}
		if (GetConVarInt(sm_logfile_filter) > 1)
		{
			if (StrEqual(CommandName, "joingame", false) || StrEqual(CommandName, "jointeam", false) || StrEqual(CommandName, "spec_next", false) || StrEqual(CommandName, "spec_prev", false) || StrEqual(CommandName, "spec_mode", false))
			{
				return Plugin_Continue;
			}
			if (StrEqual(CommandName, "sm_info", false) || StrEqual(CommandName, "sm_nextrank", false) || StrEqual(CommandName, "sm_rank", false) || StrEqual(CommandName, "SkipOuttro", false) || StrEqual(CommandName, "sm_next", false))
			{
				return Plugin_Continue;
			}
			if (StrEqual(CommandName, "sm_join", false) || StrEqual(CommandName, "sm_join2", false) || StrEqual(CommandName, "sm_points", false) || StrEqual(CommandName, "sm_maptop", false) || StrEqual(CommandName, "sm_top15", false))
			{
				return Plugin_Continue;
			}
		}
		if (GetConVarInt(sm_logfile_filter) > 2)
		{
			if (StrEqual(CommandName, "VModEnable", false) || StrEqual(CommandName, "vban", false))
			{
				return Plugin_Continue;
			}
		}
		if (GetConVarInt(sm_logfile_filter) > 3)
		{
			if (StrEqual(CommandName, "choose_closedoor", false) || StrEqual(CommandName, "choose_opendoor", false) || StrEqual(CommandName, "sm_spawnrandom", false) || StrEqual(CommandName, "sm_kickbots", false) || StrEqual(CommandName, "db_dublicate", false))
			{
				return Plugin_Continue;
			}
		}
		if (GetConVarInt(sm_logfile_filter) > 4)
		{
			if (StrEqual(CommandName, "sm_csm", false) || StrEqual(CommandName, "motd", false) || StrEqual(CommandName, "sm_admin", false) || StrEqual(CommandName, "Vote", false) || StrEqual(CommandName, "sm_suicide", false))
			{
				return Plugin_Continue;
			}
		}
		if (GetConVarInt(sm_logfile_filter) > 5)
		{
			if (StrEqual(CommandName, "sm_killtarget", false) || StrEqual(CommandName, "sm_cancelvote", false) || StrEqual(CommandName, "sm_test", false) || StrEqual(CommandName, "sm_spawn", false) || StrEqual(CommandName, "db_info", false))
			{
				return Plugin_Continue;
			}
		}
	}
	
	new String:cvar_logfile_commands[128];
	GetConVarString(sm_logfile_commands, cvar_logfile_commands, sizeof(cvar_logfile_commands));
	if (StrEqual(cvar_logfile_commands, "", false) == true)
	{
		return Plugin_Continue;
	}

	decl String:file[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, file, sizeof(file), cvar_logfile_commands);

	if (args > 0)
	{
		decl String:argstring[255];
		GetCmdArgString(argstring, sizeof(argstring));
		LogToFileEx(file, "%N - %s [%s]", client, CommandName, argstring);
		return Plugin_Continue;
	}

	LogToFileEx(file, "%N - %s", client, CommandName);
	return Plugin_Continue;
}

public Action:Command_Say(client, args)
{
	new String:cvar_logfile_chat[128];
	GetConVarString(sm_logfile_chat, cvar_logfile_chat, sizeof(cvar_logfile_chat));
	if (StrEqual(cvar_logfile_chat, "", false) == true)
	{
		return Plugin_Continue;
	}

	if (!client)
	{
		return Plugin_Continue;
	}
	
	decl String:text[192];
	if (!GetCmdArgString(text, sizeof(text)))
	{
		return Plugin_Continue;
	}
	
	new startidx = 0;

	if (text[strlen(text) - 1] == '"')
	{
		text[strlen(text) - 1] = '\0';
		startidx = 1;
	}

	decl String:file[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, file, sizeof(file), cvar_logfile_chat);
	LogToFileEx(file, "[%N]: %s", client, text[startidx]);
	return Plugin_Continue;
}