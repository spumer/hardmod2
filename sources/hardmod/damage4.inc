new Handle:hm_damage;
new Handle:hm_damage_friendly;
new Handle:hm_damage_showvalue;
new Handle:hm_damage_supercharger;

new Handle:hm_damage_hunter;
new Handle:hm_damage_smoker;
new Handle:hm_damage_boomer;
new Handle:hm_damage_spitter1;
new Handle:hm_damage_spitter2;
new Handle:hm_damage_jockey;
new Handle:hm_damage_charger;
new Handle:hm_damage_tank;
new Handle:hm_damage_tankrock;
new Handle:hm_damage_common;

new Handle:hm_damage_ak47;
new Handle:hm_damage_awp;
new Handle:hm_damage_scout;
new Handle:hm_damage_m60;
new Handle:hm_damage_pipebomb;
new Handle:hm_damage_spas;
new Handle:hm_damage_sg552; // rifle_sg552
new Handle:hm_damage_sniper_military;

//new Handle:hm_damage_chainsaw_fix;

public DamagePluginInitialization()
{
}

public DamageOnPluginStart()
{
	Version += 14;

	hm_damage = CreateConVar("hm_damage", "0", "Enable/Disable damage", FCVAR_PLUGIN);
	hm_damage_friendly = CreateConVar("hm_damage_friendly", "0.2", "Enable/Disable ff damage", FCVAR_PLUGIN);
	hm_damage_showvalue = CreateConVar("hm_damage_showvalue", "0", "Enable/Disable show damage", FCVAR_PLUGIN);
	hm_damage_supercharger = CreateConVar("hm_damage_supercharger", "0", "Enable/Disable damage", FCVAR_PLUGIN);

	hm_damage_hunter = CreateConVar("hm_damage_hunter", "0", "Hunter additional damage", FCVAR_PLUGIN);
	hm_damage_smoker = CreateConVar("hm_damage_smoker", "0", "Smoker additional damage", FCVAR_PLUGIN);
	hm_damage_boomer = CreateConVar("hm_damage_boomer", "0", "Boomer additional damage", FCVAR_PLUGIN);
	hm_damage_spitter1 = CreateConVar("hm_damage_spitter1", "0", "Spitter additional damage", FCVAR_PLUGIN);
	hm_damage_spitter2 = CreateConVar("hm_damage_spitter2", "0", "Spitter additional damage (spit)", FCVAR_PLUGIN);
	hm_damage_jockey = CreateConVar("hm_damage_jockey", "0", "Jockey additional damage", FCVAR_PLUGIN);
	hm_damage_charger = CreateConVar("hm_damage_charger", "0", "Charger additional damage", FCVAR_PLUGIN);
	hm_damage_tank = CreateConVar("hm_damage_tank", "0", "Tank additional damage", FCVAR_PLUGIN);
	hm_damage_tankrock = CreateConVar("hm_damage_tankrock", "0", "Tank additional damage", FCVAR_PLUGIN);
	hm_damage_common = CreateConVar("hm_damage_common", "0", "Common additional damage", FCVAR_PLUGIN);
	
	hm_damage_ak47 = CreateConVar("hm_damage_ak47", "0", "AK47 additional damage", FCVAR_PLUGIN);
	hm_damage_awp = CreateConVar("hm_damage_awp", "0", "AWP additional damage", FCVAR_PLUGIN);
	hm_damage_scout = CreateConVar("hm_damage_scout", "0", "Scout additional damage", FCVAR_PLUGIN);
	hm_damage_m60 = CreateConVar("hm_damage_m60", "0", "M60 additional damage", FCVAR_PLUGIN);
	hm_damage_spas = CreateConVar("hm_damage_spas", "0", "SPAS additional damage", FCVAR_PLUGIN);
	hm_damage_sg552 = CreateConVar("hm_damage_sg552", "0", "SG552 additional damage", FCVAR_PLUGIN);
	hm_damage_sniper_military = CreateConVar("hm_damage_sniper_military", "0", "sniper military additional damage", FCVAR_PLUGIN);
	hm_damage_pipebomb = CreateConVar("hm_damage_pipebomb", "0", "Pipe bomb additional damage", FCVAR_PLUGIN);
	
//	hm_damage_chainsaw_fix = CreateConVar("hm_damage_chainsaw_fix", "1", "Fix for chainsaw (crashing server)", FCVAR_PLUGIN);

	HookEvent("player_hurt", Event_PlayerHurt, EventHookMode_Pre);
//	HookEvent("charger_charge_end", Event_ChargerCarryEnd, EventHookMode_Pre); 
	HookEvent("charger_carry_start", Event_ChargerCarryStart, EventHookMode_Pre);
	HookEvent("charger_carry_end", Event_ChargerCarryEnd, EventHookMode_Pre);
}

