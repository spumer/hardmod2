#include <sourcemod>
#include <sdktools>

#define PLAYERS_ENABLED false
#define L4D_MAXPLAYERS	32

#define PLUGIN_NAME "L4D2 Events"
#define PLUGIN_AUTHOR "Jonny"
#define PLUGIN_DESCRIPTION "L4D2 Events"
#define PLUGIN_VERSION "1.0.4"
#define PLUGIN_URL ""

new bool:First_Player_Transitioned;

public Plugin:myinfo =
{
	name = PLUGIN_NAME,
	author = PLUGIN_AUTHOR,
	description = PLUGIN_DESCRIPTION,
	version = PLUGIN_VERSION,
	url = PLUGIN_URL
}

#include "hardmod/basic.inc"

new Handle:Plugin_Mode;
new Handle:Configs_Dir;

public OnPluginStart()
{
	new String:cvar_sm_logfile_events[256] = "logs/events.log";
	if (StrEqual(cvar_sm_logfile_events, "", false) != true)
	{
		decl String:file[PLATFORM_MAX_PATH];
		BuildPath(Path_SM, file, sizeof(file), cvar_sm_logfile_events);	
		LogToFileEx(file, "OnPluginStart()");
	}
	new String:moddir[64];
	GetGameFolderName(moddir, sizeof(moddir));
	if (!StrEqual(moddir, "left4dead2", false)) SetFailState("Use this plugin for Left 4 Dead 2 only.");
	CreateConVar("l4d2_advanced", PLUGIN_VERSION, "[L4D2] Events Version", FCVAR_PLUGIN|FCVAR_SPONLY|FCVAR_UNLOGGED|FCVAR_DONTRECORD|FCVAR_REPLICATED|FCVAR_NOTIFY);
	Plugin_Mode = CreateConVar("l4d2_advanced_mode", "2", "", FCVAR_PLUGIN|FCVAR_SPONLY|FCVAR_NOTIFY);
	Configs_Dir = CreateConVar("l4d2_config_dir", "events", "", FCVAR_PLUGIN|FCVAR_SPONLY|FCVAR_NOTIFY);
//	HookSourceServerEvents();
	HookL4D2Events();
}

public HookSourceServerEvents()
{
	HookEvent("server_spawn", Event_ExecConfig);
	HookEvent("server_shutdown", Event_ExecConfig);
	HookEvent("server_cvar", Event_ExecConfig);
//	HookEvent("server_msg", Event_ExecConfig); // Native "HookEvent" reported: Game event "server_msg" does not exist
	HookEvent("server_addban", Event_ExecConfig);
	HookEvent("server_removeban", Event_ExecConfig);
	HookEvent("player_connect", Event_ExecConfig);
	HookEvent("player_info", Event_ExecConfig);
	HookEvent("player_disconnect", Event_ExecConfig);
	HookEvent("player_activate", Event_ExecConfig);
	HookEvent("player_say", Event_ExecConfig);
}

