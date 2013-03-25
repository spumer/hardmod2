new Float:RDifficultyMultiplier = 1.0;
new Float:XDifficultyMultiplier = 2.89;
new Float:LootNoDrop = 15.0;
new BonusDrop = 0;
new playerscount = 0;
new cvar_special_infected_min = 4;
new cvar_special_infected_max = 18;
new cvar_spawn_interval_min = 3;
new cvar_spawn_interval_max = 10;
#if COOP_STATS_ENABLED
	new Float:rank_sum = 0.0;
#endif
new Handle:PDifficultyMultiplier;
new bool:l4d2_plugin_loot = true;
new bool:l4d2_plugin_monsterbots = true;

new Handle:hm_autodifficulty;
new Handle:hm_autodifficulty_forcehp;
#if COOP_STATS_ENABLED
	new Handle:hm_rankmod_mode;
#endif
new Handle:z_difficulty;
new Handle:z_common_limit;
new Handle:z_special_spawn_interval;
new Handle:special_respawn_interval;
new Handle:z_hunter_health;
new Handle:z_smoker_health;
new Handle:z_boomer_health;
new Handle:z_spitter_health;
new Handle:z_jockey_health;
new Handle:z_witch_health;
new Handle:z_health;
new Handle:z_hunter_limit;
new Handle:z_smoker_limit;
new Handle:z_boomer_limit;
new Handle:z_spitter_limit;
new Handle:z_jockey_limit;
new Handle:z_spitter_max_wait_time;
new Handle:z_vomit_interval;

new Handle:z_smoker_speed;
new Handle:z_boomer_speed;
new Handle:z_spitter_speed;

new Handle:jockey_pz_claw_dmg;
new Handle:smoker_pz_claw_dmg;
new Handle:tongue_choke_damage_amount;
new Handle:tongue_drag_damage_amount;
new Handle:tongue_miss_delay;
new Handle:tongue_hit_delay;
new Handle:tongue_range;

new Handle:grenadelauncher_damage;

new Handle:z_spitter_range;
new Handle:z_spit_interval;

new Handle:l4d2_loot_h_drop_items;
new Handle:l4d2_loot_b_drop_items;
new Handle:l4d2_loot_s_drop_items;
new Handle:l4d2_loot_sp_drop_items;
new Handle:l4d2_loot_j_drop_items;
new Handle:hm_loot_mod;
new Handle:hm_tank_hp_mod;
new Handle:hm_infected_hp_mod;
new Handle:hm_spawn_time_mod;

new Handle:hm_special_infected_min;
new Handle:hm_special_infected_max;
new Handle:hm_spawn_interval_min;
new Handle:hm_spawn_interval_max;

new String:sGameDifficulty[16];
new cvar_difficulty = 1;
//new Handle:l4d2_loot_g_chance_nodrop;

