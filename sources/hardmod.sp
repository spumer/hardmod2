#include <sourcemod>
#include <sdktools>
#include <sdkhooks>
//#include <geoipcity>

#pragma dynamic 65535

#define HARDMOD true

#define PLUGIN_NAME "[L4D2] Hard Mod"
#define PLUGIN_AUTHOR "Jonny"
#define PLUGIN_DESCRIPTION "L4D2 Hard Mod"
#define PLUGIN_VERSION "0.2.9"
#define PLUGIN_URL "http://webj.narod.ru/projects/hardmod/"

#define HARDMOD_TAG "hardmod"

#include "hardmod/defines.inc"

#define TEAM_SPECTATORS 1
#define TEAM_SURVIVORS 2
#define TEAM_INFECTED 3

#define ZC_ZOMBIE 0
#define ZC_SMOKER 1
#define ZC_BOOMER 2
#define ZC_HUNTER 3
#define ZC_SPITTER 4
#define ZC_JOCKEY 5
#define ZC_CHARGER 6
#define ZC_WITCH 7
#define ZC_TANK 8

#define MAX_CMD_LINE_LENGTH 256
#define MAX_MAP_NAME_LENGTH 128
#define MAX_FILE_NAME_LENGTH 256
#define MAX_LINE_WIDTH 64
#define MAX_STEAM_LENGTH 20
#define MAX_COUNTRY_NAME_LENGTH 128
#define STEAM_GROUP_NAME_LENGTH 128
#define L4D_MINPLAYERS	4
#define L4D_MAXPLAYERS	32

public Plugin:myinfo =
{
	name = PLUGIN_NAME,
	author = PLUGIN_AUTHOR,
	description = PLUGIN_DESCRIPTION,
	version = PLUGIN_VERSION,
	url = PLUGIN_URL
}

new Version = 66;
new UpTime = 0;
new Handle:hardmod_version_autoshow;
new Handle:IsMapFinished;
new Handle:sv_tags;
new Handle:Cvar_Tag;
new Handle:LockCvar_Gamemode;
new Handle:Cvar_Gamemode;
new Handle:Cvar_VomitExtinguish;
new Handle:Cvar_Block_Vocalize;
new Handle:Cvar_Block_AFK;
new Handle:Cvar_Restore_COOP;
new Handle:Cvar_MOTD_Title;
new Handle:Cvar_MOTD_URL;
new Handle:Cvar_MOTD_File;
new Handle:Cvar_MOTD_Time;
new Handle:Cvar_MOTD_Command;
new Handle:Cvar_Disable_Grab;
new Handle:Cvar_Steam_Group_Name;
new Handle:Cvar_Log_File;
new Handle:Cvar_Log_Level;
new Handle:Cvar_DebugCheckPoints;
new Handle:Cvar_ConfigName;
new Handle:hm_spawn_file;
new Handle:hm_spawn_delay;

new bool:Log_To_Server = false;
new String:current_map[40];
new String:Server_UpTime[20];
new cvar_maxplayers;
new rounds = 0;
new round_start_time = 0;
new last_gamemode_cmd_used_time = 0;
new total_rounds_post_nav = 0;
new bool:IsFirstMapFlag = true;
new bool:IsCampaignDone = false;
new bool:IsMapChanging = false;
new bool:IsFirstRoundAfterLoad = true;
new bool:IsServerShuttingDown = false;

new LastHighCPUCMDTIME[L4D_MAXPLAYERS + 1];
new LastInfoTIME[L4D_MAXPLAYERS + 1];
new String:PlayersCustomSpawn[MAX_FILE_NAME_LENGTH];

#include "include/colors.inc"

#if BASIC_ENABLED
	#include "hardmod/basic.inc"
#endif

#if CRASHWATCHER_ENABLED
	#include "hardmod/crashwatcher.inc"
#endif

#if GLOW_ENABLED
	#include "hardmod/glow.inc"
#endif

#if KEEPITEMFIX_ENABLED
	#include "hardmod/keepitemfix.inc"
#endif

#if PLAYERS_ENABLED
	#include "hardmod/players.inc"
#endif

#if EFFECTS_ENABLED
	#include "hardmod/effects.inc"
#endif

#if COOP_STATS_ENABLED
	#include "hardmod/coop_stats.inc"
#endif

#if AMMOMOD_ENABLED
	#include "hardmod/ammomod.inc"
#endif

#if DAMAGEMOD_ENABLED
	#include "hardmod/damage4.inc"
#endif

#if COOP_AUTOD_ENABLED
	#include "hardmod/coop_autodifficulty2.inc"
#endif

#if SURVIVAL_ENABLED
	#include "hardmod/survival.inc"
#endif

#if SURV_AUTOD_ENABLED
	#include "hardmod/survival_autodifficulty.inc"
#endif

#if AIRSTRIKE_ENABLED
	#include "hardmod/airstrike.inc"
#endif

#if CHEAT_ENABLED
	#include "hardmod/cheat.inc"
#endif

#if FUN_ENABLED
	#include "hardmod/fun.inc"
#endif

#if HEALING_ENABLED
	#include "hardmod/healing.inc"
#endif

#if COOP_VOTES_ENABLED
	#include "hardmod/coop_votes.inc"
#endif

#if SURVIVAL_VOTES_ENABLED
	#include "hardmod/survival_votes.inc"
#endif

#if PANIC_ENABLED
	#include "hardmod/panic.inc"
#endif

#if PRECACHE_ENABLED
	#include "hardmod/precache.inc"
#endif

new bool:PostLoadCfgLoaded = false;

