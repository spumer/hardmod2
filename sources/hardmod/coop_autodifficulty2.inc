new Float:RDifficultyMultiplier = 1.0;
new BonusDrop = 0;
new playerscount = 4;
#if COOP_STATS_ENABLED
	new Float:rank_sum = 0.0;
#endif
new Handle:PDifficultyMultiplier;
new bool:l4d2_plugin_loot = true;
new bool:l4d2_plugin_monsterbots = true;
new bool:extra_charger = false;
new bool:autodifficulty_calculated = false;
new Handle:hm_autodifficulty;
new Handle:hm_autodifficulty_forcehp;
#if COOP_STATS_ENABLED
	new Handle:hm_rankmod_mode;
	new Handle:hm_rankmod_min;
	new Handle:hm_rankmod_max;
	new Handle:hm_rankmod_logarithm;
#endif
new Handle:z_difficulty;
new Handle:z_special_spawn_interval;
new Handle:special_respawn_interval;

new Handle:hm_auto_tongue_range_min;
new Handle:hm_auto_tongue_range_max;
new Handle:hm_auto_tongue_choke_dmg_min;
new Handle:hm_auto_tongue_choke_dmg_max;
new Handle:hm_auto_tongue_drag_dmg_min;
new Handle:hm_auto_tongue_drag_dmg_max;
new Handle:hm_auto_smoker_pz_claw_dmg_min;
new Handle:hm_auto_smoker_pz_claw_dmg_max;
new Handle:hm_auto_jockey_pz_claw_dmg_min;
new Handle:hm_auto_jockey_pz_claw_dmg_max;

new Handle:hm_auto_grenade_lr_dmg_min;
new Handle:hm_auto_grenade_lr_dmg_max;

new Handle:hm_loot_mod;
new Handle:hm_tank_hp_mod;
new Handle:hm_infected_hp_mod;
new Handle:hm_spawn_time_mod;
new Handle:hm_spawn_count_mod;

new Handle:hm_special_infected_min;
new Handle:hm_special_infected_max;
new Handle:hm_spawn_interval_min;
new Handle:hm_spawn_interval_max;
new Handle:hm_tank_burn_duration_min;
new Handle:hm_tank_burn_duration_max;

new Handle:hm_autohp_automod;
#if DAMAGEMOD_ENABLED
new Handle:hm_autohp_supercharger_auto;
#endif
new Handle:hm_autohp_zombie_min;
new Handle:hm_autohp_zombie_max;
new Handle:hm_autohp_hunter_min;
new Handle:hm_autohp_hunter_max;
new Handle:hm_autohp_smoker_min;
new Handle:hm_autohp_smoker_max;
new Handle:hm_autohp_boomer_min;
new Handle:hm_autohp_boomer_max;
new Handle:hm_autohp_jockey_min;
new Handle:hm_autohp_jockey_max;
new Handle:hm_autohp_charger_min;
new Handle:hm_autohp_charger_max;
new Handle:hm_autohp_spitter_min;
new Handle:hm_autohp_spitter_max;
new Handle:hm_autohp_witch_min;
new Handle:hm_autohp_witch_max;
new Handle:hm_autohp_tank_min;
new Handle:hm_autohp_tank_max;

new Handle:hm_items_automod;
new Handle:hm_items_supercharger_auto;
new Handle:hm_items_hunter_min;
new Handle:hm_items_hunter_max;
new Handle:hm_items_smoker_min;
new Handle:hm_items_smoker_max;
new Handle:hm_items_boomer_min;
new Handle:hm_items_boomer_max;
new Handle:hm_items_jockey_min;
new Handle:hm_items_jockey_max;
new Handle:hm_items_charger_min;
new Handle:hm_items_charger_max;
new Handle:hm_items_spitter_min;
new Handle:hm_items_spitter_max;
new Handle:hm_items_tank_min;
new Handle:hm_items_tank_max;

new Handle:hm_spawn_automod;
new Handle:hm_spawn_zombie_min;
new Handle:hm_spawn_zombie_max;
new Handle:hm_spawn_hunter_min;
new Handle:hm_spawn_hunter_max;
new Handle:hm_spawn_smoker_min;
new Handle:hm_spawn_smoker_max;
new Handle:hm_spawn_boomer_min;
new Handle:hm_spawn_boomer_max;
new Handle:hm_spawn_jockey_min;
new Handle:hm_spawn_jockey_max;
new Handle:hm_spawn_charger_min;
new Handle:hm_spawn_charger_max;
new Handle:hm_spawn_spitter_min;
new Handle:hm_spawn_spitter_max;

new Handle:hm_speed_automod;
new Handle:hm_speed_hunter_min;
new Handle:hm_speed_hunter_max;
new Handle:hm_speed_smoker_min;
new Handle:hm_speed_smoker_max;
new Handle:hm_speed_boomer_min;
new Handle:hm_speed_boomer_max;
new Handle:hm_speed_jockey_min;
new Handle:hm_speed_jockey_max;
new Handle:hm_speed_charger_min;
new Handle:hm_speed_charger_max;
new Handle:hm_speed_spitter_min;
new Handle:hm_speed_spitter_max;
new Handle:hm_speed_tank_min;
new Handle:hm_speed_tank_max;

new String:sGameDifficulty[16];
new cvar_difficulty = 1;
//new Handle:l4d2_loot_g_chance_nodrop;

// ZC_ZOMBIE 0 (common)
// ZC_SMOKER 1
// ZC_BOOMER 2
// ZC_HUNTER 3
// ZC_SPITTER 4
// ZC_JOCKEY 5
// ZC_CHARGER 6
// ZC_WITCH 7
// ZC_TANK 8

new AutodifficultyHP[L4D_MAXPLAYERS + 1][9];
new AutodifficultyGrenadeLRDmg[L4D_MAXPLAYERS + 1];
new AutodifficultyItems[L4D_MAXPLAYERS + 1][9];
new AutodifficultySpawnLimit[L4D_MAXPLAYERS + 1][9];
new AutodifficultySpeed[L4D_MAXPLAYERS + 1][9];
new AutodifficultySpawnInterval[L4D_MAXPLAYERS + 1];
new AutodifficultySpawnCount[L4D_MAXPLAYERS + 1];
new AutodifficultyTongueRange[L4D_MAXPLAYERS + 1];
new AutodifficultyTongueChokeDmg[L4D_MAXPLAYERS + 1];
new AutodifficultyTongueDragDmg[L4D_MAXPLAYERS + 1];
new AutodifficultySmokerClawDmg[L4D_MAXPLAYERS + 1];
new AutodifficultyJockeyClawDmg[L4D_MAXPLAYERS + 1];
new AutodifficultyTankBurnTime[L4D_MAXPLAYERS + 1];