DMOnClientPutInServer(client)
{
	SDKHook(client, SDKHook_OnTakeDamage, OnTakeDamage);
}

public Action:OnTakeDamage(victim, &attacker, &inflictor, &Float:damage, &damagetype)
{
//	PrintToChatAll("OnTakeDamage event");
	if (GetConVarInt(hm_damage) != 1 || !victim) return Plugin_Continue;
	if (inflictor > L4D_MAXPLAYERS || attacker > L4D_MAXPLAYERS || !attacker || damage == 0.0) return Plugin_Continue;
	decl String:Weapon[32];
	GetClientWeapon(attacker, Weapon, sizeof(Weapon));
	new Float:original_damage = damage;
//	PrintToChatAll("[OnTakeDamage] wpn = %s, type = %d", Weapon, damagetype);
	if (damagetype == 128)
	{
		if (StrEqual(Weapon, "weapon_boomer_claw")) damage *= GetConVarFloat(hm_damage_boomer);
		else if (StrEqual(Weapon, "weapon_charger_claw")) damage *= GetConVarFloat(hm_damage_charger);
		else if (StrEqual(Weapon, "weapon_hunter_claw")) damage *= GetConVarFloat(hm_damage_hunter);
		else if (StrEqual(Weapon, "weapon_smoker_claw")) damage *= GetConVarFloat(hm_damage_smoker);
		else if (StrEqual(Weapon, "weapon_spitter_claw")) damage *= GetConVarFloat(hm_damage_spitter1);
		else if (StrEqual(Weapon, "weapon_jockey_claw")) damage *= GetConVarFloat(hm_damage_jockey);
		else if (StrEqual(Weapon, "weapon_tank_claw")) damage *= GetConVarFloat(hm_damage_tank);
		else if (StrEqual(Weapon, "weapon_tank_rock")) damage *= GetConVarFloat(hm_damage_tankrock);
	}
	else
	{
		if (StrEqual(Weapon, "weapon_rifle_ak47")) damage *= GetConVarFloat(hm_damage_ak47);
		else if (StrEqual(Weapon, "weapon_sniper_awp")) damage *= GetConVarFloat(hm_damage_awp);
		else if (StrEqual(Weapon, "weapon_sniper_scout")) damage *= GetConVarFloat(hm_damage_scout);
		else if (StrEqual(Weapon, "weapon_rifle_m60")) damage *= GetConVarFloat(hm_damage_m60);
		else if (StrEqual(Weapon, "weapon_shotgun_spas")) damage *= GetConVarFloat(hm_damage_spas);
		else if (StrEqual(Weapon, "weapon_rifle_sg552")) damage *= GetConVarFloat(hm_damage_sg552);
		else if (StrEqual(Weapon, "weapon_sniper_military")) damage *= GetConVarFloat(hm_damage_sniper_military);
	}
	if (original_damage != damage)
	{
		if (GetClientTeam(victim) == 2 && GetClientTeam(attacker) == 2)
		{
			damage *= GetConVarFloat(hm_damage_friendly);
		}
		return Plugin_Changed;
	}
	return Plugin_Continue;
}