public OnPluginStart()
{
	UpTime = GetTime();
	decl String:moddir[24];
	GetGameFolderName(moddir, sizeof(moddir));
	if (!StrEqual(moddir, "left4dead2", false))
	{
		SetFailState("Use this plugin for Left 4 Dead 2 only, dude");
	}

	Cvar_Log_File = CreateConVar("hardmod_log_file", "hardmod/hardmod.log", "", FCVAR_PLUGIN);
	Cvar_Log_Level = CreateConVar("hardmod_log_level", "0", "", FCVAR_PLUGIN);
	
	Log("hardmod.sp / OnPluginStart() / 1", 1);

	CreateConVar("hardmod_version", PLUGIN_VERSION, "[L4D2] Hard Mod Version", FCVAR_PLUGIN|FCVAR_UNLOGGED|FCVAR_DONTRECORD|FCVAR_REPLICATED|FCVAR_NOTIFY);
	hardmod_version_autoshow = CreateConVar("hardmod_version_autoshow", PLUGIN_VERSION, "[L4D2] Hard Mod Version Show", FCVAR_PLUGIN);
	IsMapFinished = CreateConVar("hm_mapfinished", "0", "", FCVAR_PLUGIN);
	Cvar_Tag = CreateConVar("hardmod_tags", "", "", FCVAR_PLUGIN);
	Cvar_MOTD_Title = CreateConVar("hm_motd_title", "RUS COOP-16", "", FCVAR_PLUGIN);
	Cvar_MOTD_URL = CreateConVar("hm_motd_url", "", "", FCVAR_PLUGIN);
	Cvar_MOTD_File = CreateConVar("hm_motd_file", "hardmod_motd.txt", "", FCVAR_PLUGIN);
	Cvar_MOTD_Time = CreateConVar("hm_motd_time", "15", "", FCVAR_PLUGIN);
	Cvar_MOTD_Command = CreateConVar("hm_motd_command", "!news", "", FCVAR_PLUGIN);
	Cvar_Disable_Grab = CreateConVar("hm_disable_grab", "1", "", FCVAR_PLUGIN);
	Cvar_Steam_Group_Name = CreateConVar("hm_steam_group_name", "", "", FCVAR_PLUGIN);
	Cvar_DebugCheckPoints = CreateConVar("hm_print_checkpoints", "0", "", FCVAR_PLUGIN);
	Cvar_ConfigName = CreateConVar("hm_config_name", "default", "", FCVAR_PLUGIN|FCVAR_NOTIFY);

	#if CRASHWATCHER_ENABLED
		CWOnPluginStart();
	#endif

	#if GLOW_ENABLED
		GlowOnPluginStart();
	#endif

	#if AMMOMOD_ENABLED
		AmmoModOnPluginStart();
	#endif

	#if SURVIVAL_ENABLED
		SurvivalOnPluginStart();
	#endif

	#if COOP_AUTOD_ENABLED
		CoopAutoDiffOnPluginStart();
	#endif

	#if COOP_STATS_ENABLED
		CoopStatsOnPluginStart();
	#endif

	#if SURV_AUTOD_ENABLED
		AutoDiffOnPluginStart();
	#endif

	#if AIRSTRIKE_ENABLED
		AirStrikeOnPluginStart();
	#endif

	#if BASIC_ENABLED
		BasicOnPluginStart();
	#endif

	#if CHEAT_ENABLED
		CheatOnPluginStart();
	#endif

	#if FUN_ENABLED
		FunOnPluginStart();
	#endif

	#if DAMAGEMOD_ENABLED
		DamageOnPluginStart();
	#endif

	#if EFFECTS_ENABLED
		EffectsOnPluginStart();
	#endif

	#if HEALING_ENABLED
		HealingOnPluginStart();
	#endif

	#if KEEPITEMFIX_ENABLED
		KeepItemFixOnPluginStart();
	#endif

	#if COOP_VOTES_ENABLED
		CVotesOnPluginStart();
	#endif

	#if SURVIVAL_VOTES_ENABLED
		SVotesOnPluginStart();
	#endif

	#if PANIC_ENABLED
		PanicOnPluginStart();
	#endif

	#if PLAYERS_ENABLED
		PlayersOnPluginStart();
	#endif

	#if PRECACHE_ENABLED
		PrecacheOnPluginStart();
	#endif

	Log("hardmod.sp / OnPluginStart() / 2", 4);

	sv_tags = FindConVar("sv_tags");
	SetConVarFlags(sv_tags, FCVAR_NONE);
	AddServerTagFunc();
	
	BuildPath(Path_SM, PlayersCustomSpawn, sizeof(PlayersCustomSpawn), "hardmod/spawn.txt");
	hm_spawn_file = CreateConVar("hm_spawn_file", "hardmod/spawn.txt", "", FCVAR_PLUGIN);
	hm_spawn_delay = CreateConVar("hm_spawn_delay", "0.3", "", FCVAR_PLUGIN);
	HookConVarChange(hm_spawn_file, hm_spawn_file_changed);

	Cvar_Gamemode = FindConVar("mp_gamemode");
	LockCvar_Gamemode = CreateConVar("lock_gamemode", "", "", FCVAR_PLUGIN);
	Cvar_VomitExtinguish = CreateConVar("hm_vomitextinguish", "1", "", FCVAR_PLUGIN);

	Cvar_Block_Vocalize = CreateConVar("hm_blockvocalize", "1", "", FCVAR_PLUGIN);
	Cvar_Block_AFK = CreateConVar("hm_blockafk", "1", "", FCVAR_PLUGIN);
	Cvar_Restore_COOP = CreateConVar("hm_restore_coop", "1", "", FCVAR_PLUGIN);

	HookConVarChange(sv_tags, sv_tags_changed);
	HookConVarChange(Cvar_Gamemode, Cvar_Gamemode_Changed);
	HookConVarChange(FindConVar("sv_maxplayers"), cvar_maxplayers_changed);
	
	Log("hardmod.sp / OnPluginStart() / 3", 4);

	HookEvent("server_addban", Event_ServerAddBan, EventHookMode_Pre);

	HookEvent("finale_win", Event_FinalWin);
	HookEvent("map_transition", Event_MapTransition);
	HookEvent("player_transitioned", Event_PlayerTransitioned);
	HookEvent("round_start_post_nav", Event_RoundStartPostNav);
	HookEvent("round_start", Event_RoundStart);
	HookEvent("round_freeze_end", Event_RoundFreezeEnd);
	
	HookEvent("player_entered_checkpoint", Event_CheckPoint);
	HookEvent("player_now_it", Event_PlayerNowIt);
	HookEvent("player_death", Event_PlayerDeath);
	HookEvent("heal_success", Event_HealSuccess);
	HookEvent("pills_used", Event_PillsUsed);
	HookEvent("defibrillator_used", Event_DefibrillatorUsed);
	HookEvent("revive_success", Event_ReviveSuccess);
	HookEvent("player_first_spawn", Event_PlayerFirstSpawn);
	HookEvent("player_spawn", Event_PlayerSpawn);
	HookEvent("player_team", Event_PlayerTeam, EventHookMode_Post);
	HookEvent("player_jump", Event_PlayerJump);
	HookEvent("gameinstructor_nodraw", Event_GameInstructorNodraw);

	Log("hardmod.sp / OnPluginStart() / 4", 4);

	RegConsoleCmd("kickid", Command_KickID);
	RegConsoleCmd("sm_suicide", Command_Suicide);
	RegConsoleCmd("sm_modversion", Command_Version);
	RegConsoleCmd("sm_bw", Command_BlackWhite);
	RegConsoleCmd("vocalize", Command_Vocalize);
	RegConsoleCmd("changelevel", Command_ChangeLevel);
	RegConsoleCmd("exit", Command_Exit);
	RegConsoleCmd("go_away_from_keyboard", Command_AFK);
	RegConsoleCmd("say", Command_SAY);
//	RegConsoleCmd("sm_join", Command_JoinSurvivors);
	RegConsoleCmd("sm_id", Command_ID);
	RegConsoleCmd("sm_info", Command_Info);
	RegConsoleCmd("sm_serverinfo", Command_ServerInfo);
	RegConsoleCmd("sm_uptime", Command_UpTime);
	RegConsoleCmd("sm_ping", Command_Ping);

	RegAdminCmd("sm_kickfakeclients", Command_KickFakeClients, ADMFLAG_KICK, "sm_kickfakeclients (1 - spectators, 2 - survivors, 3 - infected)");
	RegAdminCmd("sm_kickextrabots", Command_KickExtraBots, ADMFLAG_KICK, "sm_kickextrabots");
	RegAdminCmd("sm_kickteam", Command_KickTeam, ADMFLAG_KICK, "sm_kickteam (1 - spectators, 2 - survivors, 3 - infected)");
	RegAdminCmd("sm_defreeze", Command_DeFreeze, ADMFLAG_CHEATS, "sm_defreeze");
	RegAdminCmd("sm_test", Command_Test, ADMFLAG_CHEATS, "sm_test");
	RegAdminCmd("sm_killtarget", Command_KillTarget, ADMFLAG_SLAY, "");
	RegAdminCmd("sm_togglefirstmap", Command_ToggleFirstMap, ADMFLAG_ROOT, "");
	RegAdminCmd("sm_gamemode", Command_GameMode, ADMFLAG_GENERIC, "");

	Log("hardmod.sp / OnPluginStart() / 5", 4);

	PrintToServer("# COMPILED ON SOURCEMOD : %s", SOURCEMOD_VERSION);
	PrintToServer("# MAX SUPPORTED PLAYERS : %d", L4D_MAXPLAYERS);

	PrintToChatAll("\x05HardMod (%d)\x04 activated", Version);
	PrintToServer("HardMod (%d) activated", Version);
#if COOP_STATS_ENABLED
	PrintToChatAll("\x05HardMod/COOP Stats (%d)\x04 activated", STATS_VERSION);
	PrintToServer("HardMod/COOP Stats (%d) activated", STATS_VERSION);
#endif
#if COOP_AUTOD_ENABLED
	PrintToChatAll("\x05HardMod/Autodifficulty\x04 activated");
	PrintToServer("HardMod/Autodifficulty activated");
#endif
#if AIRSTRIKE_ENABLED
	PrintToChatAll("\x05HardMod/\"Hell Box\"\x04 activated");
	PrintToServer("HardMod/\"Hell Box\" activated");
#endif
#if COOP_VOTES_ENABLED
	PrintToChatAll("\x05HardMod/Votes\x04 activated");
	PrintToServer("HardMod/Votes activated");
#endif
	Log("hardmod.sp / OnPluginStart() / 6 - end", 4);
	PrintToServer("[HARDMOD] exec hardmod/start.cfg");
	ServerCommand("exec hardmod/start");
}

#if DAMAGEMOD_ENABLED
	public PluginInitialization()
	{
		DamagePluginInitialization();
	}
#endif