public CoopAutoDiffOnPluginStart()
{
	Version += 39;

	hm_autodifficulty = CreateConVar("hm_autodifficulty", "1", "Is the plugin enabled.", FCVAR_PLUGIN);
	hm_autodifficulty_forcehp = CreateConVar("hm_autodifficulty_forcehp", "0", "", FCVAR_PLUGIN);
#if COOP_STATS_ENABLED	
	hm_rankmod_mode = CreateConVar("hm_rankmod_mode", "0", "", FCVAR_PLUGIN);
	hm_rankmod_min = CreateConVar("hm_rankmod_min", "0.66", "", FCVAR_PLUGIN);
	hm_rankmod_max = CreateConVar("hm_rankmod_max", "1.5", "", FCVAR_PLUGIN);
	hm_rankmod_logarithm = CreateConVar("hm_rankmod_logarithm", "-2.0", "", FCVAR_PLUGIN);
#endif
	PDifficultyMultiplier = CreateConVar("hm_difficulty_p", "1.0", "", FCVAR_PLUGIN);
	hm_loot_mod = CreateConVar("hm_loot_mod", "1.0", "", FCVAR_PLUGIN);
	hm_tank_hp_mod = CreateConVar("hm_tank_hp_mod", "1.0", "", FCVAR_PLUGIN);
	hm_infected_hp_mod = CreateConVar("hm_infected_hp_mod", "1.0", "", FCVAR_PLUGIN);
	hm_spawn_time_mod = CreateConVar("hm_spawn_time_mod", "1.0", "", FCVAR_PLUGIN);
	hm_spawn_count_mod = CreateConVar("hm_spawn_count_mod", "1.0", "", FCVAR_PLUGIN);

	z_difficulty = FindConVar("z_difficulty");
	HookConVarChange(z_difficulty, z_difficulty_changed);

	z_special_spawn_interval = FindConVar("z_special_spawn_interval");
	special_respawn_interval = FindConVar("director_special_respawn_interval");

	hm_auto_tongue_range_min = CreateConVar("hm_auto_tongue_range_min", "750", "", FCVAR_PLUGIN);
	hm_auto_tongue_range_max = CreateConVar("hm_auto_tongue_range_max", "1500", "", FCVAR_PLUGIN);
	hm_auto_tongue_choke_dmg_min = CreateConVar("hm_auto_tongue_choke_dmg_min", "30", "", FCVAR_PLUGIN);
	hm_auto_tongue_choke_dmg_max = CreateConVar("hm_auto_tongue_choke_dmg_max", "87", "", FCVAR_PLUGIN);
	hm_auto_tongue_drag_dmg_min = CreateConVar("hm_auto_tongue_drag_dmg_min", "9", "", FCVAR_PLUGIN);
	hm_auto_tongue_drag_dmg_max = CreateConVar("hm_auto_tongue_drag_dmg_max", "35", "", FCVAR_PLUGIN);
	hm_auto_smoker_pz_claw_dmg_min = CreateConVar("hm_auto_smoker_pz_claw_dmg_min", "4", "", FCVAR_PLUGIN);
	hm_auto_smoker_pz_claw_dmg_max = CreateConVar("hm_auto_smoker_pz_claw_dmg_max", "15", "", FCVAR_PLUGIN);
	hm_auto_jockey_pz_claw_dmg_min = CreateConVar("hm_auto_jockey_pz_claw_dmg_min", "4", "", FCVAR_PLUGIN);
	hm_auto_jockey_pz_claw_dmg_max = CreateConVar("hm_auto_jockey_pz_claw_dmg_max", "15", "", FCVAR_PLUGIN);

	hm_auto_grenade_lr_dmg_min = CreateConVar("hm_auto_grenade_lr_dmg_min", "400", "", FCVAR_PLUGIN);
	hm_auto_grenade_lr_dmg_max = CreateConVar("hm_auto_grenade_lr_dmg_max", "3000", "", FCVAR_PLUGIN);

	hm_special_infected_min = CreateConVar("hm_special_infected_min", "4", "", FCVAR_PLUGIN);
	hm_special_infected_max = CreateConVar("hm_special_infected_max", "18", "", FCVAR_PLUGIN);
	hm_spawn_interval_min = CreateConVar("hm_spawn_interval_min", "8", "", FCVAR_PLUGIN);
	hm_spawn_interval_max = CreateConVar("hm_spawn_interval_max", "18", "", FCVAR_PLUGIN);
	hm_tank_burn_duration_min = CreateConVar("hm_tank_burn_duration_min", "75", "", FCVAR_PLUGIN);
	hm_tank_burn_duration_max = CreateConVar("hm_tank_burn_duration_max", "300", "", FCVAR_PLUGIN);
	
	hm_autohp_automod = CreateConVar("hm_autohp_automod", "1", "", FCVAR_PLUGIN);
#if DAMAGEMOD_ENABLED
	hm_autohp_supercharger_auto = CreateConVar("hm_autohp_supercharger_auto", "0.66", "", FCVAR_PLUGIN);
#endif
	hm_autohp_zombie_min = CreateConVar("hm_autohp_zombie_min", "50", "", FCVAR_PLUGIN);
	hm_autohp_zombie_max = CreateConVar("hm_autohp_zombie_max", "50", "", FCVAR_PLUGIN);
	hm_autohp_hunter_min = CreateConVar("hm_autohp_hunter_min", "250", "", FCVAR_PLUGIN);
	hm_autohp_hunter_max = CreateConVar("hm_autohp_hunter_max", "1400", "", FCVAR_PLUGIN);
	hm_autohp_smoker_min = CreateConVar("hm_autohp_smoker_min", "250", "", FCVAR_PLUGIN);
	hm_autohp_smoker_max = CreateConVar("hm_autohp_smoker_max", "2500", "", FCVAR_PLUGIN);
	hm_autohp_boomer_min = CreateConVar("hm_autohp_boomer_min", "50", "", FCVAR_PLUGIN);
	hm_autohp_boomer_max = CreateConVar("hm_autohp_boomer_max", "500", "", FCVAR_PLUGIN);
	hm_autohp_jockey_min = CreateConVar("hm_autohp_jockey_min", "325", "", FCVAR_PLUGIN);
	hm_autohp_jockey_max = CreateConVar("hm_autohp_jockey_max", "2400", "", FCVAR_PLUGIN);
	hm_autohp_spitter_min = CreateConVar("hm_autohp_spitter_min", "100", "", FCVAR_PLUGIN);
	hm_autohp_spitter_max = CreateConVar("hm_autohp_spitter_max", "1000", "", FCVAR_PLUGIN);
	hm_autohp_charger_min = CreateConVar("hm_autohp_charger_min", "600", "", FCVAR_PLUGIN);
	hm_autohp_charger_max = CreateConVar("hm_autohp_charger_max", "1500", "", FCVAR_PLUGIN);
	hm_autohp_witch_min = CreateConVar("hm_autohp_witch_min", "1000", "", FCVAR_PLUGIN);
	hm_autohp_witch_max = CreateConVar("hm_autohp_witch_max", "1500", "", FCVAR_PLUGIN);
	hm_autohp_tank_min = CreateConVar("hm_autohp_tank_min", "6000", "", FCVAR_PLUGIN);
	hm_autohp_tank_max = CreateConVar("hm_autohp_tank_max", "150000", "", FCVAR_PLUGIN);

	hm_items_automod = CreateConVar("hm_items_automod", "1", "", FCVAR_PLUGIN);
	hm_items_supercharger_auto = CreateConVar("hm_items_supercharger_auto", "2", "", FCVAR_PLUGIN);
	hm_items_hunter_min = CreateConVar("hm_items_hunter_min", "3", "", FCVAR_PLUGIN);
	hm_items_hunter_max = CreateConVar("hm_items_hunter_max", "6", "", FCVAR_PLUGIN);
	hm_items_smoker_min = CreateConVar("hm_items_smoker_min", "1", "", FCVAR_PLUGIN);
	hm_items_smoker_max = CreateConVar("hm_items_smoker_max", "3", "", FCVAR_PLUGIN);
	hm_items_boomer_min = CreateConVar("hm_items_boomer_min", "1", "", FCVAR_PLUGIN);
	hm_items_boomer_max = CreateConVar("hm_items_boomer_max", "3", "", FCVAR_PLUGIN);
	hm_items_jockey_min = CreateConVar("hm_items_jockey_min", "1", "", FCVAR_PLUGIN);
	hm_items_jockey_max = CreateConVar("hm_items_jockey_max", "3", "", FCVAR_PLUGIN);
	hm_items_charger_min = CreateConVar("hm_items_charger_min", "1", "", FCVAR_PLUGIN);
	hm_items_charger_max = CreateConVar("hm_items_charger_max", "2", "", FCVAR_PLUGIN);
	hm_items_spitter_min = CreateConVar("hm_items_spitter_min", "1", "", FCVAR_PLUGIN);
	hm_items_spitter_max = CreateConVar("hm_items_spitter_max", "2", "", FCVAR_PLUGIN);
	hm_items_tank_min = CreateConVar("hm_items_tank_min", "4", "", FCVAR_PLUGIN);
	hm_items_tank_max = CreateConVar("hm_items_tank_max", "15", "", FCVAR_PLUGIN);

	hm_spawn_automod = CreateConVar("hm_spawn_automod", "1", "", FCVAR_PLUGIN);
	hm_spawn_zombie_min = CreateConVar("hm_spawn_zombie_min", "30", "", FCVAR_PLUGIN);
	hm_spawn_zombie_max = CreateConVar("hm_spawn_zombie_max", "30", "", FCVAR_PLUGIN);
	hm_spawn_hunter_min = CreateConVar("hm_spawn_hunter_min", "1", "", FCVAR_PLUGIN);
	hm_spawn_hunter_max = CreateConVar("hm_spawn_hunter_max", "3", "", FCVAR_PLUGIN);
	hm_spawn_smoker_min = CreateConVar("hm_spawn_smoker_min", "1", "", FCVAR_PLUGIN);
	hm_spawn_smoker_max = CreateConVar("hm_spawn_smoker_max", "3", "", FCVAR_PLUGIN);
	hm_spawn_boomer_min = CreateConVar("hm_spawn_boomer_min", "1", "", FCVAR_PLUGIN);
	hm_spawn_boomer_max = CreateConVar("hm_spawn_boomer_max", "5", "", FCVAR_PLUGIN);
	hm_spawn_jockey_min = CreateConVar("hm_spawn_jockey_min", "1", "", FCVAR_PLUGIN);
	hm_spawn_jockey_max = CreateConVar("hm_spawn_jockey_max", "3", "", FCVAR_PLUGIN);
	hm_spawn_spitter_min = CreateConVar("hm_spawn_spitter_min", "1", "", FCVAR_PLUGIN);
	hm_spawn_spitter_max = CreateConVar("hm_spawn_spitter_max", "3", "", FCVAR_PLUGIN);
	hm_spawn_charger_min = CreateConVar("hm_spawn_charger_min", "1", "", FCVAR_PLUGIN);
	hm_spawn_charger_max = CreateConVar("hm_spawn_charger_max", "2", "", FCVAR_PLUGIN);

	hm_speed_automod = CreateConVar("hm_speed_automod", "1", "", FCVAR_PLUGIN);
	hm_speed_hunter_min = CreateConVar("hm_speed_hunter_min", "300", "", FCVAR_PLUGIN);
	hm_speed_hunter_max = CreateConVar("hm_speed_hunter_max", "350", "", FCVAR_PLUGIN);
	hm_speed_smoker_min = CreateConVar("hm_speed_smoker_min", "210", "", FCVAR_PLUGIN);
	hm_speed_smoker_max = CreateConVar("hm_speed_smoker_max", "315", "", FCVAR_PLUGIN);
	hm_speed_boomer_min = CreateConVar("hm_speed_boomer_min", "175", "", FCVAR_PLUGIN);
	hm_speed_boomer_max = CreateConVar("hm_speed_boomer_max", "280", "", FCVAR_PLUGIN);
	hm_speed_jockey_min = CreateConVar("hm_speed_jockey_min", "250", "", FCVAR_PLUGIN);
	hm_speed_jockey_max = CreateConVar("hm_speed_jockey_max", "300", "", FCVAR_PLUGIN);
	hm_speed_charger_min = CreateConVar("hm_speed_charger_min", "250", "", FCVAR_PLUGIN);
	hm_speed_charger_max = CreateConVar("hm_speed_charger_max", "300", "", FCVAR_PLUGIN);
	hm_speed_spitter_min = CreateConVar("hm_speed_spitter_min", "210", "", FCVAR_PLUGIN);
	hm_speed_spitter_max = CreateConVar("hm_speed_spitter_max", "315", "", FCVAR_PLUGIN);
	hm_speed_tank_min = CreateConVar("hm_speed_tank_min", "210", "", FCVAR_PLUGIN);
	hm_speed_tank_max = CreateConVar("hm_speed_tank_max", "315", "", FCVAR_PLUGIN);

	RegAdminCmd("sm_autodifficulty_init", Command_AutoDifficultyInit, ADMFLAG_CONFIG, "");
	RegAdminCmd("sm_autodifficulty_refresh", Command_AutoDifficultyRefresh, ADMFLAG_CONFIG, "");
	RegAdminCmd("sm_check", Command_Check, ADMFLAG_CONFIG, "");
	RegAdminCmd("sm_spawn_limits", Command_SpawnLimits, ADMFLAG_CONFIG, "");
#if COOP_STATS_ENABLED
	RegConsoleCmd("sm_ranksum", Command_RankSum);
#endif
}