public IncapTarget(target)
{
	if(IsValidEntity(target))
	{
		new iDmgEntity = CreateEntityByName("point_hurt");
		SetEntityHealth(target, 1);
		DispatchKeyValue(target, "targetname", "bm_target");
		DispatchKeyValue(iDmgEntity, "DamageTarget", "bm_target");
		DispatchKeyValue(iDmgEntity, "Damage", "100");
		DispatchKeyValue(iDmgEntity, "DamageType", "0");
		DispatchSpawn(iDmgEntity);
		AcceptEntityInput(iDmgEntity, "Hurt", target);
		DispatchKeyValue(target, "targetname", "bm_targetoff");
		RemoveEdict(iDmgEntity);
	}
}

public Action:TimedKill(Handle:timer, any:client)
{
	if (IsClientInGame(client) && IsPlayerAlive(client))
	{
		ForcePlayerSuicide(client);
	}
}

public Action:Event_PlayerHurt(Handle:event, const String:name[], bool:dontBroadcast)
{
	if (GetConVarInt(hm_damage) < 1) return Plugin_Continue;
	new enemy = GetClientOfUserId(GetEventInt(event, "attacker"));
	new target = GetClientOfUserId(GetEventInt(event, "userid"));
	new dmg_health = GetEventInt(event, "dmg_health");
	new damagetype = GetEventInt(event, "type");
	if (GetConVarInt(hm_damage) < 2 && damagetype == 128 || target == 0 || !dmg_health) return Plugin_Continue;
	decl String:weapon[16];
	GetEventString(event, "weapon", weapon, sizeof(weapon));
	new hardmod_damage = 0;
/* 	PrintToChatAll("[PlayerHurt] wpn = %s, type = %d", weapon, damagetype);
	if (GetConVarInt(hm_damage_chainsaw_fix) > 0 && damagetype == 134217792 && StrEqual(weapon, "pipe_bomb", false))
	{
		CancelCreatedEvent(event);
		return Plugin_Continue;
	}
 */	if (StrEqual(weapon, "insect_swarm", false)) hardmod_damage = GetConVarInt(hm_damage_spitter2);
	else if (StrEqual(weapon, "pipe_bomb", false)) hardmod_damage = GetConVarInt(hm_damage_pipebomb);
	else if (StrEqual(weapon, "", false)) hardmod_damage = GetConVarInt(hm_damage_common);
	else if (GetConVarInt(hm_damage) > 1 && enemy)
	{
		if (damagetype == 128)
		{
			if (StrEqual(weapon, "boomer_claw")) hardmod_damage = GetConVarInt(hm_damage_boomer);
			else if (StrEqual(weapon, "charger_claw")) hardmod_damage = GetConVarInt(hm_damage_charger);
			else if (StrEqual(weapon, "hunter_claw")) hardmod_damage = GetConVarInt(hm_damage_hunter);
			else if (StrEqual(weapon, "smoker_claw")) hardmod_damage = GetConVarInt(hm_damage_smoker);
			else if (StrEqual(weapon, "spitter_claw")) hardmod_damage = GetConVarInt(hm_damage_spitter1);
			else if (StrEqual(weapon, "jockey_claw")) hardmod_damage = GetConVarInt(hm_damage_jockey);
			else if (StrEqual(weapon, "tank_claw")) hardmod_damage = GetConVarInt(hm_damage_tank);
			else if (StrEqual(weapon, "tank_rock")) hardmod_damage = GetConVarInt(hm_damage_tankrock);
		}
		else
		{
			if (StrEqual(weapon, "rifle_ak47")) hardmod_damage = GetConVarInt(hm_damage_ak47);
			else if (StrEqual(weapon, "sniper_awp")) hardmod_damage = GetConVarInt(hm_damage_awp);
			else if (StrEqual(weapon, "sniper_scout")) hardmod_damage = GetConVarInt(hm_damage_scout);
			else if (StrEqual(weapon, "rifle_m60")) hardmod_damage = GetConVarInt(hm_damage_m60);
			else if (StrEqual(weapon, "shotgun_spas")) hardmod_damage = GetConVarInt(hm_damage_spas);
			else if (StrEqual(weapon, "rifle_sg552")) hardmod_damage = GetConVarInt(hm_damage_sg552);
			else if (StrEqual(weapon, "sniper_military")) hardmod_damage = GetConVarInt(hm_damage_sniper_military);
		}
	}
//	else return Plugin_Continue;
	if (hardmod_damage > 0)
	{
		if (enemy && GetClientTeam(target) == 2 && GetClientTeam(enemy) == 2)
		{
			hardmod_damage = RoundToNearest(hardmod_damage * GetConVarFloat(hm_damage_friendly));
		}
		dmg_health += hardmod_damage;
		SetEventInt(event, "dmg_health", dmg_health);
		DamageTarget(target, hardmod_damage);
	}
	if (GetConVarInt(hm_damage_showvalue) > 0)
	{
		if (IsValidClient(enemy) && !IsFakeClient(enemy))
		{
			PrintHintText(enemy, "%d", dmg_health);
		}
		else if (IsValidClient(target) && !IsFakeClient(target))
		{
			PrintHintText(target, "-%d", dmg_health);
		}
	}
	return Plugin_Continue;	
}