Log(const String:LogReport[], loglevel)
{
	if (Log_To_Server) PrintToServer("[HARDMOD.%d|%d] %s", Version, loglevel, LogReport);
	if (GetConVarInt(Cvar_Log_Level) < loglevel) return;
	new String:cvar_logfile[MAX_FILE_NAME_LENGTH];
	GetConVarString(Cvar_Log_File, cvar_logfile, sizeof(cvar_logfile));
	if (StrEqual(cvar_logfile, "", false) == true)
	{
		return;
	}
	decl String:file[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, file, sizeof(file), cvar_logfile);
	LogToFileEx(file, "[HARDMOD.%d|%d] %s", Version, loglevel, LogReport);
	return;
}

public hm_spawn_file_changed(Handle:hVariable, const String:strOldValue[], const String:strNewValue[])
{
	BuildSpawnPath();
}

BuildSpawnPath()
{
	new String:cvar_hm_spawn_file[MAX_FILE_NAME_LENGTH];
	GetConVarString(hm_spawn_file, cvar_hm_spawn_file, sizeof(cvar_hm_spawn_file));
	if (StrEqual(cvar_hm_spawn_file, "", false) == true) return;
	BuildPath(Path_SM, PlayersCustomSpawn, sizeof(PlayersCustomSpawn), "hardmod/%s", cvar_hm_spawn_file);
}

public OnMapStart()
{
	Log("hardmod.sp / OnMapStart() / 1", 2);
	rounds = 1;

	IsCampaignDone = false;
	IsMapChanging = false;
	GetCurrentMap(current_map, 39);

	cvar_maxplayers = GetConVarInt(FindConVar("sv_maxplayers"));

	BuildSpawnPath();

	#if PLAYERS_ENABLED
		PlayersOnMapStart();
	#endif

	#if COOP_STATS_ENABLED
		CStatsOnMapStart();
	#endif
	
	#if COOP_AUTOD_ENABLED
		ADOnMapStart();
	#endif

	#if CRASHWATCHER_ENABLED
		CWOnMapStart();
	#endif

	#if GLOW_ENABLED
		GlowOnMapStart();
	#endif

	#if AMMOMOD_ENABLED
		AmmoModOnMapStart();
	#endif

	#if AIRSTRIKE_ENABLED
		AirStrikeOnMapStart();
	#endif

	#if EFFECTS_ENABLED
		EffectsOnMapStart();
	#endif

	#if KEEPITEMFIX_ENABLED
		KIFOnMapStart();
	#endif

	Log("hardmod.sp / OnMapStart() / 2 - end", 4);
}

CustomPlayerSpawn(client)
{
#if KEEPITEMFIX_ENABLED
	if (GetConVarInt(hardmod_KIF) != 2 || GetClientTeam(client) != TEAM_SURVIVORS) return;
#endif
	if (GetConVarFloat(hm_spawn_delay) < 0.1) CreateTimer(0.1, TimedPlayerSpawn, client);
	else CreateTimer(GetConVarFloat(hm_spawn_delay), TimedPlayerSpawn, client);
}

public Action:TimedPlayerSpawn(Handle:timer, any:client)
{
	new Handle:file = INVALID_HANDLE;
	file = OpenFile(PlayersCustomSpawn, "r");
	if (file == INVALID_HANDLE)
	{
		Log("hardmod.sp / CustomPlayerSpawn() / file == INVALID_HANDLE", 2);
		return;
	}
	FileSeek(file, 0, SEEK_SET);
	new String:Custom_Spawn_Lines[MAX_CMD_LINE_LENGTH];
	new bool:IsFirstLineFound = false;
	while (!IsEndOfFile(file))
	{
		if (!IsFirstLineFound)
		{
			IsFirstLineFound = true;
			for (new i = 0; i < 5; i++) if (GetPlayerWeaponSlot(client, i) > -1) RemovePlayerItem(client, GetPlayerWeaponSlot(client, i));
		}
		// we read the line
		if (ReadFileLine(file, Custom_Spawn_Lines, sizeof(Custom_Spawn_Lines)))
		{
			if (StrEqual(Custom_Spawn_Lines, "temphealth", false)) SwitchHealth(client);
			else if (StrEqual(Custom_Spawn_Lines, "blackwhite", false))
			{
				SetEntProp(client, Prop_Send, "m_currentReviveCount", 2);
				SetEntProp(client, Prop_Send, "m_isGoingToDie", 1);
			}
			else if (StrEqual(Custom_Spawn_Lines, "laser", false)) CheatCMD(client, "upgrade_add", "LASER_SIGHT");
			else CheatCMD(client, "give", Custom_Spawn_Lines);
		}
		else break;
	}
	CloseHandle(file);
}

public Action:TimedCheckRounds(Handle:timer, any:client)
{
	Log("hardmod.sp / Action:TimedCheckRounds() / 1", 4);
	if (IsServerProcessing())
	{
		Log("hardmod.sp / Action:TimedCheckRounds() / 2 - IsServerProcessing()", 4);
		new Players_Count = 0;
		for (new i = 1; i <= L4D_MAXPLAYERS; i++) if (IsClientConnected(i) && IsClientInGame(i)) Players_Count++;
		if (Players_Count < 1) rounds = 0;
	}
	else rounds = 0;
	Log("hardmod.sp / Action:TimedCheckRounds() / 3 - end", 4);
}

public Action:TimedCheckTag(Handle:timer, any:client)
{
	Log("hardmod.sp / Action:TimedCheckTag()", 4);
	AddServerTagFunc();
}

public Action:TimedConfigRun(Handle:timer, any:client)
{
	Log("hardmod.sp / Action:TimedConfigRun()", 4);
	if (PostLoadCfgLoaded) return Plugin_Stop;
	PrintToServer("[HARDMOD] exec server_postload.cfg");
	ServerCommand("exec server_postload.cfg");
	PostLoadCfgLoaded = true;
	return Plugin_Stop;
}

public Action:TimedMOTD(Handle:timer, any:client)
{
	Log("hardmod.sp / Action:TimedMOTD()", 4);
	if (!IsClientConnected(client)) return;
	if (!IsClientInGame(client)) return;
	new String:cvar_motd_title[128];
	GetConVarString(Cvar_MOTD_Title, cvar_motd_title, sizeof(cvar_motd_title));
	new String:cvar_motd_url[192];
	GetConVarString(Cvar_MOTD_URL, cvar_motd_url, sizeof(cvar_motd_url));
	if (!StrEqual(cvar_motd_url, "", false))
	{
		ShowMOTDPanel(client, cvar_motd_title, cvar_motd_url, MOTDPANEL_TYPE_URL);
	}
	else
	{
		new String:cvar_motd_file[MAX_FILE_NAME_LENGTH];
		GetConVarString(Cvar_MOTD_File, cvar_motd_file, sizeof(cvar_motd_file));
		ShowMOTDPanel(client, cvar_motd_title, cvar_motd_file, MOTDPANEL_TYPE_FILE);
	}
}

public Action:TimedAnnounce(Handle:timer, any:client)
{
	Log("hardmod.sp / Action:TimedAnnounce()", 4);
	if (!IsClientConnected(client)) return;
	if (!IsClientInGame(client)) return;
	#if defined STATS_VERSION
		PrintHintText(client, "HARDMOD VERSION %s (%d)\nSTATS VERSION: %d\nLatest Hardmod version: %s", PLUGIN_VERSION, Version, STATS_VERSION, PLUGIN_URL);
		PrintToChat(client, "\x05HARDMOD VERSION \x04%s\x05 (\x04%d\x05)\nSTATS VERSION: \x04%d\x05\nLatest Hardmod version: \x04%s", PLUGIN_VERSION, Version, STATS_VERSION, PLUGIN_URL);
	#else
		PrintHintText(client, "HARDMOD VERSION %s (%d)\nLatest Hardmod version: %s", PLUGIN_VERSION, Version, PLUGIN_URL);
		PrintToChat(client, "\x05HARDMOD VERSION \x04%s\x05 (\x04%d\x05)\nLatest Hardmod version: \x04%s", PLUGIN_VERSION, Version, PLUGIN_URL);
	#endif
}