public Action:Command_AutoDifficultyInit(client, args)
{
	Log("hardmod.sp / Command_AutoDifficultyInit()", 3);
	AutoDifficultyInit();
}

public Action:Command_AutoDifficultyRefresh(client, args)
{
	Log("hardmod.sp / Command_AutoDifficultyRefresh()", 3);
	Autodifficulty();
}

public Action:Command_Check(client, args)
{
	PrintToServer("hm_autohp_charger_min = %d, hm_autohp_charger_max = %d, sv_maxplayers = %d", GetConVarInt(hm_autohp_charger_min), GetConVarInt(hm_autohp_charger_max), cvar_maxplayers);
	for (new i = L4D_MINPLAYERS; i <= L4D_MAXPLAYERS; i++)
	{
		PrintToServer("AutodifficultyItems[%d][ZC_SMOKER] = %d | AutodifficultyHP[%d][ZC_CHARGER] = %d", i, AutodifficultyItems[i][ZC_SMOKER], i, AutodifficultyHP[i][ZC_CHARGER]);
	}
}

public Action:Command_SpawnLimits(client, args)
{
	if (client)
	{
		PrintToChat(client, "z_common_limit = %d", GetConVarInt(FindConVar("z_common_limit")));
		PrintToChat(client, "z_hunter_limit = %d", GetConVarInt(FindConVar("z_hunter_limit")));
		PrintToChat(client, "z_smoker_limit = %d", GetConVarInt(FindConVar("z_smoker_limit")));
		PrintToChat(client, "z_boomer_limit = %d", GetConVarInt(FindConVar("z_boomer_limit")));
		PrintToChat(client, "z_spitter_limit = %d", GetConVarInt(FindConVar("z_spitter_limit")));
		PrintToChat(client, "z_jockey_limit = %d", GetConVarInt(FindConVar("z_jockey_limit")));
		PrintToChat(client, "z_charger_limit = %d", GetConVarInt(FindConVar("z_charger_limit")));
	}
	else
	{
		PrintToServer("z_common_limit = %d", GetConVarInt(FindConVar("z_common_limit")));
		PrintToServer("z_hunter_limit = %d (spawned %d)", GetConVarInt(FindConVar("z_hunter_limit")), CountMonsters(ZC_HUNTER));
		PrintToServer("z_smoker_limit = %d (spawned %d)", GetConVarInt(FindConVar("z_smoker_limit")), CountMonsters(ZC_SMOKER));
		PrintToServer("z_boomer_limit = %d (spawned %d)", GetConVarInt(FindConVar("z_boomer_limit")), CountMonsters(ZC_BOOMER));
		PrintToServer("z_spitter_limit = %d (spawned %d)", GetConVarInt(FindConVar("z_spitter_limit")), CountMonsters(ZC_SPITTER));
		PrintToServer("z_jockey_limit = %d (spawned %d)", GetConVarInt(FindConVar("z_jockey_limit")), CountMonsters(ZC_JOCKEY));
		PrintToServer("z_charger_limit = %d (spawned %d)", GetConVarInt(FindConVar("z_charger_limit")), CountMonsters(ZC_CHARGER));
	}
}