public CoopAutoDiffOnPluginStart()
{
	Version += 25;

	hm_autodifficulty = CreateConVar("hm_autodifficulty", "1", "Is the plugin enabled.", FCVAR_PLUGIN);
	hm_autodifficulty_forcehp = CreateConVar("hm_autodifficulty_forcehp", "0", "", FCVAR_PLUGIN);
#if COOP_STATS_ENABLED	
	hm_rankmod_mode = CreateConVar("hm_rankmod_mode", "1", "", FCVAR_PLUGIN);
#endif
	PDifficultyMultiplier = CreateConVar("hm_difficulty_p", "1.0", "", FCVAR_PLUGIN);
	hm_loot_mod = CreateConVar("hm_loot_mod", "1.0", "", FCVAR_PLUGIN);
	hm_tank_hp_mod = CreateConVar("hm_tank_hp_mod", "1.0", "", FCVAR_PLUGIN);
	hm_infected_hp_mod = CreateConVar("hm_infected_hp_mod", "1.0", "", FCVAR_PLUGIN);
	hm_spawn_time_mod = CreateConVar("hm_spawn_time_mod", "1.0", "", FCVAR_PLUGIN);

	z_difficulty = FindConVar("z_difficulty");
	HookConVarChange(z_difficulty, z_difficulty_cnahged);

	z_common_limit = FindConVar("z_common_limit");

	z_special_spawn_interval = FindConVar("z_special_spawn_interval");
	special_respawn_interval = FindConVar("director_special_respawn_interval");
	z_hunter_health = FindConVar("z_hunter_health");
	z_smoker_health = FindConVar("z_gas_health");
	z_boomer_health = FindConVar("z_exploding_health");
	z_spitter_health = FindConVar("z_spitter_health");
	z_jockey_health = FindConVar("z_jockey_health");
	z_witch_health = FindConVar("z_witch_health");
	z_hunter_limit = FindConVar("z_hunter_limit");
	z_smoker_limit = FindConVar("z_smoker_limit");
	z_boomer_limit = FindConVar("z_boomer_limit");
	z_spitter_limit = FindConVar("z_spitter_limit");
	z_jockey_limit = FindConVar("z_jockey_limit");
	z_health = FindConVar("z_health");
	z_spitter_max_wait_time = FindConVar("z_spitter_max_wait_time");
	z_vomit_interval = FindConVar("z_vomit_interval");

	z_smoker_speed = FindConVar("z_gas_speed");
	z_boomer_speed = FindConVar("z_exploding_speed");
	z_spitter_speed = FindConVar("z_spitter_speed");

	grenadelauncher_damage = FindConVar("grenadelauncher_damage");

	jockey_pz_claw_dmg = FindConVar("jockey_pz_claw_dmg");
	smoker_pz_claw_dmg = FindConVar("smoker_pz_claw_dmg");
	tongue_choke_damage_amount = FindConVar("tongue_choke_damage_amount");
	tongue_drag_damage_amount = FindConVar("tongue_drag_damage_amount");
	tongue_miss_delay = FindConVar("tongue_miss_delay");
	tongue_hit_delay = FindConVar("tongue_hit_delay");
	tongue_range = FindConVar("tongue_range");

	z_spitter_range = FindConVar("z_spitter_range");
	z_spit_interval = FindConVar("z_spit_interval");
	
	hm_special_infected_min = CreateConVar("hm_special_infected_min", "4", "", FCVAR_PLUGIN);
	hm_special_infected_max = CreateConVar("hm_special_infected_max", "18", "", FCVAR_PLUGIN);
	hm_spawn_interval_min = CreateConVar("hm_spawn_interval_min", "3", "", FCVAR_PLUGIN);
	hm_spawn_interval_max = CreateConVar("hm_spawn_interval_max", "10", "", FCVAR_PLUGIN);
	HookConVarChange(hm_special_infected_min, hm_special_infected_changed);
	HookConVarChange(hm_special_infected_max, hm_special_infected_changed);
	HookConVarChange(hm_spawn_interval_min, hm_special_infected_changed);
	HookConVarChange(hm_spawn_interval_max, hm_special_infected_changed);

//	RegConsoleCmd("sm_info", Command_Info);
#if COOP_STATS_ENABLED
	RegConsoleCmd("sm_ranksum", Command_RankSum);
#endif
}

public z_difficulty_cnahged(Handle:hVariable, const String:strOldValue[], const String:strNewValue[])
{
	Log("coop_autodifficulty.inc / z_difficulty_cnahged()", 5);
	if (IsServerShuttingDown) return;
	GetConVarString(z_difficulty, sGameDifficulty, sizeof(sGameDifficulty));
	if (ReplaceString(sGameDifficulty, sizeof(sGameDifficulty), "Impossible", "Expert", false)) cvar_difficulty = 4;
	if (ReplaceString(sGameDifficulty, sizeof(sGameDifficulty), "Hard", "Master", false)) cvar_difficulty = 3;
}

public hm_special_infected_changed(Handle:hVariable, const String:strOldValue[], const String:strNewValue[])
{
	Log("coop_autodifficulty.inc / hm_special_infected_changed()", 5);
	if (IsServerShuttingDown) return;
	cvar_special_infected_min = GetConVarInt(hm_special_infected_min);
	cvar_special_infected_max = GetConVarInt(hm_special_infected_max);
	cvar_spawn_interval_min = GetConVarInt(hm_spawn_interval_min);
	cvar_spawn_interval_max = GetConVarInt(hm_spawn_interval_max);
}