public DamageTarget(any:client, damage)
{
	if (GetHealth(client) < 1) return;
	new HP = GetHealth(client);
	if (HP > damage)
	{
		SetEntityHealth(client, HP - damage);
	}
	else
	{
		if (HP > 1)
		{
			damage -= HP - 1;
			SetEntityHealth(client, 1);
		}
		new TempHP = GetClientTempHealth(client);
		if (TempHP >= damage)
		{
			SetTempHealth(client, TempHP - damage);
		}
		else
		{
			if (GetClientTeam(client) == 2 && !IsGoingToDie(client))
			{
				IncapTarget(client);
			}
			else
			{
				if (GetConVarInt(hm_damage) > 2)
				{
					DamageEffect(client, 5.0);
				}
				else
				{
					SetTempHealth(client, 0);
				}
			}
		}
	}
}

stock DamageEffect(target, Float:damage) // (C) http://forums.alliedmods.net/showthread.php?p=1229450
{
	decl String:tName[20];
	Format(tName, 20, "target%d", target);
	new pointHurt = CreateEntityByName("point_hurt");
	DispatchKeyValue(target, "targetname", tName);
	DispatchKeyValueFloat(pointHurt, "Damage", damage);
	DispatchKeyValue(pointHurt, "DamageTarget", tName);
	DispatchKeyValue(pointHurt, "DamageType", "65536");
	DispatchSpawn(pointHurt);
	AcceptEntityInput(pointHurt, "Hurt");
	AcceptEntityInput(pointHurt, "Kill");
}

public Action:Event_ChargerCarryStart(Handle:event, const String:name[], bool:dontBroadcast)
{
	if (GetConVarInt(hm_damage_supercharger) < 1)
	{
		return Plugin_Continue;
	}
	if (GetConVarInt(hm_damage_supercharger) < 2 && GetRealClientCount() < 8)
	{
		return Plugin_Continue;
	}
//	else
//	{
//		PrintToChatAll("hm_damage_supercharger = %d; GetRealClientCount() = %d", GetConVarInt(hm_damage_supercharger), GetRealClientCount());
//	}
	new target = GetClientOfUserId(GetEventInt(event, "victim"));
	SetEntProp(target, Prop_Send, "m_currentReviveCount", 2);
	SetEntProp(target, Prop_Send, "m_isGoingToDie", 1);
	SetEntityHealth(target, 0);
	return Plugin_Continue;
}

public Action:Event_ChargerCarryEnd(Handle:event, const String:name[], bool:dontBroadcast)
{
	if (GetConVarInt(hm_damage_supercharger) < 1)
	{
		return Plugin_Continue;
	}
	if (GetConVarInt(hm_damage_supercharger) < 2 && GetRealClientCount() < 8)
	{
		return Plugin_Continue;
	}
//	else
//	{
//		PrintToChatAll("hm_damage_supercharger = %d; GetRealClientCount() = %d", GetConVarInt(hm_damage_supercharger), GetRealClientCount());
//	}
	new target = GetClientOfUserId(GetEventInt(event, "victim"));
	CreateTimer(0.01, TimedKill, target);
	return Plugin_Continue;
}