stock CountMonsters(ZOMBIE_CLASS)
{
	new count = 0;
	for (new i = 1; i <= L4D_MAXPLAYERS; i++) if (GetClientZC(i) == ZOMBIE_CLASS) count++;
	return count;
}

public z_difficulty_changed(Handle:hVariable, const String:strOldValue[], const String:strNewValue[])
{
	UpdateDifficultyName();
}

public hm_ad_options_changed(Handle:hVariable, const String:strOldValue[], const String:strNewValue[])
{
	Log("coop_autodifficulty.inc / hm_ad_options_changed()", 5);
	if (IsServerShuttingDown) return;
	AutoDifficultyInit();
}

UpdateDifficultyName()
{
	Log("coop_autodifficulty.inc / UpdateDifficultyName()", 5);
	if (IsServerShuttingDown) return;
	GetConVarString(z_difficulty, sGameDifficulty, sizeof(sGameDifficulty));
	if (ReplaceString(sGameDifficulty, sizeof(sGameDifficulty), "Impossible", "Expert", false)) cvar_difficulty = 4;
	else if (ReplaceString(sGameDifficulty, sizeof(sGameDifficulty), "Hard", "Master", false)) cvar_difficulty = 3;
}

AutoDifficultyInit()
{
	UpdateDifficultyName();
	if (!cvar_maxplayers) cvar_maxplayers = GetConVarInt(FindConVar("sv_maxplayers"));
	if (cvar_maxplayers < 1) return;
	if (FindConVar("l4d2_loot_h_drop_items") == INVALID_HANDLE) l4d2_plugin_loot = false;
	else l4d2_plugin_loot = true;	
	if (FindConVar("monsterbots_interval") == INVALID_HANDLE) l4d2_plugin_monsterbots = false;
	else l4d2_plugin_monsterbots = true;
	if (FindConVar("l4d2_charger_steering_allow") != INVALID_HANDLE && GetConVarInt(FindConVar("l4d2_charger_steering_allow")) > 0 && GetConVarFloat(hm_autohp_supercharger_auto) > 0) extra_charger = true;
	else extra_charger = false;
	for (new i = L4D_MINPLAYERS; i <= L4D_MAXPLAYERS; i++)
	{
		AutodifficultyHP[i][ZC_ZOMBIE] = GetLineFunction(GetConVarInt(hm_autohp_zombie_min), GetConVarInt(hm_autohp_zombie_max), i);
		AutodifficultyHP[i][ZC_SMOKER] = GetLineFunction(GetConVarInt(hm_autohp_smoker_min), GetConVarInt(hm_autohp_smoker_max), i);
		AutodifficultyHP[i][ZC_BOOMER] = GetLineFunction(GetConVarInt(hm_autohp_boomer_min), GetConVarInt(hm_autohp_boomer_max), i);
		AutodifficultyHP[i][ZC_HUNTER] = GetLineFunction(GetConVarInt(hm_autohp_hunter_min), GetConVarInt(hm_autohp_hunter_max), i);
		AutodifficultyHP[i][ZC_SPITTER] = GetLineFunction(GetConVarInt(hm_autohp_spitter_min), GetConVarInt(hm_autohp_spitter_max), i);
		AutodifficultyHP[i][ZC_JOCKEY] = GetLineFunction(GetConVarInt(hm_autohp_jockey_min), GetConVarInt(hm_autohp_jockey_max), i);
		AutodifficultyHP[i][ZC_CHARGER] = GetLineFunction(GetConVarInt(hm_autohp_charger_min), GetConVarInt(hm_autohp_charger_max), i);
		AutodifficultyHP[i][ZC_WITCH] = GetLineFunction(GetConVarInt(hm_autohp_witch_min), GetConVarInt(hm_autohp_witch_max), i);
		AutodifficultyHP[i][ZC_TANK] = RoundToNearest(GetLineFunction(GetConVarInt(hm_autohp_tank_min), GetConVarInt(hm_autohp_tank_max), i) / 2.0);
		if (l4d2_plugin_loot)
		{
			AutodifficultyItems[i][ZC_SMOKER] = GetLineFunction(GetConVarInt(hm_items_smoker_min), GetConVarInt(hm_items_smoker_max), i);
			AutodifficultyItems[i][ZC_BOOMER] = GetLineFunction(GetConVarInt(hm_items_boomer_min), GetConVarInt(hm_items_boomer_max), i);
			AutodifficultyItems[i][ZC_HUNTER] = GetLineFunction(GetConVarInt(hm_items_hunter_min), GetConVarInt(hm_items_hunter_max), i);
			AutodifficultyItems[i][ZC_SPITTER] = GetLineFunction(GetConVarInt(hm_items_spitter_min), GetConVarInt(hm_items_spitter_max), i);
			AutodifficultyItems[i][ZC_JOCKEY] = GetLineFunction(GetConVarInt(hm_items_jockey_min), GetConVarInt(hm_items_jockey_max), i);
			AutodifficultyItems[i][ZC_CHARGER] = GetLineFunction(GetConVarInt(hm_items_charger_min), GetConVarInt(hm_items_charger_max), i);
			AutodifficultyItems[i][ZC_TANK] = GetLineFunction(GetConVarInt(hm_items_tank_min), GetConVarInt(hm_items_tank_max), i);
		}
		AutodifficultySpawnLimit[i][ZC_ZOMBIE] = GetLineFunction(GetConVarInt(hm_spawn_zombie_min), GetConVarInt(hm_spawn_zombie_max), i);
		AutodifficultySpawnLimit[i][ZC_SMOKER] = GetLineFunction(GetConVarInt(hm_spawn_smoker_min), GetConVarInt(hm_spawn_smoker_max), i);
		AutodifficultySpawnLimit[i][ZC_BOOMER] = GetLineFunction(GetConVarInt(hm_spawn_boomer_min), GetConVarInt(hm_spawn_boomer_max), i);
		AutodifficultySpawnLimit[i][ZC_HUNTER] = GetLineFunction(GetConVarInt(hm_spawn_hunter_min), GetConVarInt(hm_spawn_hunter_max), i);
		AutodifficultySpawnLimit[i][ZC_SPITTER] = GetLineFunction(GetConVarInt(hm_spawn_spitter_min), GetConVarInt(hm_spawn_spitter_max), i);
		AutodifficultySpawnLimit[i][ZC_JOCKEY] = GetLineFunction(GetConVarInt(hm_spawn_jockey_min), GetConVarInt(hm_spawn_jockey_max), i);
		AutodifficultySpawnLimit[i][ZC_CHARGER] = GetLineFunction(GetConVarInt(hm_spawn_charger_min), GetConVarInt(hm_spawn_charger_max), i);

		AutodifficultySpeed[i][ZC_SMOKER] = GetLineFunction(GetConVarInt(hm_speed_smoker_min), GetConVarInt(hm_speed_smoker_max), i);
		AutodifficultySpeed[i][ZC_BOOMER] = GetLineFunction(GetConVarInt(hm_speed_boomer_min), GetConVarInt(hm_speed_boomer_max), i);
		AutodifficultySpeed[i][ZC_HUNTER] = GetLineFunction(GetConVarInt(hm_speed_hunter_min), GetConVarInt(hm_speed_hunter_max), i);
		AutodifficultySpeed[i][ZC_SPITTER] = GetLineFunction(GetConVarInt(hm_speed_spitter_min), GetConVarInt(hm_speed_spitter_max), i);
		AutodifficultySpeed[i][ZC_JOCKEY] = GetLineFunction(GetConVarInt(hm_speed_jockey_min), GetConVarInt(hm_speed_jockey_max), i);
		AutodifficultySpeed[i][ZC_CHARGER] = GetLineFunction(GetConVarInt(hm_speed_charger_min), GetConVarInt(hm_speed_charger_max), i);
		AutodifficultySpeed[i][ZC_TANK] = GetLineFunction(GetConVarInt(hm_speed_tank_min), GetConVarInt(hm_speed_tank_max), i);

		AutodifficultySpawnInterval[i] = GetLineFunction(GetConVarInt(hm_spawn_interval_max), GetConVarInt(hm_spawn_interval_min), i);
		AutodifficultySpawnCount[i] = GetLineFunction(GetConVarInt(hm_special_infected_min), GetConVarInt(hm_special_infected_max), i);
		AutodifficultyTongueRange[i] = GetLineFunction(GetConVarInt(hm_auto_tongue_range_min), GetConVarInt(hm_auto_tongue_range_max), i);
		AutodifficultyTongueChokeDmg[i] = GetLineFunction(GetConVarInt(hm_auto_tongue_choke_dmg_min), GetConVarInt(hm_auto_tongue_choke_dmg_max), i);
		AutodifficultyTongueDragDmg[i] = GetLineFunction(GetConVarInt(hm_auto_tongue_drag_dmg_min), GetConVarInt(hm_auto_tongue_drag_dmg_max), i);
		AutodifficultySmokerClawDmg[i] = GetLineFunction(GetConVarInt(hm_auto_smoker_pz_claw_dmg_min), GetConVarInt(hm_auto_smoker_pz_claw_dmg_max), i);
		AutodifficultyJockeyClawDmg[i] = GetLineFunction(GetConVarInt(hm_auto_jockey_pz_claw_dmg_min), GetConVarInt(hm_auto_jockey_pz_claw_dmg_max), i);
		AutodifficultyGrenadeLRDmg[i] = GetLineFunction(GetConVarInt(hm_auto_grenade_lr_dmg_min), GetConVarInt(hm_auto_grenade_lr_dmg_max), i);
		AutodifficultyTankBurnTime[i] = GetLineFunction(GetConVarInt(hm_tank_burn_duration_min), GetConVarInt(hm_tank_burn_duration_max), i);
	}
	autodifficulty_calculated = true;
}