public OnClientPostAdminCheck(client)
{
	Log("hardmod.sp / OnClientPostAdminCheck() / 1", 4);
	if (client < 1 || client > L4D_MAXPLAYERS)
		return;

	Log("hardmod.sp / OnClientPostAdminCheck() / 2", 4);

	#if SURV_AUTOD_ENABLED
		ADOnClientPostAdminCheck(client);
	#endif

	#if COOP_STATS_ENABLED
		CStatsOnClientPostAdminCheck(client);
	#endif

	#if PLAYERS_ENABLED
		PlayersOnClientPAC(client); // post admin check
	#endif

	#if KEEPITEMFIX_ENABLED
		KIFOnClientPostAdminCheck(client);
	#endif
//	if (GetClient() > 0 && !IsFakeClient(client)) KickFakeClients(0);
	if (GetConVarInt(hardmod_version_autoshow) > 0)
	{
		if (!IsFakeClient(client))
		{
			if (GetConVarInt(Cvar_MOTD_Time) > 0)
			{
				new String:cvar_motd_url[192];
				GetConVarString(Cvar_MOTD_URL, cvar_motd_url, sizeof(cvar_motd_url));
				new String:cvar_motd_file[MAX_FILE_NAME_LENGTH];
				GetConVarString(Cvar_MOTD_File, cvar_motd_file, sizeof(cvar_motd_file));
				if (!StrEqual(cvar_motd_url, "", false) || FileExists(cvar_motd_file, false))
				{
					CreateTimer(GetConVarInt(Cvar_MOTD_Time) * 1.0, TimedMOTD, client);
				}
			}
			CreateTimer(90.0 * GetRandomFloat(0.8, 1.6), TimedAnnounce, client);
		}
	}
	Log("hardmod.sp / OnClientPostAdminCheck() / 3 - end", 4);
}

public Action:Event_ServerAddBan(Handle:event, const String:name[], bool:dontBroadcast)
{
	Log("hardmod.sp / Action:Event_ServerAddBan()", 4);
	decl String:cvar_name[128];
	decl String:cvar_userid[128];
	decl String:cvar_networkid[128];
	GetEventString(event, "name", cvar_name, 128);
	GetEventString(event, "userid", cvar_userid, 128);
	GetEventString(event, "networkid", cvar_networkid, 128);
	CPrintToChatAll("\x5[BAN] user = \"{blue}%s\"\x5, userid = \"\x4%s\"\x5, netid = \"\x4%s\"", cvar_name, cvar_userid, cvar_networkid);
	return Plugin_Handled;
}

public OnClientDisconnect(client)
{
	Log("hardmod.sp / OnClientDisconnect()", 5);
	#if KEEPITEMFIX_ENABLED
		KIFOnClientDisconnect(client);
	#endif

	#if COOP_AUTOD_ENABLED
		CADOnClientDisconnect();
	#endif

	#if COOP_STATS_ENABLED
		CStatsOnClientDisconnect(client);
	#endif
	
	#if SURV_AUTOD_ENABLED
		ADOnClientDisconnect(client);
	#endif

	#if PLAYERS_ENABLED
		PlayersOnClientDisc(client); // post admin check
	#endif
	if (IsFirstMapFlag && !IsFakeClient(client)) CreateTimer(1.0, TimedCheckRounds);
}

public OnClientDisconnect_Post(client)
{
	Log("hardmod.sp / OnClientDisconnect_Post()", 5);
	#if KEEPITEMFIX_ENABLED
		KIFOnClientDisconnectPost(client);
	#endif

	#if COOP_STATS_ENABLED
		CSOnClientDisconnectPost(client);
	#endif
}

public OnClientConnected(client)
{
	Log("hardmod.sp / OnClientConnected()", 5);
	#if GLOW_ENABLED
		GlowOnClientConnected(client);
	#endif

	#if COOP_STATS_ENABLED
		CStatsOnClientConnected(client);
	#endif
	
	#if KEEPITEMFIX_ENABLED
		KIFOnClientConnected(client);
	#endif
}

public OnClientAuthorized(client, const String:auth[])
{
	Log("hardmod.sp / OnClientAuthorized()", 5);
	#if PLAYERS_ENABLED
		PlayersOnClientAuthorized(client, auth);
	#endif
}

public OnClientPutInServer(client)
{
	Log("hardmod.sp / OnClientPutInServer()", 5);
	#if COOP_STATS_ENABLED
		CoopStatsOnClientPutInServer(client);
	#endif
	
	#if KEEPITEMFIX_ENABLED
		KIFOnClientPutInServer(client);
	#endif

	#if DAMAGEMOD_ENABLED	
		DMOnClientPutInServer(client);
	#endif
}

public Cvar_Gamemode_Changed(Handle:hVariable, const String:strOldValue[], const String:strNewValue[])
{
	Log("hardmod.sp / Cvar_Gamemode_Changed()", 5);
	if (IsServerShuttingDown) return;
	new String:GameMode[20];
	GetConVarString(LockCvar_Gamemode, GameMode, sizeof(GameMode));
	if (StrEqual(GameMode, "", false)) return;
	if (!StrEqual(GameMode, strNewValue, false)) SetConVarString(Cvar_Gamemode, GameMode, false, false);
}

public cvar_maxplayers_changed(Handle:hVariable, const String:strOldValue[], const String:strNewValue[])
{
	cvar_maxplayers = GetConVarInt(FindConVar("sv_maxplayers"));
}

public Action:Command_KickID(client, args)
{
}

public Action:Command_Suicide(client, args)
{
	Log("hardmod.sp / Command_Suicide()", 4);
	if (!IsValidClient(client))
	{
		return Plugin_Handled;
	}
	ForcePlayerSuicide(client);
	return Plugin_Continue;
}

public Action:Command_Version(client, args)
{
	Log("hardmod.sp / Command_Version()", 4);
	if (client == 0)
	{
		PrintToServer("# COMPILED ON SOURCEMOD : %s", SOURCEMOD_VERSION);
		PrintToServer("# MAX SUPPORTED PLAYERS : %d", L4D_MAXPLAYERS);
		PrintToServer("----------------------------");
		PrintToServer("MOD VERSION = %s (%d)", PLUGIN_VERSION, Version);
		#if COOP_STATS_ENABLED
			PrintToServer("STATS VERSION = %d", STATS_VERSION);
		#endif
		#if SURVIVAL_ENABLED
			PrintToServer("INC: SURVIVAL");
		#endif
	}
	else
	{
		PrintToChat(client, "# COMPILED ON SOURCEMOD : %s", SOURCEMOD_VERSION);
		PrintToChat(client, "# MAX SUPPORTED PLAYERS : %d", L4D_MAXPLAYERS);
		PrintToChat(client, "----------------------------");
		PrintToChat(client, "\x05MOD VERSION = \x03%s \x05(\x03%d\x05)", PLUGIN_VERSION, Version);
		#if COOP_STATS_ENABLED
			PrintToChat(client, "\x05STATS VERSION = \x03%d", STATS_VERSION);
		#endif
		#if SURVIVAL_ENABLED
			PrintToServer("\x05INC: \x03SURVIVAL");
		#endif
	}
	return Plugin_Continue;
}

