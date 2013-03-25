#if CHEAT_ENABLED == false
	#undef CHEAT_ENABLED
	#define CHEAT_ENABLED true
	CheatOnPluginStart();
#endif

public FunOnPluginStart()
{
	Version += 5;
	RegAdminCmd("sm_grenadelauncher", Command_GrenadeLauncher, ADMFLAG_CHEATS, "sm_grenadelauncher");
	RegAdminCmd("sm_awp", Command_AWP, ADMFLAG_CHEATS, "sm_swp");
	RegAdminCmd("sm_scout", Command_Scout, ADMFLAG_CHEATS, "sm_scout");
	RegAdminCmd("sm_ak47", Command_AK47, ADMFLAG_CHEATS, "sm_ak47");
	RegAdminCmd("sm_m16", Command_M16, ADMFLAG_CHEATS, "sm_m16");
	RegAdminCmd("sm_m60", Command_M60, ADMFLAG_CHEATS, "sm_m60");
	RegAdminCmd("sm_spas", Command_SPAS, ADMFLAG_CHEATS, "sm_spas");
	RegAdminCmd("sm_mp5", Command_MP5, ADMFLAG_CHEATS, "sm_mp5");
	RegAdminCmd("sm_uzi", Command_Uzi, ADMFLAG_CHEATS, "sm_uzi");
	RegAdminCmd("sm_mac10", Command_Uzi2, ADMFLAG_CHEATS, "sm_mac10");
	RegAdminCmd("sm_shotgun", Command_Shotgun, ADMFLAG_CHEATS, "sm_shotgun");
	RegAdminCmd("sm_pistol", Command_Pistol, ADMFLAG_CHEATS, "sm_pistol");
	RegAdminCmd("sm_magnum", Command_Magnum, ADMFLAG_CHEATS, "sm_magnum");
	RegAdminCmd("sm_chainsaw", Command_Chainsaw, ADMFLAG_CHEATS, "sm_chainsaw");
	RegAdminCmd("sm_crowbar", Command_Crowbar, ADMFLAG_CHEATS, "sm_crowbar");
	RegAdminCmd("sm_freeman", Command_Crowbar, ADMFLAG_CHEATS, "sm_crowbar");
	RegAdminCmd("sm_null", Command_Null, ADMFLAG_CHEATS, "sm_null");
	RegAdminCmd("sm_left4dead", Command_Left4Dead, ADMFLAG_CHEATS, "sm_left4dead");
	RegAdminCmd("sm_left4dead2", Command_Left4Dead2, ADMFLAG_CHEATS, "sm_left4dead2");
	RegAdminCmd("sm_setbw", Command_SetBW, ADMFLAG_CHEATS, "sm_setbw");
}

public Action:Command_GrenadeLauncher(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i) && IsValidEdict(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				SetGrenadeLauncher(i);
			}
		}
	}
}

public Action:Command_AWP(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i) && IsValidEdict(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				SetWeapon(i, "weapon_sniper_awp", "weapon_pistol_magnum");
			}
		}
	}
}

public Action:Command_Scout(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i) && IsValidEdict(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				SetWeapon(i, "weapon_sniper_scout", "weapon_pistol_magnum");
			}
		}
	}
}

public Action:Command_AK47(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i) && IsValidEdict(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				SetWeapon(i, "weapon_rifle_ak47", "weapon_pistol_magnum");
			}
		}
	}
}

public Action:Command_M16(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i) && IsValidEdict(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				SetWeapon(i, "weapon_rifle", "weapon_pistol_magnum");
			}
		}
	}
}

public Action:Command_M60(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i) && IsValidEdict(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				SetM60(i);
			}
		}
	}
}

public Action:Command_SPAS(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i) && IsValidEdict(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				SetWeapon(i, "weapon_shotgun_spas", "weapon_pistol_magnum");
			}
		}
	}
}

public Action:Command_MP5(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i) && IsValidEdict(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				SetWeapon(i, "weapon_smg_mp5", "weapon_pistol_magnum");
			}
		}
	}
}