stock GetLineFunction(GLF_Min, GLF_Max, i)
{
	new result = GetLineFunctionEx(GLF_Min, GLF_Max, i, cvar_maxplayers);
	if (result < 0) return GLF_Min;
	return result;
}

// Нахождение уровнения прямой по двум точкам : http://www.reshalki.ru/index.php/reshalki/77
stock GetLineFunctionEx(GLF_Min, GLF_Max, i, GLF_maxplayers)
{
	new Float:k = (GLF_Max - GLF_Min) * 1.0 / (GLF_maxplayers - L4D_MINPLAYERS) * 1.0;
	new Float:b = GLF_Max * 1.0 - k * GLF_maxplayers;
	return RoundToNearest(k * i + b);
}

stock GetTankHP()
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i) && IsClientInGame(i) && GetClientTeam(i) == TEAM_INFECTED && GetClientZC(i) == ZC_TANK && GetClientHealth(i) > 0)
		{
			if (IsIncapacitated(i))	return 0;
			return GetClientHealth(i);
		}
	}
	return GetConVarInt(FindConVar("z_tank_health")) * 2;
}

public Action:Command_ADInfo(client, args)
{
	if (client == 0) return Plugin_Handled;
	PrintToChat(client, "\x05Informantion:\x03");

	if (RDifficultyMultiplier >= 1000.0)
	{
		decl String:MapDifficultyMultiplier[8];
		FloatToString(RDifficultyMultiplier, MapDifficultyMultiplier, sizeof(MapDifficultyMultiplier));
		PrintToChat(client, "\x05Difficulty: \x04%s\x05 x \x04%s\x05 | Players: \x04%i", sGameDifficulty, MapDifficultyMultiplier, GetRealClientCount(true));
	}
	else if (RDifficultyMultiplier >= 100.0)
	{
		decl String:MapDifficultyMultiplier[7];
		FloatToString(RDifficultyMultiplier, MapDifficultyMultiplier, sizeof(MapDifficultyMultiplier));
		PrintToChat(client, "\x05Difficulty: \x04%s\x05 x \x04%s\x05 | Players: \x04%i", sGameDifficulty, MapDifficultyMultiplier, GetRealClientCount(true));
	}
	else if (RDifficultyMultiplier >= 10.0)
	{
		decl String:MapDifficultyMultiplier[6];
		FloatToString(RDifficultyMultiplier, MapDifficultyMultiplier, sizeof(MapDifficultyMultiplier));
		PrintToChat(client, "\x05Difficulty: \x04%s\x05 x \x04%s\x05 | Players: \x04%i", sGameDifficulty, MapDifficultyMultiplier, GetRealClientCount(true));
	}
	else
	{
		decl String:MapDifficultyMultiplier[5];
		FloatToString(RDifficultyMultiplier, MapDifficultyMultiplier, sizeof(MapDifficultyMultiplier));
		PrintToChat(client, "\x05Difficulty: \x04%s\x05 x \x04%s\x05 | Players: \x04%i", sGameDifficulty, MapDifficultyMultiplier, GetRealClientCount(true));
	}
	if (IsTankAlive()) PrintToChat(client, "\x05Tank HP: \x03%i\x05 | Witch HP: \x04%i\x05 | Zombie HP: \x04%i\x03", GetTankHP(), GetConVarInt(FindConVar("z_witch_health")), GetConVarInt(FindConVar("z_health")));
	else PrintToChat(client, "\x05Tank HP: \x04%i\x05 | Witch HP: \x04%i\x05 | Zombie HP: \x04%i\x03", GetTankHP(), GetConVarInt(FindConVar("z_witch_health")), GetConVarInt(FindConVar("z_health")));
	PrintToChat(client, "\x05Hunter HP: \x04%i\x05 | Smoker HP: \x04%i\x05 | Boomer HP: \x04%i\x05 \nCharger HP: \x04%i\x05 | Spitter HP: \x04%i\x05 | Jockey HP: \x04%i\x03", GetConVarInt(FindConVar("z_hunter_health")), GetConVarInt(FindConVar("z_gas_health")), GetConVarInt(FindConVar("z_exploding_health")), GetConVarInt(FindConVar("z_charger_health")), GetConVarInt(FindConVar("z_spitter_health")), GetConVarInt(FindConVar("z_jockey_health")));
	PrintToChat(client, "\x05Grenade Launcher Damage: \x04%d", GetConVarInt(FindConVar("grenadelauncher_damage")));
	return Plugin_Continue;
}