public Action:Command_BlackWhite(client, args)
{
	Log("hardmod.sp / Command_BlackWhite()", 4);
	new j = 0;
	if (client == 0)
	{
		for (new i = 1; i <= L4D_MAXPLAYERS; i++)
		{
			if (IsClientConnected(i) && IsClientInGame(i))
			{
				if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
				{
					if (GetEntProp(i, Prop_Send, "m_currentReviveCount") > 1)
					{
						new flag_health_items = false;
						new String:Message[256];
						new String:TempMessage[22];
						if (GetPlayerWeaponSlot(i, 3) != -1)
						{
							GetWeaponNameAtSlot(i, 3, TempMessage, 22);
							if (StrEqual(TempMessage, "weapon_first_aid_kit", false))
							{
								if (!flag_health_items)
								{
									flag_health_items = true;
									StrCat(String:Message, sizeof(Message), "(");
								}
								StrCat(String:Message, sizeof(Message), "medkit");
							}
							if (StrEqual(TempMessage, "weapon_defibrillator", false))
							{
								if (!flag_health_items)
								{
									flag_health_items = true;
									StrCat(String:Message, sizeof(Message), "(");
								}
								StrCat(String:Message, sizeof(Message), "defibrillator");
							}
						}
						if (GetPlayerWeaponSlot(i, 4) != -1)
						{
							if (!flag_health_items)
							{
								flag_health_items = true;
								StrCat(String:Message, sizeof(Message), "(");
							}
							else
							{
								StrCat(String:Message, sizeof(Message), ", ");
							}
							GetWeaponNameAtSlot(i, 4, TempMessage, 22);
							if (StrEqual(TempMessage, "weapon_pain_pills", false)) StrCat(String:Message, sizeof(Message), "pills");
							if (StrEqual(TempMessage, "weapon_adrenaline", false)) StrCat(String:Message, sizeof(Message), "adrenaline");
						}
						if (flag_health_items)
						{
							StrCat(String:Message, sizeof(Message), ")");
							PrintToServer("Going to die: %N %s", i, Message);
						}
						else
						{
							PrintToServer("Going to die: %N", i);
						}
						j++;
					}
				}
			}
		}
		if (j == 0) PrintToServer("Nobody is going to die");
	}
	else
	{
		for (new i = 1; i <= L4D_MAXPLAYERS; i++)
		{
			if (IsClientConnected(i) && IsClientInGame(i))
			{
				if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
				{
					if (GetEntProp(i, Prop_Send, "m_currentReviveCount") > 1)
					{
						new flag_health_items = false;
						new String:Message[256];
						new String:TempMessage[22];
						if (GetPlayerWeaponSlot(i, 3) != -1)
						{
							GetWeaponNameAtSlot(i, 3, TempMessage, 22);
							if (StrEqual(TempMessage, "weapon_first_aid_kit", false))
							{
								if (!flag_health_items)
								{
									flag_health_items = true;
									StrCat(String:Message, sizeof(Message), "(");
								}
								StrCat(String:Message, sizeof(Message), "medkit");
							}
							if (StrEqual(TempMessage, "weapon_defibrillator", false))
							{
								if (!flag_health_items)
								{
									flag_health_items = true;
									StrCat(String:Message, sizeof(Message), "(");
								}
								StrCat(String:Message, sizeof(Message), "defibrillator");
							}
						}
						if (GetPlayerWeaponSlot(i, 4) != -1)
						{
							if (!flag_health_items)
							{
								flag_health_items = true;
								StrCat(String:Message, sizeof(Message), "(");
							}
							else
							{
								StrCat(String:Message, sizeof(Message), ", ");
							}
							GetWeaponNameAtSlot(i, 4, TempMessage, 22);
							if (StrEqual(TempMessage, "weapon_pain_pills", false)) StrCat(String:Message, sizeof(Message), "pills");
							if (StrEqual(TempMessage, "weapon_adrenaline", false)) StrCat(String:Message, sizeof(Message), "adrenaline");
						}
						if (flag_health_items)
						{
							StrCat(String:Message, sizeof(Message), ")");
							CPrintToChat(client, "\x01Going to die: {blue}%N \x04%s", i, Message);
						}
						else
						{
							CPrintToChat(client, "\x01Going to die: {blue}%N", i);
						}
						j++;
					}
				}
			}
		}
		if (j == 0)
		{
			PrintHintText(client, "Nobody is going to die");
		}
	}
}

public Action:Command_Vocalize(client, args)
{
	Log("hardmod.sp / Command_Vocalize()", 5);
	if (GetConVarInt(Cvar_Block_Vocalize) > 0)
	{
		return Plugin_Handled;
	}
	return Plugin_Continue;
}

public Action:Command_ChangeLevel(client, args)
{
	Log("hardmod.sp / Command_ChangeLevel()", 3);
	IsMapChanging = true;
	if (GetConVarInt(IsMapFinished) > 0)
	{
		if (IsCampaignDone) IsFirstMapFlag = true;
		else IsFirstMapFlag = false;
	}
	else IsFirstMapFlag = true;
	return Plugin_Continue;
}

public Action:Command_Exit(client, args)
{
	Log("hardmod.sp / Command_Exit()", 3);
	IsServerShuttingDown = true;
	Command_ChangeLevel(client, args);
	return Plugin_Continue;
}

public Action:Command_AFK(client, args)
{
	Log("hardmod.sp / Command_AFK()", 4);
	if (GetConVarInt(Cvar_Block_AFK) > 1) return Plugin_Handled;
#if COOP_STATS_ENABLED
	if (GetConVarInt(Cvar_Block_AFK) > 0)
	{
		if (ChachedPoints[client] >= 0 && !IsIncapacitated(client) && GetHealth(client) > 1) return Plugin_Continue;
		return Plugin_Handled;
	}
#endif
	return Plugin_Continue;
}

public Action:Command_SAY(client, args)
{
	Log("hardmod.sp / Command_SAY()", 4);
	decl String:text[256];
	if (!GetCmdArgString(text, sizeof(text)))
	{
		return Plugin_Continue;
	}
	if (client != 0)
	{
		new String:cvar_motd_command[64];
		GetConVarString(Cvar_MOTD_Command, cvar_motd_command, sizeof(cvar_motd_command));

		if (!StrEqual(cvar_motd_command, "", false))
		{
			new startidx = 0;
			if (text[strlen(text)-1] == '"')
			{
					text[strlen(text)-1] = '\0';
					startidx = 1;
			}
			if (strcmp(text[startidx], cvar_motd_command, false) == 0)
			{
				new String:cvar_motd_url[192];
				GetConVarString(Cvar_MOTD_URL, cvar_motd_url, sizeof(cvar_motd_url));
				new String:cvar_motd_file[128];
				GetConVarString(Cvar_MOTD_File, cvar_motd_file, sizeof(cvar_motd_file));
				if (!StrEqual(cvar_motd_url, "", false) || FileExists(cvar_motd_file, false))
				{
					CreateTimer(0.3, TimedMOTD, client);
				}			
			}
		}	
		return Plugin_Continue;
	}
	new String:HostName[48];
	new Handle:gamecvar_hostname = FindConVar("hostname");
	GetConVarString(gamecvar_hostname, HostName, sizeof(HostName));
	PrintToChatAll("\x4%s\x1 : %s", HostName, text);
	return Plugin_Handled;
}

public Action:Command_JoinSurvivors(client, args)
{
	if (GetClientTeam(client) != TEAM_SURVIVORS && GetFakeClient() != 0) FakeClientCommand(client, "jointeam 2");
}

public Action:Command_ID(client, args)
{
	ReplyToCommand(client, "Your client ID is %d", client);
	ReplyToCommand(client, "Your zombie class is %d", GetClientZC(client));
}

public Action:Command_Info(client, args)
{
	if (client == 0)
	{
		Command_ServerInfo(client, args);
		return Plugin_Continue;		
	}
//	if (!IsPlayerCPUAllowed(client)) return Plugin_Handled;
	if (LastInfoTIME[client] + 1 >= GetTime())
	{
		LastInfoTIME[client] = GetTime();
		Command_ServerInfo(client, args);
		return Plugin_Continue;
	}
	LastInfoTIME[client] = GetTime();
	new Target;
	if (args > 0)
	{
		decl String:temp_text[40];
		GetCmdArg(1, temp_text, sizeof(temp_text));
		if (StrEqual(temp_text, "@me", false)) Target = client;
	}
	else
	{
		if (!Target) Target = GetClientAimTarget(client, false);
		if (!IsValidClient(Target))
		{
		#if COOP_AUTOD_ENABLED
			Command_ADInfo(client, args);
		#endif
			return Plugin_Continue;
		}
		if (IsFakeClient(Target))
		{
			switch (GetClientZC(Target))
			{
				case ZC_SMOKER: CPrintToChat(client, "{red}Smoker \x05health: \x04%d", GetClientHealth(Target));
				case ZC_BOOMER: CPrintToChat(client, "{red}Boomer \x05health: \x04%d", GetClientHealth(Target));
				case ZC_JOCKEY: CPrintToChat(client, "{red}Jockey \x05health: \x04%d", GetClientHealth(Target));
				case ZC_HUNTER: CPrintToChat(client, "{red}Hunter health: \x04%d", GetClientHealth(Target));
				case ZC_SPITTER: CPrintToChat(client, "{red}Spitter \x05health: \x04%d", GetClientHealth(Target));
				case ZC_CHARGER: CPrintToChat(client, "{red}Charger \x05health: \x04%d", GetClientHealth(Target));
				case ZC_TANK: CPrintToChat(client, "{red}Tank \x05health: \x04%d", GetClientHealth(Target));
			}
			return Plugin_Continue;
		}
	}
	if (GetClientTeam(Target) == TEAM_SURVIVORS)
	{
		new String:Message[256];
		new String:line_user_level[4];
		new String:line_user_admin[7];
		new AdminId:AId = GetUserAdmin(Target);
		new flags = GetAdminFlags(AId, Access_Effective);
		new bool:is_target_admin = false;
		if (flags & ADMFLAG_GENERIC || flags & ADMFLAG_ROOT || flags & ADMFLAG_RCON|| flags & ADMFLAG_BAN || flags & ADMFLAG_KICK || flags & ADMFLAG_SLAY) is_target_admin = true;
		new target_level = GetTargetLevel(client, Target);
		if (target_level < 1)
		{
			line_user_level = "?";
			if (flags & ADMFLAG_GENERIC) line_user_admin = "admin";
			else line_user_admin = "player";
		}
		else
		{
			Format(line_user_level, sizeof(line_user_level), "%d", target_level);
			if (is_target_admin) line_user_admin = "admin";
			else line_user_admin = "player";
		}
		Format(Message, sizeof(Message), "\x05Name: {blue}%N \x05| Status: \x04%s \x05| Level: в€† \x04%s\n", Target, line_user_admin, line_user_level);
#if COOP_STATS_ENABLED
		new String:TempMessage[256];
		Format(TempMessage, sizeof(TempMessage), "\x05Rank: \x04%d \x05| Points: \x04%d", ClientRank[Target], ClientPoints[Target]);
		StrCat(String:Message, sizeof(Message), TempMessage);
#endif
		CPrintToChat(client, "%s", Message);
		Format(Message, sizeof(Message), "\x05Health: \x04%d", GetClientHealthTotal(Target));
		CPrintToChat(client, "%s", Message);
#if PLAYERS_ENABLED
		new String:TempGroupMessage[256];
		if (StrEqual(SteamGroupNames[Target], "", false)) TempGroupMessage = "none";
		else TempGroupMessage = SteamGroupNames[Target];
		if (StrEqual(Country[Target], "", false)) Country[Target] = "Unknown";
		Format(Message, sizeof(Message), "\x05Location: в—Џ \x04%s \x05| Group: \x04%s", Country[Target], TempGroupMessage);
		CPrintToChat(client, "%s", Message);
#endif
		AId = GetUserAdmin(client);
		flags = GetAdminFlags(AId, Access_Effective);
		if (flags & ADMFLAG_BAN || flags & ADMFLAG_ROOT)
		{
			decl String:ClientIP[24];
			GetClientIP(Target, ClientIP, sizeof(ClientIP), true);
			decl String:ClientSteamID[32];
			GetClientAuthString(Target, ClientSteamID, sizeof(ClientSteamID));
			Format(Message, sizeof(Message), "\x05IP: \x04%s \x05| ID: \x04%s", ClientIP, ClientSteamID);
			CPrintToChat(client, "%s", Message);
		}
	}
	return Plugin_Continue;
}