stock GetTankHP()
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i) && IsClientInGame(i))
		{
			if (GetClientTeam(i) == TEAM_INFECTED)
			{
				if (GetClientZC(i) == ZC_TANK && GetClientHealth(i) > 0)
				{
					if (IsIncapacitated(i))
					{
						return 0;
					}
					return GetClientHealth(i);
				}
			}
		}
	}
	return GetConVarInt(FindConVar("z_tank_health")) * 2;
}

public Action:Command_ADInfo(client, args)
{
	if (client == 0) return Plugin_Handled;
	new Handle:z_charger_health = FindConVar("z_charger_health");

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

	if (IsTankAlive())
	{
		PrintToChat(client, "\x05Tank HP: \x03%i\x05 | Witch HP: \x04%i\x05 | Zombie HP: \x04%i\x03", GetTankHP(), GetConVarInt(z_witch_health), GetConVarInt(z_health));
	}
	else
	{
		PrintToChat(client, "\x05Tank HP: \x04%i\x05 | Witch HP: \x04%i\x05 | Zombie HP: \x04%i\x03", GetTankHP(), GetConVarInt(z_witch_health), GetConVarInt(z_health));
	}
	PrintToChat(client, "\x05Hunter HP: \x04%i\x05 | Smoker HP: \x04%i\x05 | Boomer HP: \x04%i\x05 \nCharger HP: \x04%i\x05 | Spitter HP: \x04%i\x05 | Jockey HP: \x04%i\x03", GetConVarInt(z_hunter_health), GetConVarInt(z_smoker_health), GetConVarInt(z_boomer_health), GetConVarInt(z_charger_health), GetConVarInt(z_spitter_health), GetConVarInt(z_jockey_health));
	PrintToChat(client, "\x05Grenade Launcher Damage = \x04%d\x05!", GetConVarInt(grenadelauncher_damage));
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

public CADOnClientPostAdminCheck(client)
{
	playerscount = GetRealClientCount(true);
	Autodifficulty();
}

public CADOnClientDisconnect(client)
{
	playerscount = GetRealClientCount(true);
	Autodifficulty();
}

public Action:AutoDifficulty(Handle:timer)
{
	playerscount = GetRealClientCount(true);
	Autodifficulty();
}

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
	if (FindConVar("monsterbots_interval") == INVALID_HANDLE) l4d2_plugin_monsterbots = false;
	else l4d2_plugin_monsterbots = true;
	switch (GetConVarInt(hm_autodifficulty))
	{
		case 2:
		{
			new Handle:monsterbots_maxbots = FindConVar("monsterbots_maxbots");
			if (monsterbots_maxbots != INVALID_HANDLE) SetConVarInt(monsterbots_maxbots, CheckCvarRange(RoundToZero(playerscount / 1.454), 4, 11));
			if (l4d2_plugin_monsterbots)
			{
				new Float:X = -0.033 * playerscount + 0.733;
				if (X < 0.2) X = 0.2;
				if (X > 0.6) X = 0.6;
				SetConVarFloat(hm_spawn_time_mod, X);
			}
		}
		case 3:
		{
			new Handle:monsterbots_maxbots = FindConVar("monsterbots_maxbots");
			if (monsterbots_maxbots != INVALID_HANDLE) SetConVarInt(monsterbots_maxbots, CheckCvarRange(RoundToZero(playerscount / 1.23), 4, 9));
			if (l4d2_plugin_monsterbots)
			{
				new Float:X = (0.143 * playerscount -0.714) / 10;
				if (X < 0.1) X = 0.1;
				if (X > 0.75) X = 0.75;
				SetConVarFloat(hm_spawn_time_mod, X);
			}
		}
		case 4:
		{
			new Handle:monsterbots_maxbots = FindConVar("monsterbots_maxbots");
			if (monsterbots_maxbots != INVALID_HANDLE) SetConVarInt(monsterbots_maxbots, CheckCvarRange(RoundToZero(4 + (playerscount / 1.2)), 5, 15));
			if (l4d2_plugin_monsterbots)
			{
				new Float:X = (0.143 * playerscount -0.714) / 10;
				if (X < 0.1) X = 0.1;
				if (X > 0.5) X = 0.5;
				SetConVarFloat(hm_spawn_time_mod, X);
			}
		}
		case 5:
		{
			new Handle:monsterbots_maxbots = FindConVar("monsterbots_maxbots");
			if (monsterbots_maxbots != INVALID_HANDLE)
			{
				new Float:k = (cvar_special_infected_max - cvar_special_infected_min) / (cvar_maxplayers - 4) * 1.0;
				new Float:b = cvar_special_infected_max - k * cvar_maxplayers;
				SetConVarInt(monsterbots_maxbots, RoundToZero(k * playerscount + b));
				if (l4d2_plugin_monsterbots)
				{
					k = (cvar_spawn_interval_min - cvar_spawn_interval_max) / (cvar_maxplayers - 4) * 1.0;
					b = cvar_spawn_interval_min - k * cvar_maxplayers;
					SetConVarFloat(hm_spawn_time_mod, k * playerscount + b);
				}
			}
		}
	}
	if (FindConVar("l4d2_loot_h_drop_items") == INVALID_HANDLE) l4d2_plugin_loot = false;
	else l4d2_plugin_loot = true;
	if (playerscount < 4) playerscount = 4;
	new common_zombies = 44 - playerscount;
	if (common_zombies > 30) SetConVarInt(z_common_limit, 30, false, false);
	else SetConVarInt(z_common_limit, common_zombies, false, false);
	if (GetGameMode() == 8) BonusDrop = 2;
	else BonusDrop = 0;

#if COOP_STATS_ENABLED
	RDifficultyMultiplier = Calculate_Rank_Mod();
#else
	RDifficultyMultiplier = GetConVarFloat(PDifficultyMultiplier);
#endif

	new ItemsDropCount[6];

	if (l4d2_plugin_loot)
	{
		l4d2_loot_h_drop_items = FindConVar("l4d2_loot_h_drop_items");
		l4d2_loot_b_drop_items = FindConVar("l4d2_loot_b_drop_items");
		l4d2_loot_s_drop_items = FindConVar("l4d2_loot_s_drop_items");
		l4d2_loot_sp_drop_items = FindConVar("l4d2_loot_sp_drop_items");
		l4d2_loot_j_drop_items = FindConVar("l4d2_loot_j_drop_items");
	}

	if (playerscount > 4)
	{ // Ќахождение уровнени€ пр€мой по двум точкам : http://www.reshalki.ru/index.php/reshalki/77

		SetConVarInt(z_spitter_max_wait_time, 34 - playerscount, false, false);
		SetConVarInt(z_vomit_interval, 34 - playerscount, false, false);

		SetConVarInt(z_smoker_speed, 210 + RoundToZero(3.0 * (playerscount - 4) * XDifficultyMultiplier), false, false);
		SetConVarInt(z_boomer_speed, 175 + RoundToZero(3.0 * (playerscount - 4) * XDifficultyMultiplier), false, false);
		SetConVarInt(z_spitter_speed, 160 + RoundToZero(15.0 * playerscount * XDifficultyMultiplier), false, false);

		SetConVarInt(z_hunter_limit, RoundToZero(2.5 + (playerscount / 5)), false, false);
		SetConVarInt(z_smoker_limit, RoundToZero(1.5 + (playerscount / 6)), false, false);
		SetConVarInt(z_boomer_limit, RoundToZero(1.5 + (playerscount / 7)), false, false);
		SetConVarInt(z_spitter_limit, RoundToZero(1.4 + (playerscount / 6)), false, false);
		SetConVarInt(z_jockey_limit, RoundToZero(0.5 + (playerscount / 8)), false, false);

		if (l4d2_plugin_loot)
		{
			ItemsDropCount[0] = CheckCvarRange(RoundToZero((playerscount / 5.3) * SquareRoot(XDifficultyMultiplier)), 1, 100);
			ItemsDropCount[1] = CheckCvarRange(RoundToZero((playerscount / 4.0) * SquareRoot(XDifficultyMultiplier)), 1, 100);
			ItemsDropCount[2] = CheckCvarRange(RoundToZero((playerscount / 4.0) * SquareRoot(XDifficultyMultiplier)), 1, 100);
			ItemsDropCount[4] = CheckCvarRange(RoundToZero((playerscount / 4.6) * SquareRoot(XDifficultyMultiplier)), 1, 100);
			ItemsDropCount[5] = CheckCvarRange(RoundToZero((playerscount / 4.6) * SquareRoot(XDifficultyMultiplier)), 1, 100);
		}

		SetConVarInt(z_hunter_health, RoundToZero((95.833 * playerscount -133.333) * GetConVarFloat(hm_infected_hp_mod)), false, false);
		SetConVarInt(z_smoker_health, RoundToZero((187.5 * playerscount -500.0) * GetConVarFloat(hm_infected_hp_mod)), false, false);
		SetConVarInt(z_boomer_health, RoundToZero((33.333 * playerscount -83.333)* GetConVarFloat(hm_infected_hp_mod)), false, false);
		SetConVarInt(z_spitter_health, RoundToZero((75 * playerscount -200.0) * GetConVarFloat(hm_infected_hp_mod)), false, false);
		SetConVarInt(z_jockey_health, RoundToZero((172.917 * playerscount -366.667) * GetConVarFloat(hm_infected_hp_mod)), false, false);
		
//		SetConVarInt(z_witch_health, CheckCvarMax(RoundToZero(250.0 * playerscount * XDifficultyMultiplier), 3000), false, false);
//		SetConVarInt(z_health, RoundToZero(8.5 * playerscount * XDifficultyMultiplier), false, false);

		SetConVarInt(grenadelauncher_damage, RoundToZero(216.667 * playerscount -466.667), false, false);

		SetConVarInt(z_special_spawn_interval, CheckCvarRange(RoundToZero((-3.333 * playerscount + 58.333) * GetConVarFloat(hm_spawn_time_mod)), 5, 100), false, false);
		SetConVarInt(special_respawn_interval, CheckCvarRange(RoundToZero((-3.333 * playerscount + 58.333) * GetConVarFloat(hm_spawn_time_mod)), 5, 100), false, false);
	}
	else
	{

		if (l4d2_plugin_loot)
		{
			ItemsDropCount[0] = CheckCvarRange(RoundToZero((playerscount / 5.3) * SquareRoot(XDifficultyMultiplier)), 1, 100);
			ItemsDropCount[1] = CheckCvarRange(RoundToZero((playerscount / 4.0) * SquareRoot(XDifficultyMultiplier)), 2, 100);
			ItemsDropCount[2] = CheckCvarRange(RoundToZero((playerscount / 4.0) * SquareRoot(XDifficultyMultiplier)), 1, 100);
			ItemsDropCount[4] = CheckCvarRange(RoundToZero((playerscount / 4.6) * SquareRoot(XDifficultyMultiplier)), 2, 100);
			ItemsDropCount[5] = CheckCvarRange(RoundToZero((playerscount / 4.6) * SquareRoot(XDifficultyMultiplier)), 1, 100);
		}

		SetConVarInt(z_hunter_health, 250, false, false);
		SetConVarInt(z_smoker_health, 250, false, false);
		SetConVarInt(z_boomer_health, 50, false, false);
		SetConVarInt(z_spitter_health, 100, false, false);
		SetConVarInt(z_jockey_health, 325, false, false);
//		SetConVarInt(z_witch_health, 1000, false, false);
//		SetConVarInt(z_health, 50, false, false);

		SetConVarInt(z_special_spawn_interval, 45, false, false);

		SetConVarInt(z_hunter_limit, 1, false, false);
		SetConVarInt(z_smoker_limit, 1, false, false);
		SetConVarInt(z_boomer_limit, 1, false, false);
		SetConVarInt(z_spitter_limit, 1, false, false);
		SetConVarInt(z_jockey_limit, 1, false, false);

		SetConVarInt(z_smoker_speed, 210, false, false);
		SetConVarInt(z_spitter_max_wait_time, 30, false, false);
		SetConVarInt(z_boomer_speed, 175, false, false);
		SetConVarInt(z_spitter_speed, 210, false, false);
		SetConVarInt(grenadelauncher_damage, 400, false, false);
	}

	if (l4d2_plugin_loot)
	{
		SetConVarInt(l4d2_loot_h_drop_items, RoundToZero((CheckCvarRange(ItemsDropCount[0], 0, 2) + BonusDrop) * GetConVarFloat(hm_loot_mod)), false, false);
		SetConVarInt(l4d2_loot_b_drop_items, RoundToZero((CheckCvarRange(ItemsDropCount[1], 0, 4) + BonusDrop) * GetConVarFloat(hm_loot_mod)), false, false);
		SetConVarInt(l4d2_loot_s_drop_items, RoundToZero((CheckCvarRange(ItemsDropCount[2], 0, 2) + BonusDrop) * GetConVarFloat(hm_loot_mod)), false, false);
		SetConVarInt(l4d2_loot_sp_drop_items, RoundToZero((CheckCvarRange(ItemsDropCount[4], 0, 3) + BonusDrop) * GetConVarFloat(hm_loot_mod)), false, false);
		SetConVarInt(l4d2_loot_j_drop_items, RoundToZero((CheckCvarRange(ItemsDropCount[5], 0, 3) + BonusDrop) * GetConVarFloat(hm_loot_mod)), false, false);
	}

	SetConVarInt(smoker_pz_claw_dmg, playerscount, false, false);
	SetConVarInt(jockey_pz_claw_dmg, playerscount, false, false);
	SetConVarInt(tongue_choke_damage_amount, RoundToZero((10 + (playerscount - 4) * 1.666) * XDifficultyMultiplier), false, false);
	SetConVarInt(tongue_drag_damage_amount, RoundToZero(playerscount * 0.75 * XDifficultyMultiplier), false, false);
	SetConVarInt(tongue_miss_delay, CheckCvarRange(17 - playerscount, 1, 100), false, false);
	SetConVarInt(tongue_hit_delay, CheckCvarRange(17 - playerscount, 1, 100), false, false);
	
//	SetConVarInt(l4d2_loot_g_chance_nodrop, CheckCvarMin(RoundToZero(65 / XDifficultyMultiplier), 5), false, false);
	if (GetConVarInt(IsMapFinished) == 0 && l4d2_plugin_monsterbots)
	{
		new Handle:monsterbots_interval = FindConVar("monsterbots_interval");
		SetConVarInt(monsterbots_interval, CheckCvarRange(RoundToZero((26 - playerscount) * GetConVarFloat(hm_spawn_time_mod)), 1, 100), false, false);
	}
	
	SetConVarInt(tongue_range, 750 + RoundToZero((playerscount - 4) * 20 * XDifficultyMultiplier), false, false);

	SetConVarInt(z_spitter_range, 850 + RoundToZero((playerscount - 4) * 20 * XDifficultyMultiplier), false, false);
	SetConVarInt(z_spit_interval, CheckCvarRange(20 - RoundToZero((playerscount - 4) * 0.83 * XDifficultyMultiplier), 5, 100), false, false);
	
	AutoDifficulty_Tank();
	AutoDifficulty_Charger();
}