#if COOP_STATS_ENABLED
	public Action:Command_RankSum(client, args)
	{
		new Float:RankMod = Calculate_Rank_Mod();
		if (client == 0)
		{
			PrintToServer("Rank Sum: %f", rank_sum);
			PrintToServer("Rank Mod: %f", RankMod);
		}
		else
		{
			PrintToChat(client, "\x05Rank Sum: \x04%f", rank_sum);
			PrintToChat(client, "\x05Rank Mod: \x04%f", RankMod);
		}
	}
#endif

stock GetGameMode()
{
	new String:GameMode[13];
	new Handle:gamecvar_mp_gamemode = FindConVar("mp_gamemode");
	GetConVarString(gamecvar_mp_gamemode, GameMode, sizeof(GameMode));
	if (StrEqual(GameMode, "coop", false) == true) return 1;
	else if (StrEqual(GameMode, "realism", false) == true) return 2;
	else if (StrEqual(GameMode, "survival", false) == true) return 3;
	else if (StrEqual(GameMode, "versus", false) == true) return 4;
	else if (StrEqual(GameMode, "teamversus", false) == true) return 5;
	else if (StrEqual(GameMode, "scavenge", false) == true) return 6;
	else if (StrEqual(GameMode, "teamscavenge", false) == true) return 7;
	else if (StrEqual(GameMode, "mutation3", false) == true) return 8;
	else if (StrEqual(GameMode, "mutation12", false) == true) return 9;
	return 0;
}