public Action:Command_Uzi(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i) && IsValidEdict(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				SetWeaponOnly(i, "weapon_smg", "weapon_pistol");
			}
		}
	}
}

public Action:Command_Uzi2(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i) && IsValidEdict(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				SetWeaponOnly(i, "weapon_smg_silenced", "weapon_pistol");
			}
		}
	}
}

public Action:Command_Shotgun(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i) && IsValidEdict(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				SetWeaponOnly(i, "shotgun_chrome", "weapon_pistol");
			}
		}
	}
}

public Action:Command_Pistol(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i) && IsValidEdict(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				SetWeaponOnly(i, "", "weapon_pistol");
			}
		}
	}
}

public Action:Command_Magnum(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i) && IsValidEdict(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				SetWeaponOnly(i, "", "weapon_pistol_magnum");
			}
		}
	}
}

public Action:Command_Chainsaw(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i) && IsValidEdict(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				SetWeaponOnly(i, "", "weapon_chainsaw");
			}
		}
	}
}

public Action:Command_Crowbar(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i) && IsValidEdict(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				SetWeaponOnly(i, "", "weapon_crowbar");
			}
		}
	}
}

public Action:Command_Null(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i) && IsValidEdict(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				SetNullWeapon(i);
			}
		}
	}
}

public Action:Command_Left4Dead(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i) && IsValidEdict(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				SetDeadWeapon(i);
			}
		}
	}
	new Handle:l4d_stoptk_enabled = INVALID_HANDLE;
	l4d_stoptk_enabled = FindConVar("l4d_stoptk_enabled");
	if (l4d_stoptk_enabled != INVALID_HANDLE)
	{
		SetConVarInt(l4d_stoptk_enabled, 0);
	}	
}

public Action:Command_Left4Dead2(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i) && IsValidEdict(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				SetDeadWeapon2(i);
			}
		}
	}
	new Handle:l4d_stoptk_enabled = INVALID_HANDLE;
	l4d_stoptk_enabled = FindConVar("l4d_stoptk_enabled");
	if (l4d_stoptk_enabled != INVALID_HANDLE)
	{
		SetConVarInt(l4d_stoptk_enabled, 0);
	}
}

public Action:Command_SetBW(client, args)
{
	decl String:arg[64];
	GetCmdArg(1, arg, sizeof(arg));

	decl String:target_name[64];
	decl target_list[L4D_MAXPLAYERS + 1];
	decl target_count;
	decl bool:tn_is_ml;

	if ((target_count = ProcessTargetString(arg, client, target_list,	MAXPLAYERS,	0, target_name,	sizeof(target_name), tn_is_ml)) <= 0)
	{
		ReplyToTargetError(client, target_count);
		return;
	}

	for (new i = 0; i < target_count; i++)
	{
		SetEntProp(target_list[i], Prop_Send, "m_isGoingToDie", 2);
		SetEntProp(target_list[i], Prop_Send, "m_iHealth", 1, 1);
		SetEntProp(target_list[i], Prop_Send, "m_currentReviveCount", 2);
	}
}

public SetGrenadeLauncher(client)
{
	for (new i = 0; i < 5; i++)
	{
		if (GetPlayerWeaponSlot(client, i) > -1)
		{
			RemovePlayerItem(client, GetPlayerWeaponSlot(client, i));
		}
	}
	CheatCMD(client, "give", "weapon_grenade_launcher");
	SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_iExtraPrimaryAmmo", 0);
	SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_iClip1", 100);
	SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_upgradeBitVec", 1);
	SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_nUpgradedPrimaryAmmoLoaded", 100);
	CheatCMD(client, "give", "weapon_chainsaw");
	CheatCMD(client, "give", "weapon_molotov");
	CheatCMD(client, "give", "weapon_first_aid_kit");
	CheatCMD(client, "give", "weapon_pain_pills");
	CheatCMD(client, "give", "health");
	SetEntProp(client, Prop_Send, "m_iHealth", 100, 1);
}