public Action:Command_ServerInfo(client, args)
{
	new String:HostName[96];
	GetConVarString(FindConVar("hostname"), HostName, sizeof(HostName));
	new String:ConfigName[96];
	GetConVarString(Cvar_ConfigName, ConfigName, sizeof(ConfigName));
	UpdateServerUpTime();
	new String:SteamGroupName[96];
	GetConVarString(Cvar_Steam_Group_Name, SteamGroupName, sizeof(SteamGroupName));

	new total_ping = 0;
	new total_players = 0;
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidClient (i) && IsClientInGame(i) && !IsFakeClient(i))
		{
			total_ping += RoundToZero(GetClientAvgLatency(i, NetFlow_Both) * 1000.0);
			total_players++;
		}
	}
	if (total_players) total_ping = RoundToZero(total_ping / total_players * 1.0);
	if (client > 0)
	{
		PrintToChat(client, "\x05Server Informantion:");
		PrintToChat(client, "\x05Host: \x04%s \x05| SteamGroup: \x04%s", HostName, SteamGroupName);
		PrintToChat(client, "\x05Config: \x04%s", ConfigName);
		PrintToChat(client, "\x05Host FPS: \x04%d \x05| Average ping: \x04%d \x05| UpTime: \x04%s", GetConVarInt(FindConVar("fps_max")), total_ping, Server_UpTime);
	}
	else
	{
		PrintToServer("Server Informantion:");
		PrintToServer("Host: %s | SteamGroup: %s", HostName, SteamGroupName);
		PrintToServer("Config: %s", ConfigName);
		PrintToServer("Host FPS: %d | Average ping: %d | UpTime: %s", GetConVarInt(FindConVar("fps_max")), total_ping, Server_UpTime);
	}
}

public Action:Command_UpTime(client, args)
{
	UpdateServerUpTime();
	if (client) PrintToChat(client, "\x05Server uptime is \x04%s", Server_UpTime);
	else PrintToServer("Server uptime is %s", Server_UpTime);
}

public Action:Command_Ping(client, args)
{
	PrintToChat(client, "\x05Ping (Current / Average):\nOutgouing: \x04%d / %d\x05 | Incoming: \x04%d / %d\x05 | Both: \x04%d / %d", RoundToZero(GetClientLatency(client, NetFlow_Outgoing) * 1000.0),
	RoundToZero(GetClientAvgLatency(client, NetFlow_Outgoing) * 1000.0), RoundToZero(GetClientLatency(client, NetFlow_Incoming) * 1000.0), RoundToZero(GetClientAvgLatency(client, NetFlow_Incoming) * 1000.0),
	RoundToZero(GetClientLatency(client, NetFlow_Both) * 1000.0), RoundToZero(GetClientAvgLatency(client, NetFlow_Both) * 1000.0));
}

public Action:Command_KickFakeClients(client, args)
{
	Log("hardmod.sp / Command_KickFakeClients()", 3);
	if (args < 1)
	{
		ReplyToCommand(client, "[SM] Usage: sm_kickfakeclients (1 - spectators, 2 - survivors, 3 - infected)");
		return Plugin_Handled;
	}
	decl String:arg[8];
	GetCmdArg(1, arg, sizeof(arg));
	KickFakeClients(StringToInt(arg));
	return Plugin_Continue;
}

public Action:Command_KickExtraBots(client, args)
{
	Log("hardmod.sp / Command_KickExtraBots()", 3);
	new clients = 0;
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i) && IsClientInGame(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS)
			{
				if (clients > 3)
				{
					if (IsFakeClient(i)) ServerKickClient(i);
				}
				else clients++;
			}
		}
	}
	return Plugin_Continue;
}

public Action:Command_KickTeam(client, args)
{
	Log("hardmod.sp / Command_KickTeam()", 3);
	if (args < 1)
	{
		ReplyToCommand(client, "[SM] Usage: sm_kickteam (1 - spectators, 2 - survivors, 3 - infected)");
		return Plugin_Handled;
	}
	decl String:arg[8];
	GetCmdArg(1, arg, sizeof(arg));
	KickTeam(StringToInt(arg));

	return Plugin_Continue;
}

public Action:Command_DeFreeze(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i) && IsClientInGame(i) && !IsFakeClient(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS)
			{
//				SetEntProp(client, Prop_Send, "m_fFlags", GetEntProp(client, Prop_Send, "m_fFlags") & ~FL_FROZEN);
//				Native "GetEntProp" reported: Property "m_fFlags" not found (entity 0/worldspawn)
			}
		}
	}
	return Plugin_Continue;
}

stock GetPluginMode()
{
	return GetConVarInt(Plugin_Mode);
}

public Action:Command_Test(client, args)
{
	Log("hardmod.sp / Command_Test()", 3);
	if (args < 1)
	{
		return Plugin_Handled;
	}

	new Float:position[3];
	GetEntPropVector(client, Prop_Send, "m_vecOrigin", position);

	decl String:command_text[192];
	GetCmdArg(1, command_text, sizeof(command_text));

	if (StrEqual(command_text, "boom", false))
	{
		Boom(position);
	}
	if (StrEqual(command_text, "boom2", false))
	{
		Boom2(position);
	}
	if (StrEqual(command_text, "fire", false))
	{
		Fire(position);
	}
#if AIRSTRIKE_ENABLED	
	if (StrEqual(command_text, "airstrike", false))
	{
		Airstrike(client);
	}
#endif	

	return Plugin_Continue;
}

public Action:Command_KillTarget(client, args)
{
	Log("hardmod.sp / Command_KillTarget()", 3);
	new Target;
	Target = GetClientAimTarget(client, false);
	if (!IsValidClient(Target)) return Plugin_Continue;
	if (IsClientConnected(Target) && IsClientInGame(Target) && IsPlayerAlive(Target))
	{
		if (GetClientTeam(Target) == TEAM_SURVIVORS) ForcePlayerSuicide(Target);
		else
		{
			new AdminId:ClientAdminId = GetUserAdmin(client);
			new flags = GetAdminFlags(ClientAdminId, Access_Effective);
			if (flags & ADMFLAG_ROOT) ForcePlayerSuicide(Target);
		}
	}
	return Plugin_Continue;
}

