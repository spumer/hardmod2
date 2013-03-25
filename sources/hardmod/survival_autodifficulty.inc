new Handle:l4d2_autodifficulty;
new Float:XDifficultyMultiplier = 2.89;

new Handle:tank_burn_duration;
new Handle:z_spitter_max_wait_time;
new Handle:z_vomit_interval;
new Handle:z_smoker_speed;
new Handle:z_boomer_speed;
new Handle:z_spitter_speed;
new Handle:z_tank_speed;
new Handle:z_hunter_limit;
new Handle:z_smoker_limit;
new Handle:z_boomer_limit;
new Handle:z_charger_limit;
new Handle:z_spitter_limit;
new Handle:z_jockey_limit;
new Handle:grenadelauncher_damage;
new Handle:z_special_spawn_interval;
new Handle:special_respawn_interval;
new Handle:smoker_pz_claw_dmg;
new Handle:jockey_pz_claw_dmg;
new Handle:tongue_choke_damage_amount;
new Handle:tongue_drag_damage_amount;
new Handle:tongue_miss_delay;
new Handle:tongue_hit_delay;
new Handle:tongue_range;
new Handle:z_spitter_range;
new Handle:z_spit_interval;

public AutoDiffOnPluginStart()
{
	l4d2_autodifficulty = CreateConVar("l4d2_autodifficulty", "1", "Is the autodifficulty enabled.", FCVAR_PLUGIN);
	tank_burn_duration = FindConVar("tank_burn_duration");
	z_spitter_max_wait_time = FindConVar("z_spitter_max_wait_time");
	z_vomit_interval = FindConVar("z_vomit_interval");
	z_smoker_speed = FindConVar("z_gas_speed");

	z_boomer_speed = FindConVar("z_exploding_speed");
	z_spitter_speed = FindConVar("z_spitter_speed");
	z_tank_speed = FindConVar("z_tank_speed");

	z_hunter_limit = FindConVar("z_hunter_limit");
	z_smoker_limit = FindConVar("z_smoker_limit");
	z_boomer_limit = FindConVar("z_boomer_limit");
	z_charger_limit = FindConVar("z_charger_limit");
	z_spitter_limit = FindConVar("z_spitter_limit");
	z_jockey_limit = FindConVar("z_jockey_limit");

	grenadelauncher_damage = FindConVar("grenadelauncher_damage");
	
	z_special_spawn_interval = FindConVar("z_special_spawn_interval");
	special_respawn_interval = FindConVar("director_special_respawn_interval");
	
	smoker_pz_claw_dmg = FindConVar("smoker_pz_claw_dmg");
	jockey_pz_claw_dmg = FindConVar("jockey_pz_claw_dmg");
	tongue_choke_damage_amount = FindConVar("tongue_choke_damage_amount");
	tongue_drag_damage_amount = FindConVar("tongue_drag_damage_amount");
	tongue_miss_delay = FindConVar("tongue_miss_delay");
	tongue_hit_delay = FindConVar("tongue_hit_delay");
	tongue_range = FindConVar("tongue_range");

	z_spitter_range = FindConVar("z_spitter_range");
	z_spit_interval = FindConVar("z_spit_interval");
}

public ADOnClientPostAdminCheck(client)
{
	Autodifficulty();
}

public ADOnClientDisconnect(client)
{
	Autodifficulty();
}