public SetM60(client)
{
	for (new i = 0; i < 5; i++)
	{
		if (GetPlayerWeaponSlot(client, i) > -1)
		{
			RemovePlayerItem(client, GetPlayerWeaponSlot(client, i));
		}
	}
	CheatCMD(client, "give", "weapon_rifle_m60");
//	SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_iExtraPrimaryAmmo", 250);
	SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_iClip1", 250);
	SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_upgradeBitVec", 0);
	SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_nUpgradedPrimaryAmmoLoaded", 0);
	CheatCMD(client, "give", "weapon_chainsaw");
	CheatCMD(client, "give", "weapon_molotov");
	CheatCMD(client, "give", "weapon_upgradepack_explosive");
	CheatCMD(client, "give", "weapon_pain_pills");
	CheatCMD(client, "give", "health");
	SetEntProp(client, Prop_Send, "m_iHealth", 100, 1);
	RemoveTempHealth(client);
}

public SetWeapon(client, const String:weapon_name_a[], const String:weapon_name_b[])
{
	for (new i = 0; i < 5; i++)
	{
		if (GetPlayerWeaponSlot(client, i) > -1)
		{
			RemovePlayerItem(client, GetPlayerWeaponSlot(client, i));
		}
	}
	CheatCMD(client, "give", weapon_name_a);
	CheatCMD(client, "give", weapon_name_b);
	CheatCMD(client, "give", "weapon_molotov");
	CheatCMD(client, "give", "weapon_first_aid_kit");
	CheatCMD(client, "give", "weapon_pain_pills");
	CheatCMD(client, "give", "health");
	SetEntProp(client, Prop_Send, "m_iHealth", 100, 1);
	RemoveTempHealth(client);
}

public SetWeaponOnly(client, const String:weapon_name_a[], const String:weapon_name_b[])
{
	for (new i = 0; i < 5; i++)
	{
		if (GetPlayerWeaponSlot(client, i) > -1)
		{
			RemovePlayerItem(client, GetPlayerWeaponSlot(client, i));
		}
	}
	CheatCMD(client, "give", weapon_name_a);
	CheatCMD(client, "give", weapon_name_b);
	CheatCMD(client, "give", "health");
	SetEntProp(client, Prop_Send, "m_iHealth", 50, 1);
	RemoveTempHealth(client);
}

public SetNullWeapon(client)
{
	for (new i = 0; i < 5; i++)
	{
		if (GetPlayerWeaponSlot(client, i) > -1)
		{
			RemovePlayerItem(client, GetPlayerWeaponSlot(client, i));
		}
	}
	SetEntProp(client, Prop_Send, "m_iHealth", 50, 1);
	RemoveTempHealth(client);
}

public SetDeadWeapon(client)
{
	for (new i = 0; i < 5; i++)
	{
		if (GetPlayerWeaponSlot(client, i) > -1)
		{
			RemovePlayerItem(client, GetPlayerWeaponSlot(client, i));
		}
	}
	CheatCMD(client, "give", "weapon_pain_pills");
	CheatCMD(client, "give", "weapon_defibrillator");
	SetEntProp(client, Prop_Send, "m_isGoingToDie", 1);
	SetEntProp(client, Prop_Send, "m_iHealth", 1, 1);
	SetEntProp(client, Prop_Send, "m_currentReviveCount", 2);
	PrintToChat(client, "\x04Be careful - you are going to die!");
	RemoveTempHealth(client);
}

public SetDeadWeapon2(client)
{
	for (new i = 0; i < 5; i++)
	{
		if (GetPlayerWeaponSlot(client, i) > -1)
		{
			RemovePlayerItem(client, GetPlayerWeaponSlot(client, i));
		}
	}
	CheatCMD(client, "give", "weapon_pain_pills");
	CheatCMD(client, "give", "weapon_defibrillator");
	CheatCMD(client, "give", "baseball_bat");
	SetEntProp(client, Prop_Send, "m_isGoingToDie", 2);
	SetEntProp(client, Prop_Send, "m_iHealth", 1, 1);
	SetEntProp(client, Prop_Send, "m_currentReviveCount", 2);
	PrintToChat(client, "\x04Be careful - you are going to die!");
	RemoveTempHealth(client);
}