public HookL4D2Events()
{
	HookEvent("round_freeze_end", Event_ExecConfig);
	HookEvent("round_start_pre_entity", Event_ExecConfig);
	HookEvent("round_start_post_nav", Event_ExecConfig);
//	HookEvent("nav_blocked", Event_ExecConfig);
	HookEvent("nav_generate", Event_ExecConfig);
	HookEvent("round_end_message", Event_ExecConfig);
	HookEvent("round_end", Event_ExecConfig);
	HookEvent("difficulty_changed", Event_ExecConfig);
	HookEvent("finale_start", Event_ExecConfig);
	HookEvent("finale_rush", Event_ExecConfig);
	HookEvent("finale_escape_start", Event_ExecConfig);
	HookEvent("finale_vehicle_incoming", Event_ExecConfig);
	HookEvent("finale_vehicle_ready", Event_ExecConfig);
	HookEvent("finale_vehicle_leaving", Event_ExecConfig);
	HookEvent("finale_win", Event_ExecConfig);
	HookEvent("mission_lost", Event_ExecConfig);
	HookEvent("finale_radio_start", Event_ExecConfig);
	HookEvent("finale_radio_damaged", Event_ExecConfig);
	HookEvent("final_reportscreen", Event_ExecConfig);
	HookEvent("map_transition", Event_ExecConfig);
	HookEvent("player_transitioned", Event_PlayerTransitioned);
	HookEvent("player_left_start_area", Event_ExecConfig);
	HookEvent("witch_spawn", Event_ExecConfig);
	HookEvent("witch_killed", Event_ExecConfig);
	HookEvent("tank_spawn", Event_ExecConfig);
	HookEvent("create_panic_event", Event_ExecConfig);
	HookEvent("weapon_spawn_visible", Event_ExecConfig);
	HookEvent("gameinstructor_draw", Event_ExecConfig);
	HookEvent("gameinstructor_nodraw", Event_ExecConfig);
	HookEvent("request_weapon_stats", Event_ExecConfig);
	HookEvent("player_talking_state", Event_ExecConfig);
	HookEvent("weapon_pickup", Event_ExecConfig);
	HookEvent("hunter_punched", Event_ExecConfig);
	HookEvent("tank_killed", Event_ExecConfig);
	HookEvent("gauntlet_finale_start", Event_ExecConfig);
	HookEvent("mounted_gun_start", Event_ExecConfig);
	HookEvent("mounted_gun_overheated", Event_ExecConfig);
	HookEvent("punched_clown", Event_ExecConfig);
	HookEvent("charger_killed", Event_ExecConfig);
	HookEvent("spitter_killed", Event_ExecConfig);
	HookEvent("jockey_killed", Event_ExecConfig, EventHookMode_Post);
//+	HookEvent("vomit_bomb_tank", Event_ExecConfig);
	HookEvent("triggered_car_alarm", Event_ExecConfig);
	HookEvent("panic_event_finished", Event_ExecConfig);
	HookEvent("song_played", Event_ExecConfig);
}

public OnMapStart()
{
	First_Player_Transitioned = false;
	PrintToAdmins("l4d2_events ( map_start )");
	ExecuteCFG("map_start");
}

public ExecuteCFG(const String:FileName[])
{
	if (GetConVarInt(Plugin_Mode) < 1) return;
	else if (GetConVarInt(Plugin_Mode) == 2)
	{
		new count = 0;
		for (new i = 1; i <= L4D_MAXPLAYERS; i++) if (IsValidClient (i) && IsClientInGame(i) && !IsFakeClient(i)) count++;
		if (!count) return;
	}
	new String:cvar_l4d2_config_dir[256];
	GetConVarString(Configs_Dir, cvar_l4d2_config_dir, sizeof(cvar_l4d2_config_dir));
	new String:CfgFileName[256];
	Format(CfgFileName, sizeof(CfgFileName), "%s/%s.cfg", cvar_l4d2_config_dir, FileName);
	new String:CfgFullFileName[256];
	Format(CfgFullFileName, sizeof(CfgFullFileName), "cfg/%s/%s.cfg", cvar_l4d2_config_dir, FileName);
	PrintToServer("exec %s", CfgFileName);
	if (FileExists(CfgFullFileName, false)) ServerCommand("exec %s", CfgFileName);
}

public Action:Event_PlayerTransitioned(Handle:event, const String:name[], bool:dontBroadcast)
{
	if (!First_Player_Transitioned)
	{
		First_Player_Transitioned = true;
		PrintToAdmins("l4d2_events ( first_player_transitioned )");
		ExecuteCFG("first_player_transitioned");
	}
	else Event_ExecConfig(event, name, dontBroadcast);
}

public Action:Event_ExecConfig(Handle:event, const String:name[], bool:dontBroadcast)
{
	PrintToAdmins("l4d2_events ( %s )", name);
	ExecuteCFG(name);
}