public Action:Command_ToggleFirstMap(client, args)
{
	Log("hardmod.sp / Command_ToggleFirstMap()", 3);
	IsFirstMapFlag = !IsFirstMapFlag;
}

public Action:Command_GameMode(client, args)
{
	if (GetTime() == last_gamemode_cmd_used_time) return Plugin_Handled;
	last_gamemode_cmd_used_time = GetTime();
	Log("hardmod.sp / Command_GameMode()", 3);
	if (args < 1)
	{
		new String:GameMode[20];
		GetConVarString(Cvar_Gamemode, GameMode, sizeof(GameMode));
		if (client == 0)
		{
			PrintToServer("\"mp_gamemode\" = \"%s\"", GameMode);
		}
		else
		{
			ReplyToCommand(client, "\"mp_gamemode\" = \"%s\"", GameMode);
		}
		return Plugin_Continue;
	}
	if (client == 0)
	{
		decl String:command_text[192];
		GetCmdArg(1, command_text, sizeof(command_text));
		
		SetConVarString(LockCvar_Gamemode, command_text, false, false);
		SetConVarString(Cvar_Gamemode, command_text, false, false);
	}
	else
	{
		new AdminId:ClientAdminId = GetUserAdmin(client);
		new flags = GetAdminFlags(ClientAdminId, Access_Effective);
		if (flags & ADMFLAG_ROOT || flags & ADMFLAG_CONVARS	|| flags & ADMFLAG_RCON)
		{
			decl String:command_text[192];
			GetCmdArg(1, command_text, sizeof(command_text));
			
			SetConVarString(LockCvar_Gamemode, command_text, false, false);
			SetConVarString(Cvar_Gamemode, command_text, false, false);
		}
		ReplyToCommand(client, "Access denied: not enough privileges");
		return Plugin_Handled;
	}
	return Plugin_Continue;
}

public Action:Event_FinalWin(Handle:event, const String:name[], bool:dontBroadcast)
{
	Log("hardmod.sp / Event_FinalWin()", 4);
	IsCampaignDone = true;
}

public Action:Event_MapTransition(Handle:event, const String:name[], bool:dontBroadcast)
{
	Log("hardmod.sp / Event_MapTransition()", 2);
//	PrintToChatAll("\x05public Action:Event_MapTransition(Handle:event, const String:name[], bool:dontBroadcast)");
	IsCampaignDone = false;
	IsFirstMapFlag = false;
	IsMapChanging = true;
	rounds = 1;
	CreateTimer(1.0, TimedCheckTag, _);
	#if COOP_STATS_ENABLED
		CStatsMapTransitionEvent();
	#endif

	#if HEALING_ENABLED
		NormalizeHealth();
	#endif
	
	#if KEEPITEMFIX_ENABLED
		KIFMapTransitionEvent();
	#endif
}

public Action:Event_PlayerTransitioned(Handle:event, const String:name[], bool:dontBroadcast)
{
	Log("hardmod.sp / Event_PlayerTransitioned()", 4);
	#if KEEPITEMFIX_ENABLED
		KIFPlayerTransitionEvent(event);
	#endif
}

public Action:Event_RoundStartPostNav(Handle:event, const String:name[], bool:dontBroadcast)
{
	Log("hardmod.sp / Event_RoundStartPostNav()", 2);
	total_rounds_post_nav++;
	if (!PostLoadCfgLoaded && total_rounds_post_nav > 1)
	{
//		ServerCommand("exec server_postload.cfg");
		CreateTimer(5.0, TimedConfigRun, _, TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);
		PrintToServer("server_postload done?");
//		PostLoadCfgLoaded = true;
	}
//	if (!PostLoadCfgLoaded) CreateTimer(5.0, TimedConfigRun, _, TIMER_REPEAT|TIMER_FLAG_NO_MAPCHANGE);

	IsCampaignDone = false;
	#if KEEPITEMFIX_ENABLED
		KIFRoundStartPostNavEvent();
	#endif
}

public Action:Event_RoundFreezeEnd(Handle:event, const String:name[], bool:dontBroadcast)
{
	Log("hardmod.sp / Event_RoundFreezeEnd()", 4);
	IsCampaignDone = false;
	#if KEEPITEMFIX_ENABLED
		KIFRoundFreezeEndEvent();
	#endif
}

public Action:Event_RoundStart(Handle:event, const String:name[], bool:dontBroadcast)
{
	Log("hardmod.sp / Event_RoundStart()", 2);
	new bool:EmptyRound = false;
	if (round_start_time == 0 || GetTime() - round_start_time > 14)	rounds++;
	else EmptyRound = true;
	round_start_time = GetTime();
	if (rounds < 1) rounds = 1;
	if (GetConVarInt(Cvar_Restore_COOP) > 0)
	{
		SetConVarString(LockCvar_Gamemode, "coop", false, false);
		SetConVarString(Cvar_Gamemode, "coop", false, false);
	}

	SetConVarInt(IsMapFinished, 0, false, false);
	
	#if COOP_STATS_ENABLED
		CSRoundStartEvent();
	#endif

	#if COOP_AUTOD_ENABLED
		ADRoundStart();
	#endif

	#if HEALING_ENABLED
		HealRoundStartEvent();
	#endif	
	if (!EmptyRound) IsFirstRoundAfterLoad = false;
}

UpdateServerUpTime()
{
	decl String:str_uptime_temp[8];
	new Current_UpTime = GetTime() - UpTime;
	new Days = RoundToFloor(Current_UpTime / 86400.0);
	Current_UpTime -= Days * 86400;
	if (Days > 0)
	{
		if (Days > 1) Format(Server_UpTime, sizeof(Server_UpTime), "%d days ", Days);
		else Format(Server_UpTime, sizeof(Server_UpTime), "1 day ");
	}
	else Server_UpTime = "";
	new Hours = RoundToFloor(Current_UpTime / 3600.0);
	if (Hours < 10) Format(str_uptime_temp, sizeof(str_uptime_temp), "0%d:", Hours);
	else Format(str_uptime_temp, sizeof(str_uptime_temp), "%d:", Hours);
	StrCat(String:Server_UpTime, sizeof(Server_UpTime), str_uptime_temp);
	Current_UpTime -= Hours * 3600;
	FormatTime(str_uptime_temp, sizeof(str_uptime_temp), "%M:%S", Current_UpTime);
	StrCat(String:Server_UpTime, sizeof(Server_UpTime), str_uptime_temp);
}

AddServerTagFunc()
{
	Log("hardmod.sp / AddServerTagFunc()", 5);
//	AddServerTag("hardmod");
	new String:cvar_sv_tags[256];
	new ReplaceCount = 0;
	new bool:ForceTagUpdate = false; // появились избыточные переменные и действия, но мне пока пофиг.
	GetConVarString(Cvar_Tag, cvar_sv_tags, sizeof(cvar_sv_tags));
	if (StrEqual(cvar_sv_tags, "", false))
	{
		GetConVarString(sv_tags, cvar_sv_tags, sizeof(cvar_sv_tags));
		ReplaceCount += ReplaceString(cvar_sv_tags, sizeof(cvar_sv_tags), "increased_maxplayers", "", false);
		ReplaceCount += ReplaceString(cvar_sv_tags, sizeof(cvar_sv_tags), "empty", "", false);
		for (new i = 0; i < ReplaceCount; i++) ReplaceString(cvar_sv_tags, sizeof(cvar_sv_tags), ",,", ",", false);
	}
	else ForceTagUpdate = true;
	new String:cvar_hardmod_tag[22];
	#if COOP_STATS_ENABLED
		Format(cvar_hardmod_tag, sizeof(cvar_hardmod_tag), "hm.s-%s.%d", PLUGIN_VERSION, Version);
	#else
		Format(cvar_hardmod_tag, sizeof(cvar_hardmod_tag), "hm-%s.%d", PLUGIN_VERSION, Version);
	#endif
	if (StrContains(cvar_sv_tags, cvar_hardmod_tag, false) == -1 || ForceTagUpdate)
	{
		Format(cvar_sv_tags, sizeof(cvar_sv_tags), "%s,%s", cvar_hardmod_tag, cvar_sv_tags);
		if (StrContains(cvar_sv_tags, HARDMOD_TAG, false) == -1)
		{
			Format(cvar_sv_tags, sizeof(cvar_sv_tags), "%s,%s", HARDMOD_TAG, cvar_sv_tags);
		}
		ReplaceCount++;
	}
	if (ReplaceCount == 0 && !ForceTagUpdate) return;
	SetConVarString(sv_tags, cvar_sv_tags, false, false);
	CreateTimer(0.1, TimedCheckTag, _);
}