public Autodifficulty()
{
	if (GetConVarInt(hm_autodifficulty) < 1) return;
	if (!autodifficulty_calculated)
	{
		AutoDifficultyInit();
		return;
	}
	if (playerscount < L4D_MINPLAYERS) playerscount = L4D_MINPLAYERS;
	if (l4d2_plugin_monsterbots)
	{
		SetConVarInt(FindConVar("monsterbots_maxbots"), RoundToNearest(AutodifficultySpawnCount[playerscount] * GetConVarFloat(hm_spawn_count_mod)), false, false);
		SetConVarInt(FindConVar("monsterbots_interval"), RoundToNearest(AutodifficultySpawnInterval[playerscount] * GetConVarFloat(hm_spawn_time_mod)), false, false);
	}
	else
	{
		SetConVarInt(z_special_spawn_interval, RoundToNearest(AutodifficultySpawnInterval[playerscount] * GetConVarFloat(hm_spawn_time_mod)), false, false);
		SetConVarInt(special_respawn_interval, RoundToNearest(AutodifficultySpawnInterval[playerscount] * GetConVarFloat(hm_spawn_time_mod)), false, false);
	}
	if (GetGameMode() == 8) BonusDrop = 2;
	else BonusDrop = 0;

#if COOP_STATS_ENABLED
	RDifficultyMultiplier = Calculate_Rank_Mod();
#else
	RDifficultyMultiplier = GetConVarFloat(PDifficultyMultiplier);
#endif

	if (GetConVarInt(hm_speed_automod) > 0)
	{
		SetConVarInt(FindConVar("z_hunter_speed"), AutodifficultySpeed[playerscount][ZC_HUNTER], false, false);
		SetConVarInt(FindConVar("z_gas_speed"), AutodifficultySpeed[playerscount][ZC_SMOKER], false, false);
		SetConVarInt(FindConVar("z_exploding_speed"), AutodifficultySpeed[playerscount][ZC_BOOMER], false, false);
		SetConVarInt(FindConVar("z_spitter_speed"), AutodifficultySpeed[playerscount][ZC_SPITTER], false, false);
		SetConVarInt(FindConVar("z_jockey_speed"), AutodifficultySpeed[playerscount][ZC_JOCKEY], false, false);
		SetConVarInt(FindConVar("z_charge_start_speed"), AutodifficultySpeed[playerscount][ZC_CHARGER], false, false);
		SetConVarInt(FindConVar("z_tank_speed"), AutodifficultySpeed[playerscount][ZC_TANK], false, false);
	}
	new Handle:tank_burn_duration = INVALID_HANDLE;
	switch (cvar_difficulty)
	{
		case 1: tank_burn_duration = FindConVar("tank_burn_duration");
		case 3: tank_burn_duration = FindConVar("tank_burn_duration_hard");
		case 4: tank_burn_duration = FindConVar("tank_burn_duration_expert");
	}
	SetConVarInt(tank_burn_duration, AutodifficultyTankBurnTime[playerscount], false, false);
	SetConVarInt(FindConVar("grenadelauncher_damage"), RoundToNearest(AutodifficultyGrenadeLRDmg[playerscount] * RDifficultyMultiplier), false, false);
	if (GetConVarInt(hm_spawn_automod) > 0)
	{
		SetConVarInt(FindConVar("z_common_limit"), AutodifficultySpawnLimit[playerscount][ZC_ZOMBIE], false, false);
		SetConVarInt(FindConVar("z_hunter_limit"), AutodifficultySpawnLimit[playerscount][ZC_HUNTER], false, false);
		SetConVarInt(FindConVar("z_smoker_limit"), AutodifficultySpawnLimit[playerscount][ZC_SMOKER], false, false);
		SetConVarInt(FindConVar("z_boomer_limit"), AutodifficultySpawnLimit[playerscount][ZC_BOOMER], false, false);
		SetConVarInt(FindConVar("z_spitter_limit"), AutodifficultySpawnLimit[playerscount][ZC_SPITTER], false, false);
		SetConVarInt(FindConVar("z_jockey_limit"), AutodifficultySpawnLimit[playerscount][ZC_JOCKEY], false, false);
		SetConVarInt(FindConVar("z_charger_limit"), AutodifficultySpawnLimit[playerscount][ZC_CHARGER], false, false);
	}
	new Float:HealthMod = GetConVarFloat(hm_infected_hp_mod);
	if (GetConVarInt(hm_autohp_automod) > 0) HealthMod *= RDifficultyMultiplier;

	SetConVarInt(FindConVar("z_hunter_health"), RoundToNearest(AutodifficultyHP[playerscount][ZC_HUNTER] * HealthMod), false, false);
	SetConVarInt(FindConVar("z_gas_health"), RoundToNearest(AutodifficultyHP[playerscount][ZC_SMOKER] * HealthMod), false, false);
	SetConVarInt(FindConVar("z_exploding_health"), RoundToNearest(AutodifficultyHP[playerscount][ZC_BOOMER] * HealthMod), false, false);
	SetConVarInt(FindConVar("z_spitter_health"), RoundToNearest(AutodifficultyHP[playerscount][ZC_SPITTER] * HealthMod), false, false);
	SetConVarInt(FindConVar("z_jockey_health"), RoundToNearest(AutodifficultyHP[playerscount][ZC_JOCKEY] * HealthMod), false, false);
	SetConVarInt(FindConVar("z_witch_health"), RoundToNearest(AutodifficultyHP[playerscount][ZC_WITCH] * HealthMod), false, false);
	SetConVarInt(FindConVar("z_tank_health"), RoundToNearest(AutodifficultyHP[playerscount][ZC_TANK] * HealthMod * GetConVarFloat(hm_tank_hp_mod)), false, false);
	SetConVarInt(FindConVar("z_health"), RoundToNearest(AutodifficultyHP[playerscount][ZC_ZOMBIE] * HealthMod), false, false);

#if DAMAGEMOD_ENABLED
	if (GetConVarFloat(hm_autohp_supercharger_auto) > 0) if (GetConVarInt(hm_damage_supercharger) == 1 && GetRealClientCount() >= 8 || GetConVarInt(hm_damage_supercharger) > 1) HealthMod *= GetConVarFloat(hm_autohp_supercharger_auto);
#endif

	SetConVarInt(FindConVar("z_charger_health"), RoundToNearest(AutodifficultyHP[playerscount][ZC_CHARGER] * HealthMod), false, false);

	if (l4d2_plugin_loot && GetConVarInt(hm_items_automod) > 0)
	{
		new Float:LootMod = GetConVarFloat(hm_loot_mod);
//		if (GetConVarInt(hm_items_automod) > 0) LootMod *= SquareRoot(ZDifficultyMultiplier);
		SetConVarInt(FindConVar("l4d2_loot_h_drop_items"), RoundToNearest((AutodifficultyItems[playerscount][ZC_HUNTER] + BonusDrop) * LootMod), false, false);
		SetConVarInt(FindConVar("l4d2_loot_b_drop_items"), RoundToNearest((AutodifficultyItems[playerscount][ZC_BOOMER] + BonusDrop) * LootMod), false, false);
		SetConVarInt(FindConVar("l4d2_loot_s_drop_items"), RoundToNearest((AutodifficultyItems[playerscount][ZC_SMOKER] + BonusDrop) * LootMod), false, false);
		SetConVarInt(FindConVar("l4d2_loot_sp_drop_items"), RoundToNearest((AutodifficultyItems[playerscount][ZC_SPITTER] + BonusDrop) * LootMod), false, false);
		SetConVarInt(FindConVar("l4d2_loot_j_drop_items"), RoundToNearest((AutodifficultyItems[playerscount][ZC_JOCKEY] + BonusDrop) * LootMod), false, false);
		SetConVarInt(FindConVar("l4d2_loot_t_drop_items"), RoundToNearest((AutodifficultyItems[playerscount][ZC_TANK] + BonusDrop) * LootMod), false, false);

		if (extra_charger) SetConVarInt(FindConVar("l4d2_loot_c_drop_items"), RoundToNearest((AutodifficultyItems[playerscount][ZC_CHARGER] + BonusDrop + GetConVarInt(hm_items_supercharger_auto) * LootMod)), false, false);
		else SetConVarInt(FindConVar("l4d2_loot_c_drop_items"), RoundToNearest((AutodifficultyItems[playerscount][ZC_CHARGER] + BonusDrop) * LootMod), false, false);
	}

	SetConVarInt(FindConVar("tongue_range"), AutodifficultyTongueRange[playerscount], false, false);
	SetConVarInt(FindConVar("smoker_pz_claw_dmg"), AutodifficultySmokerClawDmg[playerscount], false, false);
	SetConVarInt(FindConVar("jockey_pz_claw_dmg"), AutodifficultyJockeyClawDmg[playerscount], false, false);
	SetConVarInt(FindConVar("tongue_choke_damage_amount"), AutodifficultyTongueChokeDmg[playerscount], false, false);
	SetConVarInt(FindConVar("tongue_drag_damage_amount"), AutodifficultyTongueDragDmg[playerscount], false, false);
}