#if COOP_STATS_ENABLED
	Float:Calculate_Rank_Mod()
	{
		if (GetConVarInt(hm_rankmod_mode) == 0)
		{
			if (Total_Players < 250) return GetConVarFloat(PDifficultyMultiplier);
			rank_sum = 0.0;
			for (new i = 1; i <= L4D_MAXPLAYERS; i++)
			{
				if (IsClientConnected(i))
				{
					if (IsClientInGame(i))
					{
						if (!IsFakeClient(i))
						{
							if (GetClientTeam(i) == TEAM_SURVIVORS)
							{
								if ClientRank[i] == 0 *then rank_sum += 0.0;
								else if ClientRank[i] <= 3 *then rank_sum += 5.0;
								else if ClientRank[i] <= 10 *then rank_sum += 4.0;
								else if ClientRank[i] <= 30 *then rank_sum += 3.2;
								else if ClientRank[i] <= 60 *then rank_sum += 2.6;
								else if ClientRank[i] <= 120 *then rank_sum += 2.0;
								else if ClientRank[i] <= 240 *then rank_sum += 1.0;
							}
						}
					}
				}
			}
	//		PrintToChatAll("%d", rank_sum);
			new Float:result = 0.006 * rank_sum + 0.438;
			if (result < 0.5) result = 0.5;
			if (result > 1.0) result = 1.0;
			return result;
		}
		if (GetConVarInt(hm_rankmod_mode) == 1 || GetConVarInt(hm_rankmod_mode) == 2)
		{
			if (Total_Players < 250) return GetConVarFloat(PDifficultyMultiplier);
			rank_sum = 0.0;
			new players_count = 0;
			for (new i = 1; i <= L4D_MAXPLAYERS; i++)
			{
				if (IsClientConnected(i))
				{
					if (IsClientInGame(i))
					{
						if (!IsFakeClient(i))
						{
							if (GetClientTeam(i) == TEAM_SURVIVORS)
							{
								if ClientRank[i] == 0 *then rank_sum += 0.0;
								else if ClientRank[i] <= 3 *then rank_sum += 5.0;
								else if ClientRank[i] <= 10 *then rank_sum += 4.0;
								else if ClientRank[i] <= 30 *then rank_sum += 3.2;
								else if ClientRank[i] <= 60 *then rank_sum += 2.6;
								else if ClientRank[i] <= 120 *then rank_sum += 2.0;
								else if ClientRank[i] <= 240 *then rank_sum += 1.0;
								players_count++;
							}
						}
					}
				}
			}
			if (players_count < 1) players_count = 1;
			new Float:result = ((rank_sum - (players_count * 1.5)) / players_count) / 6 + 0.75;
			if (result < 0.5) result = 0.5;
			if (result > 1.0) result = 1.0;
			if (GetConVarInt(hm_rankmod_mode) == 2)	return result + GetConVarFloat(PDifficultyMultiplier);
			return result;
		}
		return GetConVarFloat(PDifficultyMultiplier);
	}