public sv_tags_changed(Handle:hVariable, const String:strOldValue[], const String:strNewValue[])
{
	Log("hardmod.sp / sv_tags_changed()", 5);
	AddServerTagFunc();
}

public CheckPointReached(any:client)
{
	Log("hardmod.sp / CheckPointReached()", 4);
	SetConVarInt(IsMapFinished, 1, false, false);
	ServerCommand("exec hardmod/checkpointreached");
}

public Action:Event_CheckPoint(Handle:event, const String:name[], bool:dontBroadcast)
{
	Log("hardmod.sp / Event_CheckPoint()", 6);
	if (GetConVarInt(IsMapFinished) > 0 && !GetConVarInt(Cvar_DebugCheckPoints))
	{
		return Plugin_Continue;
	}
	new Target = GetClientOfUserId(GetEventInt(event, "userid"));
	decl String:strBuffer[128];
	GetEventString(event, "doorname", strBuffer, sizeof(strBuffer));
	
	if (Target && (GetClientTeam(Target)) == TEAM_SURVIVORS)
	{
		if (StrEqual(strBuffer, "checkpoint_entrance", false))
		{
			CheckPointReached(Target);
		}
		else
		{
			new area = GetEventInt(event, "area");
			if (GetConVarInt(Cvar_DebugCheckPoints) > 0)
			{
				PrintToChatAll("\x05MAP: \x04%s\x05, AREA: \x04%d", current_map, GetEventInt(event, "area"));
			}
			if (StrEqual(current_map, "c2m1_highway", false))
			{
				if (area == 89583)
					CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "c4m4_milltown_b", false))
			{
				if (area == 502575)
					CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "c5m1_waterfront", false))
			{
				if (area == 54867)
					CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "c5m2_park", false))
			{
				if (area == 196623)
					CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "c7m1_docks", false))
			{
				if (area == 4475)
					CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "c7m2_barge", false))
			{
				if (area == 52626)
					CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "l4d_ihm01_forest", false))
			{
				if (area == 10116)
					CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "l4d_ihm02_manor", false))
			{
				if (area == 3976)
					CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "l4d_ihm03_underground", false))
			{
				if (area == 5360)
					CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "c9m1_alleys", false))
			{
				if (area == 21211)
					CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "c10m4_mainstreet", false))
			{
				if (area == 85038)
					CheckPointReached(Target);
				if (area == 85093)
					CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "C12m1_hilltop", false))
			{
				if (area == 60481)
					CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "c13m1_alpinecreek", false))
			{
				if (area == 14681)
					CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "c13m2_southpinestream", false))
			{
				if (area == 2910)
					CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "c13m3_memorialbridge", false))
			{
				if (area == 154511)
					CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "evac2", false))
			{
				if (area == 48189)
					CheckPointReached(Target);
				if (area == 48994)
					CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "l4d_viennacalling_kaiserfranz", false))
			{
				if (area == 7025)
					CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "l4d_viennacalling_gloomy", false))
			{
				if (area == 15716)
					CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "uf1_boulevard", false))
			{
				if (area == 77346)
				CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "uf2_rooftops", false))
			{
				if (area == 12535)
				CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "uf3_harbor", false))
			{
				if (area == 190081)
				CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "uf4_airfield", false))
			{
				if (area == 147)
				CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "p84m1_crash", false))
			{
				if (area == 17314)
				CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "p84m2_train", false))
			{
				if (area == 26813)
				CheckPointReached(Target);
			}
			else if (StrEqual(current_map, "p84m3_clubd", false))
			{
				if (area == 20148)
				CheckPointReached(Target);
			}
		}
	}
	return Plugin_Continue;
}

public Action:Event_PlayerNowIt(Handle:event, const String:name[], bool:dontBroadcast)
{
	Log("hardmod.sp / Event_PlayerNowIt()", 5);
	#if COOP_STATS_ENABLED
		StatsEvent_PlayerNowIt(event);
	#endif
	if (GetConVarInt(Cvar_VomitExtinguish) > 0)
	{
		ExtinguishEntity(GetClientOfUserId(GetEventInt(event, "userid")));
	}
}

public Action:Event_PlayerDeath(Handle:event, const String:name[], bool:dontBroadcast)
{
	Log("hardmod.sp / Event_PlayerDeath()", 5);
	#if GLOW_ENABLED
		GlowEvent_PlayerDeath(event, name, dontBroadcast);
	#endif
}

public Action:Event_HealSuccess(Handle:event, const String:name[], bool:dontBroadcast)
{
	Log("hardmod.sp / Event_HealSuccess()", 4);
	#if GLOW_ENABLED
		GlowEvent_HealSuccess(event, name, dontBroadcast);
	#endif
}

public Action:Event_PillsUsed(Handle:event, const String:name[], bool:dontBroadcast)
{
	Log("hardmod.sp / Event_PillsUsed()", 4);
	#if GLOW_ENABLED
		GlowEvent_PillsUsed(event, name, dontBroadcast);
	#endif
	#if COOP_STATS_ENABLED
		StatsEvent_PillsUsed(event, name, dontBroadcast);
	#endif
}

public Action:Event_DefibrillatorUsed(Handle:event, const String:name[], bool:dontBroadcast)
{
	Log("hardmod.sp / Event_DefibrillatorUsed()", 4);
	#if GLOW_ENABLED
		GlowEvent_DefibrillatorUsed(event, name, dontBroadcast);
	#endif
	#if KEEPITEMFIX_ENABLED
		KIFDefibrillatorUsed(event);
	#endif
}

public Action:Event_ReviveSuccess(Handle:event, const String:name[], bool:dontBroadcast)
{
	Log("hardmod.sp / Event_ReviveSuccess()", 4);
	#if GLOW_ENABLED
		GlowEvent_ReviveSuccess(event, name, dontBroadcast);
	#endif
}

public Action:Event_PlayerFirstSpawn(Handle:event, const String:name[], bool:dontBroadcast)
{
	Log("hardmod.sp / Event_PlayerFirstSpawn()", 5);
	#if COOP_STATS_ENABLED
		CSPlayerFirstSpawn(event);
	#endif
	new isbot = GetEventInt(event, "isbot");
	if (isbot) return;
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	CustomPlayerSpawn(client);
}

public Action:Event_PlayerSpawn(Handle:event, const String:name[], bool:dontBroadcast)
{
	Log("hardmod.sp / Event_PlayerSpawn()", 5);
	#if KEEPITEMFIX_ENABLED
		KIFPlayerSpawn(event);
	#endif

	#if COOP_STATS_ENABLED
		CSPlayerSpawn(event);
	#endif

	#if COOP_AUTOD_ENABLED
		ADPlayerSpawn(event);
	#endif

	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	CustomPlayerSpawn(client);
}

public Action:Event_PlayerTeam(Handle:event, const String:name[], bool:dontBroadcast)
{
	Log("hardmod.sp / Event_PlayerTeam()", 5);

	#if COOP_AUTOD_ENABLED
		ADPlayerTeam();
	#endif
}

public Action:Event_PlayerJump(Handle:event, const String:name[], bool:dontBroadcast)
{
	if (GetConVarInt(Cvar_Disable_Grab) < 1) return;
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	AcceptEntityInput(client, "DisableLedgeHang");
	CreateTimer(0.1, TimedPlayerJump, _);
}

public Action:TimedPlayerJump(Handle:timer, any:client)
{
	AcceptEntityInput(client, "DisableLedgeHang");
	return Plugin_Continue;
}

public Action:Event_GameInstructorNodraw(Handle:event, const String:name[], bool:dontBroadcast)
{
	Log("hardmod.sp / Event_GameInstructorNodraw()", 4);
	#if COOP_STATS_ENABLED
		CSGameInstructorNodraw();
	#endif
}

bool:IsPlayerCPUAllowed(client)
{
	if (LastHighCPUCMDTIME[client] + 1 > GetTime()) return false;
	LastHighCPUCMDTIME[client] = GetTime();
	return true;
}