#if COOP_STATS_ENABLED
	Float:Calculate_Rank_Mod()
	{
		new Float:local_result = 1.0;
		switch (GetConVarInt(hm_rankmod_mode))
		{
			case 0, 1, 2:
			{
				if (Total_Players < cvar_maxplayers) return GetConVarFloat(PDifficultyMultiplier);
				new Float:sum_low = 0.0;
				new Float:sum_high = 0.0;
				// PrintToServer("\nCalculate_Rank_Mod():\n\nTotal_Players = %d, MaxPlayers = %d, hm_rankmod_logarithm = %f", Total_Players, cvar_maxplayers, GetConVarFloat(hm_rankmod_logarithm));
				for (new i = 1; i <= cvar_maxplayers; i++)
				{
					sum_low += Sum_Function(i * 1.0);
					sum_high += Sum_Function(Total_Players * 1.0 + 1.0 - i * 1.0);
				}
				// PrintToServer("sum_low = %f | sum_high = %f | cvar_maxplayers = %d", sum_low, sum_high, cvar_maxplayers);
				sum_low *= 1.0 / cvar_maxplayers * 1.0;
				sum_high *= 1.0 / cvar_maxplayers * 1.0;
				new Float:sum_current = 0.0;
				new Float:current_player_rank = 0.0;
				new Float:current_players_count = 0.0;
				for (new i = 1; i <= L4D_MAXPLAYERS; i++) if (IsClientConnected(i) && IsClientInGame(i) && !IsFakeClient(i) && GetClientTeam(i) == TEAM_SURVIVORS)
				{
					current_players_count++;
					current_player_rank = ClientRank[i] * 1.0;
					if (current_player_rank < 1.0) current_player_rank = Total_Players * 0.5; // игрок еще не попал в статистику...
					sum_current += Sum_Function(Total_Players + 1.0 - current_player_rank);
				}
				if (current_players_count < 1) return local_result;
				// PrintToServer("sum_current = %f", sum_current);
				sum_current *= 1.0 / current_players_count * 1.0;
				PrintToServer("sum_low = %f | sum_high = %f | sum_current = %f", sum_low, sum_high, sum_current);
				new Float:k = (GetConVarFloat(hm_rankmod_max) - GetConVarFloat(hm_rankmod_min)) / (sum_high - sum_low); // угловой коэффициент прямой
				new Float:p = GetConVarFloat(hm_rankmod_max) - k * sum_high;
				local_result = k * sum_current + p;
				// PrintToServer("result = %f", local_result);
				if (local_result < GetConVarFloat(hm_rankmod_min))
				{
					// PrintToServer("[HARDMOD] Calculate_Rank_Mod() / local_result < GetConVarFloat(hm_rankmod_min) / %f < %f / corrected", local_result, GetConVarFloat(hm_rankmod_min));
					local_result = GetConVarFloat(hm_rankmod_min);
				}
				else if (local_result > GetConVarFloat(hm_rankmod_max))
				{
					// PrintToServer("[HARDMOD] Calculate_Rank_Mod() / local_result > GetConVarFloat(hm_rankmod_max) / %f > %f / corrected", local_result, GetConVarFloat(hm_rankmod_max));
					local_result = GetConVarFloat(hm_rankmod_max);
				}
				if (GetConVarInt(hm_rankmod_mode) == 1) local_result += GetConVarFloat(PDifficultyMultiplier);
				if (GetConVarInt(hm_rankmod_mode) == 2) local_result *= GetConVarFloat(PDifficultyMultiplier);
				return local_result;


// sum_low = 13.320619 | sum_high = 42.094573 | cvar_maxplayers = 16
// sum_current = 2.492760
// sum_low = 0.832538 | sum_high = 2.630910 | sum_current = 2.492760
// result = 1.223180
			
			}
			case 3, 4, 5:
			{
				if (Total_Players < 250) return GetConVarFloat(PDifficultyMultiplier);
				rank_sum = 0.0;
				new players_count = 0;
				for (new i = 1; i <= L4D_MAXPLAYERS; i++) if (IsClientConnected(i) && IsClientInGame(i) && !IsFakeClient(i) && GetClientTeam(i) == TEAM_SURVIVORS)
				{
					if ClientRank[i] == 0 *then rank_sum += 0.0;
					else if ClientRank[i] <= 3 *then rank_sum += 5.0;
					else if ClientRank[i] <= 7 *then rank_sum += 4.4;
					else if ClientRank[i] <= 15 *then rank_sum += 3.8;
					else if ClientRank[i] <= 30 *then rank_sum += 3.2;
					else if ClientRank[i] <= 60 *then rank_sum += 2.6;
					else if ClientRank[i] <= 120 *then rank_sum += 2.0;
					else if ClientRank[i] <= 240 *then rank_sum += 1.4;
					else if ClientRank[i] <= 360 *then rank_sum += 0.8;
					else if ClientRank[i] <= 480 *then rank_sum += 0.2;
					players_count++;
				}
				if (players_count < 1) players_count = 1;
				local_result = ((rank_sum * 1.0 - (players_count * 1.5)) / (players_count * 1.0)) / 6.0 + 0.75;
				if (local_result < GetConVarFloat(hm_rankmod_min)) local_result = GetConVarFloat(hm_rankmod_min);
				else if (local_result > GetConVarFloat(hm_rankmod_max)) local_result = GetConVarFloat(hm_rankmod_max);
				if (GetConVarInt(hm_rankmod_mode) == 4) local_result += GetConVarFloat(PDifficultyMultiplier);
				if (GetConVarInt(hm_rankmod_mode) == 5) local_result *= GetConVarFloat(PDifficultyMultiplier);
				return local_result;
			}
		}
		return GetConVarFloat(PDifficultyMultiplier);
	}
	Float:Sum_Function(const Float:input_value)
	{
		if (input_value == 0.0) return 0.0;
		new Float:cvar_rankmod_logarithm = GetConVarFloat(hm_rankmod_logarithm);
		if (cvar_rankmod_logarithm >= 1.0) return Logarithm(input_value, cvar_rankmod_logarithm);
		if (cvar_rankmod_logarithm >= 0.0 && cvar_rankmod_logarithm < 1.0) return input_value * cvar_rankmod_logarithm;
		if (cvar_rankmod_logarithm == 0.0) return input_value; // простая линейная зависимость
		if (cvar_rankmod_logarithm == -1.0)
		{
			new Float:x = Logarithm(input_value, 10.0);
			return x * x;
		}
		if (cvar_rankmod_logarithm == -2.0) return (input_value * input_value / ((input_value + Total_Players * 4) / (25.0 * Total_Players))) / 10.0;
		if (cvar_rankmod_logarithm == -3.0)
		{
			new Float:x = Logarithm(input_value, 10.0);
			return x * x / (0.001 * x + 1.11);
		}
		return input_value;
	}
#endif

ADPlayerSpawn(Handle:event)
{
	if (GetConVarInt(hm_autodifficulty_forcehp) < 1) return;
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	if (IsValidClient(client) && GetClientTeam(client) == TEAM_INFECTED) switch(GetClientZC(client))
	{
		case ZC_SMOKER: SetEntityHealth(client, GetConVarInt(FindConVar("z_gas_health")));
		case ZC_BOOMER: SetEntityHealth(client, GetConVarInt(FindConVar("z_exploding_health")));
		case ZC_HUNTER: SetEntityHealth(client, GetConVarInt(FindConVar("z_hunter_health")));
		case ZC_SPITTER: SetEntityHealth(client, GetConVarInt(FindConVar("z_spitter_health")));
		case ZC_JOCKEY: SetEntityHealth(client, GetConVarInt(FindConVar("z_jockey_health")));
		case ZC_CHARGER: SetEntityHealth(client, GetConVarInt(FindConVar("z_charger_health")));
		case ZC_TANK: SetEntityHealth(client, GetConVarInt(FindConVar("z_tank_health")) * 2);
	}
}

ADOnMapStart()
{
	AutoDifficultyInit();
}

ADRoundStart()
{
//	CreateTimer(10.0, TimedAutoDifficultyInit);
}

ADPlayerTeam()
{
	new count = 0;
	for (new i = 1; i <= L4D_MAXPLAYERS; i++) if (IsClientConnected(i) && IsClientInGame(i) && GetClientTeam(i) == TEAM_SURVIVORS) count++;
	if (playerscount != count)
	{
		playerscount = count;
		Autodifficulty();
	}
}

CADOnClientDisconnect()
{
	ADPlayerTeam();
}

public Action:TimedAutoDifficultyInit(Handle:timer, any:client)
{
	AutoDifficultyInit();
	Autodifficulty();
}