#include <sourcemod>

#define CVAR_FLAGS FCVAR_PLUGIN
#define PLUGIN_VERSION "1.3.Lite"

new Handle:IsPluginEnabled;
new Handle:Default_Config_Name;

public Plugin:myinfo = 
{
	name = "Map Config Loader",
	author = "Jonny",
	description = "Executes a config file based on the current map",
	version = PLUGIN_VERSION,
	url = ""
}

public OnPluginStart()
{
	Default_Config_Name = CreateConVar("map_script_default_config", "_default", "", FCVAR_PLUGIN|FCVAR_NOTIFY);
	IsPluginEnabled = CreateConVar("map_script_enabled", "1", "", FCVAR_PLUGIN|FCVAR_NOTIFY);
	CreateConVar("map_script_version", PLUGIN_VERSION, "Version of the map config loader plugin.", FCVAR_PLUGIN|FCVAR_NOTIFY);
	RegAdminCmd("map_execute", Command_Execute, ADMFLAG_ROOT, "");
}

public OnMapStart()
{
	if (GetConVarInt(IsPluginEnabled) < 1) return;
	new String:current_map[64];
	GetCurrentMap(current_map, 63);
	new String:CfgFullFileName[256];
	Format(CfgFullFileName, sizeof(CfgFullFileName), "cfg/maps/%s.cfg", current_map);
	if (!FileExists(CfgFullFileName, false))
	{
		new String:cvar_config_name[256];
		GetConVarString(Default_Config_Name, cvar_config_name, sizeof(cvar_config_name));
		ServerCommand("exec maps/%s", cvar_config_name);
	}
}

public Action:Command_Execute(client, args)
{
	LoadMapScript();
}

LoadMapScript()
{
	new String:current_map[64];
	GetCurrentMap(current_map, 63);
	new String:CfgFullFileName[256];
	Format(CfgFullFileName, sizeof(CfgFullFileName), "cfg/maps/%s.cfg", current_map);
	if (!FileExists(CfgFullFileName, false))
	{
		new String:cvar_config_name[256];
		GetConVarString(Default_Config_Name, cvar_config_name, sizeof(cvar_config_name));
		ServerCommand("exec maps/%s", cvar_config_name);
	}
	else ServerCommand("exec maps/%s", current_map);
}