#endif

AutoDifficulty_Tank()
{
	new Handle:z_tank_health = FindConVar("z_tank_health");
	new Handle:z_tank_speed = FindConVar("z_tank_speed");
	new Handle:tank_burn_duration = INVALID_HANDLE;
	switch (cvar_difficulty)
	{
		case 1: tank_burn_duration = FindConVar("tank_burn_duration");
		case 3: tank_burn_duration = FindConVar("tank_burn_duration_hard");
		case 4: tank_burn_duration = FindConVar("tank_burn_duration_expert");
	}
	if (playerscount <= 4)
	{
		SetConVarInt(z_tank_health, 6000, false, false);
		SetConVarInt(z_tank_speed, 210, false, false);
		SetConVarInt(tank_burn_duration, 75, false, false);
		if (l4d2_plugin_loot)
		{
			new Handle:l4d2_loot_t_drop_items = FindConVar("l4d2_loot_t_drop_items");
//			new Handle:l4d2_loot_t_chance_nodrop = FindConVar("l4d2_loot_t_chance_nodrop");
			SetConVarInt(l4d2_loot_t_drop_items, 6 + BonusDrop, false, false);
//			SetConVarInt(l4d2_loot_t_chance_nodrop, 0, false, false);
		}
	}
	else
	{
		SetConVarInt(z_tank_health, CheckCvarRange(RoundToZero((3666.667 * playerscount - 8666.667) * RDifficultyMultiplier * GetConVarFloat(hm_tank_hp_mod)), 4000, 85000), false, false);
		SetConVarInt(z_tank_speed, 210 + RoundToZero((8.75 * playerscount - 35) * RDifficultyMultiplier), false, false);
		SetConVarInt(tank_burn_duration, 75 + RoundToZero((18.75 * playerscount - 75) * RDifficultyMultiplier), false, false);
		if (l4d2_plugin_loot)
		{
			new Handle:l4d2_loot_t_drop_items = FindConVar("l4d2_loot_t_drop_items");
//			new Handle:l4d2_loot_t_chance_nodrop = FindConVar("l4d2_loot_t_chance_nodrop");
			SetConVarInt(l4d2_loot_t_drop_items, RoundToZero(((1.417 * playerscount - 2.333) + (2 * BonusDrop)) * GetConVarFloat(hm_loot_mod)), false, false);
//			SetConVarInt(l4d2_loot_t_chance_nodrop, RoundToZero(7.917 * playerscount - 31.667), false, false);
		}
	}
}