public Autodifficulty()
{
	if (GetConVarInt(l4d2_autodifficulty) < 1) 
	{
		return;
	}
	
	new playerscount = GetRealClientCount(true);
	
	if (playerscount < 4)
		playerscount = 4;
		
	new BonusDrop = 0;
		
	new ItemsDropCount[7];

	new Handle:l4d2_loot_h_drop_items = FindConVar("l4d2_loot_h_drop_items");
	new Handle:l4d2_loot_b_drop_items = FindConVar("l4d2_loot_b_drop_items");
	new Handle:l4d2_loot_s_drop_items = FindConVar("l4d2_loot_s_drop_items");
	new Handle:l4d2_loot_c_drop_items = FindConVar("l4d2_loot_c_drop_items");
	new Handle:l4d2_loot_sp_drop_items = FindConVar("l4d2_loot_sp_drop_items");
	new Handle:l4d2_loot_j_drop_items = FindConVar("l4d2_loot_j_drop_items");
	new Handle:l4d2_loot_t_drop_items = FindConVar("l4d2_loot_t_drop_items");
	
	new Handle:sv_disable_glow_survivors = FindConVar("sv_disable_glow_survivors");
	
	if (XDifficultyMultiplier < 5)
	{
		SetConVarInt(sv_disable_glow_survivors, 0, false, false);
	}
	else
	{
		SetConVarInt(sv_disable_glow_survivors, 1, false, false);
	}

	if (playerscount > 4)
	{
		SetConVarInt(tank_burn_duration, RoundToZero(18.75 * playerscount), false, false);

		SetConVarInt(z_spitter_max_wait_time, 34 - playerscount, false, false);
		SetConVarInt(z_vomit_interval, 34 - playerscount, false, false);

		SetConVarInt(z_smoker_speed, 210 + RoundToZero(3.0 * (playerscount - 4) * XDifficultyMultiplier), false, false); 
		SetConVarInt(z_boomer_speed, 175 + RoundToZero(3.0 * (playerscount - 4) * XDifficultyMultiplier), false, false); 
		SetConVarInt(z_spitter_speed, 160 + RoundToZero(15.0 * playerscount * XDifficultyMultiplier), false, false);
		SetConVarInt(z_tank_speed, 210 + RoundToZero((playerscount - 4) * 5 * XDifficultyMultiplier), false, false);

		SetConVarInt(z_hunter_limit, RoundToZero(2.5 + (playerscount / 5)), false, false);
		SetConVarInt(z_smoker_limit, RoundToZero(1.5 + (playerscount / 6)), false, false);
		SetConVarInt(z_boomer_limit, RoundToZero(1.5 + (playerscount / 7)), false, false);
		SetConVarInt(z_charger_limit, RoundToZero(0.3 + (playerscount / 7)), false, false);
		SetConVarInt(z_spitter_limit, RoundToZero(1.4 + (playerscount / 6)), false, false);
		SetConVarInt(z_jockey_limit, RoundToZero(0.5 + (playerscount / 8)), false, false);
	
		ItemsDropCount[0] = CheckCvarRange(RoundToZero((playerscount / 5.3) * SquareRoot(XDifficultyMultiplier)), 1, 100);
		ItemsDropCount[1] = CheckCvarRange(RoundToZero((playerscount / 4.0) * SquareRoot(XDifficultyMultiplier)), 1, 100);
		ItemsDropCount[2] = CheckCvarRange(RoundToZero((playerscount / 4.0) * SquareRoot(XDifficultyMultiplier)), 1, 100);
		ItemsDropCount[3] = CheckCvarRange(RoundToZero((playerscount / 4.2) * SquareRoot(XDifficultyMultiplier)), 1, 100);
		ItemsDropCount[4] = CheckCvarRange(RoundToZero((playerscount / 4.6) * SquareRoot(XDifficultyMultiplier)), 1, 100);
		ItemsDropCount[5] = CheckCvarRange(RoundToZero((playerscount / 4.6) * SquareRoot(XDifficultyMultiplier)), 1, 100);
		ItemsDropCount[6] = CheckCvarRange(RoundToZero(playerscount * 5 * SquareRoot(XDifficultyMultiplier)), 5, 100);
		
		SetConVarInt(l4d2_loot_h_drop_items, CheckCvarRange(ItemsDropCount[0], 0, 2) + BonusDrop, false, false);
		SetConVarInt(l4d2_loot_b_drop_items, CheckCvarRange(ItemsDropCount[1], 0, 4) + BonusDrop, false, false);
		SetConVarInt(l4d2_loot_s_drop_items, CheckCvarRange(ItemsDropCount[2], 0, 2) + BonusDrop, false, false);
		SetConVarInt(l4d2_loot_c_drop_items, CheckCvarRange(ItemsDropCount[3], 0, 5) + BonusDrop, false, false);
		SetConVarInt(l4d2_loot_sp_drop_items, CheckCvarRange(ItemsDropCount[4], 0, 3) + BonusDrop, false, false);
		SetConVarInt(l4d2_loot_j_drop_items, CheckCvarRange(ItemsDropCount[5], 0, 3) + BonusDrop, false, false);
		SetConVarInt(l4d2_loot_t_drop_items, CheckCvarRange(ItemsDropCount[6] + (BonusDrop * 3), 0, playerscount) + BonusDrop, false, false);

		SetConVarInt(grenadelauncher_damage, RoundToZero((187.5 * playerscount) + 0.3), false, false);
		
		SetConVarInt(z_special_spawn_interval, CheckCvarRange(49 - (playerscount * 3), 5, 100), false, false);
		SetConVarInt(special_respawn_interval, CheckCvarRange(49 - (playerscount * 3), 5, 100), false, false);		
	}
	else
	{
		SetConVarInt(z_special_spawn_interval, 45, false, false);

		SetConVarInt(z_hunter_limit, 1, false, false);
		SetConVarInt(z_smoker_limit, 1, false, false);
		SetConVarInt(z_boomer_limit, 1, false, false);
		SetConVarInt(z_charger_limit, 1, false, false);
		SetConVarInt(z_spitter_limit, 1, false, false);
		SetConVarInt(z_jockey_limit, 1, false, false);

		SetConVarInt(z_smoker_speed, 210, false, false);
		SetConVarInt(z_spitter_max_wait_time, 30, false, false);
		SetConVarInt(z_boomer_speed, 175, false, false);
		SetConVarInt(z_spitter_speed, 210, false, false);
		SetConVarInt(z_tank_speed, 210, false, false);

		SetConVarInt(grenadelauncher_damage, 400, false, false);
	}
	
	SetConVarInt(smoker_pz_claw_dmg, playerscount, false, false);
	SetConVarInt(jockey_pz_claw_dmg, playerscount, false, false);
	SetConVarInt(tongue_choke_damage_amount, RoundToZero((10 + (playerscount - 4) * 1.666) * XDifficultyMultiplier), false, false);
	SetConVarInt(tongue_drag_damage_amount, RoundToZero(playerscount * 0.75 * XDifficultyMultiplier), false, false);
	SetConVarInt(tongue_miss_delay, CheckCvarRange(17 - playerscount, 1, 100), false, false);
	SetConVarInt(tongue_hit_delay, CheckCvarRange(17 - playerscount, 1, 100), false, false);
	
	SetConVarInt(tongue_range, 750 + RoundToZero((playerscount - 4) * 20 * XDifficultyMultiplier), false, false);

	SetConVarInt(z_spitter_range, 850 + RoundToZero((playerscount - 4) * 20 * XDifficultyMultiplier), false, false);
	SetConVarInt(z_spit_interval, CheckCvarRange(20 - RoundToZero((playerscount - 4) * 0.83 * XDifficultyMultiplier), 5, 100), false, false);
	
	// decl String:sGameDifficulty[16];
	// GetConVarString(z_difficulty, sGameDifficulty, sizeof(sGameDifficulty));
		
	// new TankHP = 4000;
		
	// if (StrEqual(sGameDifficulty, "Easy", false))
	// {
		// TankHP = RoundToZero(500 * playerscount * XDifficultyMultiplier);
	// }
	// else if (StrEqual(sGameDifficulty, "Normal", false))
	// {
		// TankHP = RoundToZero(600 * playerscount * XDifficultyMultiplier);
	// }
	// else if (StrEqual(sGameDifficulty, "Hard", false))
	// {
		// TankHP = RoundToZero(700 * playerscount * XDifficultyMultiplier);
	// }
	// else if (StrEqual(sGameDifficulty, "Impossible", false))
	// {
		// TankHP = RoundToZero(800 * XDifficultyMultiplier * playerscount);
	// }
	// TankHP = CheckCvarMin(TankHP, 4000);
	// TankHP = CheckCvarMax(TankHP, 35000);
	// SetConVarInt(z_tank_health, TankHP, false, false);
}