AutoDifficulty_Charger()
{
	new Handle:z_charger_limit = FindConVar("z_charger_limit");
	new Handle:z_charger_health = FindConVar("z_charger_health");
	new Handle:l4d2_charger_steering_allow = FindConVar("l4d2_charger_steering_allow");
	new bool:extra_charger = false;
	if (l4d2_charger_steering_allow != INVALID_HANDLE && GetConVarInt(l4d2_charger_steering_allow) > 0)
	{
		extra_charger = true;
	}
	if (playerscount <= 4)
	{
		if (extra_charger)
		{
			SetConVarInt(z_charger_health, 400, false, false);
		}
		else
		{
			SetConVarInt(z_charger_health, 600, false, false);
		}
		if (l4d2_plugin_loot)
		{
			new Handle:l4d2_loot_c_drop_items = FindConVar("l4d2_loot_c_drop_items");
			new Handle:l4d2_loot_c_chance_nodrop = FindConVar("l4d2_loot_c_chance_nodrop");
			SetConVarInt(l4d2_loot_c_drop_items, 2 + BonusDrop, false, false);
			SetConVarInt(l4d2_loot_c_chance_nodrop, RoundToZero(RDifficultyMultiplier * LootNoDrop), false, false);
		}
		SetConVarInt(z_charger_limit, 1, false, false);
	}
	else
	{
		if (extra_charger)
		{
			SetConVarInt(z_charger_health, CheckCvarRange(RoundToZero((116.667 * playerscount + 133.333) * RDifficultyMultiplier * 0.66666 * GetConVarFloat(hm_infected_hp_mod)), 400, 1320), false, false);
		}
		else
		{
			SetConVarInt(z_charger_health, CheckCvarRange(RoundToZero((116.667 * playerscount + 133.333) * RDifficultyMultiplier * GetConVarFloat(hm_infected_hp_mod)), 600, 2000), false, false);
		}
		if (l4d2_plugin_loot)
		{
			new Handle:l4d2_loot_c_drop_items = FindConVar("l4d2_loot_c_drop_items");
			new Handle:l4d2_loot_c_chance_nodrop = FindConVar("l4d2_loot_c_chance_nodrop");
			SetConVarInt(l4d2_loot_c_drop_items, RoundToZero(((0.25 * playerscount + 1) + BonusDrop) * GetConVarFloat(hm_loot_mod)), false, false);
			SetConVarInt(l4d2_loot_c_chance_nodrop, RoundToZero(RDifficultyMultiplier * LootNoDrop), false, false);
		}
		new Handle:supercharger = FindConVar("hm_damage_supercharger");
		if (supercharger != INVALID_HANDLE)
		{
			if (GetConVarInt(supercharger) > 0)
			{
				SetConVarInt(z_charger_limit, RoundToZero(0.083 * playerscount + 0.667), false, false);
			}
			else
			{
				SetConVarInt(z_charger_limit, RoundToZero(0.167 * playerscount + 0.333), false, false);
			}
		}
	}
}

ADPlayerSpawn(Handle:event)
{
	if (GetConVarInt(hm_autodifficulty_forcehp) < 1) return;
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	if (IsValidClient(client))
	{
		if (GetClientTeam(client) == TEAM_INFECTED)
		{
			switch(GetClientZC(client))
			{
				case ZC_SMOKER: SetEntityHealth(client, GetConVarInt(z_smoker_health));
				case ZC_BOOMER: SetEntityHealth(client, GetConVarInt(z_boomer_health));
				case ZC_HUNTER: SetEntityHealth(client, GetConVarInt(z_hunter_health));
				case ZC_SPITTER: SetEntityHealth(client, GetConVarInt(z_spitter_health));
				case ZC_JOCKEY: SetEntityHealth(client, GetConVarInt(z_jockey_health));
				case ZC_CHARGER: 
				{
					new Handle:z_charger_health = FindConVar("z_charger_health");
					SetEntityHealth(client, GetConVarInt(z_charger_health));
				}
				case ZC_TANK:
				{
					new Handle:z_tank_health = FindConVar("z_tank_health");
					SetEntityHealth(client, GetConVarInt(z_tank_health) * 2);
				}
			}
		}
	}
}