// based on http://forums.alliedmods.net/showthread.php?p=1349430
// © http://forums.alliedmods.net/member.php?u=96768

#pragma semicolon 1
 
#include <sourcemod>
#include <sdktools>

new String:playerIds[L4D_MAXPLAYERS + 1][MAX_LINE_WIDTH];
new String:slotPrimary[L4D_MAXPLAYERS + 1][MAX_LINE_WIDTH];
new String:slotSecondary[L4D_MAXPLAYERS + 1][MAX_LINE_WIDTH];
new String:slotThrowable[L4D_MAXPLAYERS + 1][MAX_LINE_WIDTH];
new String:slotMedkit[L4D_MAXPLAYERS + 1][MAX_LINE_WIDTH];
new String:slotPills[L4D_MAXPLAYERS + 1][MAX_LINE_WIDTH];
new priAmmo[L4D_MAXPLAYERS + 1];
new priClip[L4D_MAXPLAYERS + 1];
new priUpgrade[L4D_MAXPLAYERS + 1];
new priUpgrAmmo[L4D_MAXPLAYERS + 1];
new slotHealth[L4D_MAXPLAYERS + 1];
new String:slotAuth[L4D_MAXPLAYERS + 1][MAX_STEAM_LENGTH];
new bool:SavedWeapons[L4D_MAXPLAYERS + 1];
new DefibUsedTime[L4D_MAXPLAYERS + 1];

new do_not_clear[L4D_MAXPLAYERS + 1];
new is_first_map_count;

new Handle:hardmod_KIF;
new Handle:hardmod_KIF_auth_check;

public KeepItemFixOnPluginStart()
{
	Version += 28;
	hardmod_KIF = CreateConVar("hardmod_kif", "1", "", FCVAR_PLUGIN);
	hardmod_KIF_auth_check = CreateConVar("hardmod_kif_auth_check", "1", "", FCVAR_PLUGIN);
	HookEvent("round_start", Event_PTRoundStart);
	RegAdminCmd("keep_save", Command_Save, ADMFLAG_CHEATS, "keep_save");
	RegAdminCmd("keep_load", Command_Load, ADMFLAG_CHEATS, "keep_load");
	RegAdminCmd("keep_clear", Command_Clear, ADMFLAG_CHEATS, "keep_clear");
	RegConsoleCmd("keep_read", Command_Read);
	RegConsoleCmd("keep_current", Command_Current);
	RegAdminCmd("keep_unsaved", Command_UnSaved, ADMFLAG_CHEATS, "keep_unsaved");

	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		SavedWeapons[i] = false;
	}
}

public KIFOnClientPostAdminCheck(client)
{
// here
	if (IsFakeClient(client))
	{
		return;
	}
//	PrintToChatAll("\x05public KIFOnClientPostAdminCheck(\x04%N\x05)", client);
//	PrintToChatAll("\x05IsFirstMapFlag = \x04%d", IsFirstMapFlag);
	if (!IsFirstMapFlag)
	{
		CreateTimer(1.5, CycleRetryRestore, client);
	}
	else
	{
		ClearClientData(client);
		CreateTimer(1.5, CycleRetryRestore, client);
	}
}

public KIFOnClientDisconnectPost(client)
{
//
}

public KIFOnClientConnected(client)
{
//
}

public KIFOnClientPutInServer(client)
{
//
}

public Action:Command_Save(client, args)
{
	SaveClients();
}

public Action:Command_Load(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
//		PrintToChatAll("\x05Command_Load(client = %N)", i);
		RestoreClientData(i);
	}
}

public Action:Command_Clear(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		ClearClientData(i);
	}
}

public Action:Command_Read(client, args)
{
	PrintToChat(client, "\x05slotPrimary = [\x04%s\x05] \x01|\x05 slotSecondary = [\x04%s\x05]", slotPrimary[client], slotSecondary[client]);
	PrintToChat(client, "\x05ammo[\x04%d\x05], clip[\x04%d\x05], upgrade[\x04%d\x05], upgradeammo[\x04%d\x05]", priAmmo[client], priClip[client], priUpgrade[client], priUpgrAmmo[client]);
	PrintToChat(client, "\x05slotThrowable = [\x04%s\x05]", slotThrowable[client]);
	PrintToChat(client, "\x05slotMedkit = [\x04%s\x05] \x01|\x05 slotPills = [\x04%s\x05], \x05slotHealth = [\x04%d\x05]", slotMedkit[client], slotPills[client], slotHealth[client]);
	PrintToChat(client, "\x05Keep weapons = [\x04%d\x05], \x05Saved weapons = [\x04%d\x05]", do_not_clear[client], SavedWeapons[client]);
	PrintToChat(client, "\x05IsMapFinished = \x04%d\x05 :: rounds = \x04%d\x05 :: first_map = \x04%d\x05", GetConVarInt(IsMapFinished), rounds, IsFirstMapFlag);
}

public Action:Command_Current(client, args)
{
	if (GetPlayerWeaponSlot(client, 0) != -1)
	{
		decl String:CurrentWeapon[64];
		GetWeaponNameAtSlot(client, 0, CurrentWeapon, 63);
		PrintToChat(client, "\x05Primary Weapon = %s", CurrentWeapon);
	}
}

public Action:Command_UnSaved(client, args)
{
	PrintToChat(client, "SavedWeapons = %d", SavedWeapons[client]);
	SavedWeapons[client] = false;
}

public KIFOnMapStart()
{
	if (GetConVarInt(hardmod_KIF) < 1) return;
	if (!IsFirstMapFlag)
	{
		for (new j = 1; j <= L4D_MAXPLAYERS; j++)
		{
			ClearPlayerWeapons(j);
		}
	}
}

ClearPlayerWeapons(client)
{
	if (!IsValidClient(client)) return;
	if (IsClientConnected(client) && IsClientInGame(client))
	{
		if (GetClientTeam(client) == TEAM_SURVIVORS && IsPlayerAlive(client))
		{
			for (new i = 0; i < 5; i++)
			{
				if (GetPlayerWeaponSlot(client, i) > -1)
				{
					RemovePlayerItem(client, GetPlayerWeaponSlot(client, i));
				}
			}
			CheatCMD(client, "give", "pistol");
		}
	}
}

KIFRoundFreezeEndEvent()
{
	if (IsFirstMapFlag)
	{
		for (new j = 1; j <= L4D_MAXPLAYERS; j++)
		{
			if (IsClientConnected(j) && IsClientInGame(j))
			{
				if (GetClientTeam(j) != TEAM_INFECTED)
				{
					ClearClientData(j);
				}
			}
		}
	}
	else if (rounds > 2)
	{
		for (new j = 1; j <= L4D_MAXPLAYERS; j++)
		{
			if (!SavedWeapons[j])
			{
				if (IsClientConnected(j) && IsClientInGame(j)  && !IsFakeClient(j))
				{
					if (GetClientTeam(j) != TEAM_INFECTED)
					{
						ClearClientData(j);
					}
				}
			}
		}
	}
}

public KIFMapTransitionEvent()
{
	is_first_map_count++;
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i) && IsClientInGame(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i) && !IsFakeClient(i))
			{
				do_not_clear[i] = 1;
				SaveClientData(i);
			}
			else
			{
				do_not_clear[i] = 0;
				SavedWeapons[i] = false;
				ClearClientData(i);
			}
		}
		else
		{
			do_not_clear[i] = 0;
			SavedWeapons[i] = false;
			ClearClientData(i);
		}
	}
}

KIFPlayerTransitionEvent(Handle:event)
{
	if (GetConVarInt(hardmod_KIF) != 1) return;
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	for (new i = 0; i < 5; i++) if (GetPlayerWeaponSlot(client, i) > -1) RemovePlayerItem(client, GetPlayerWeaponSlot(client, i));
	CheatCMD(client, "give", "pistol");
}

public KIFRoundStartPostNavEvent()
{
//	do_not_clear = false;
}

public KIFOnClientDisconnect(client)
{
	if (!IsClientInGame(client) || IsFakeClient(client) || GetClientTeam(client) != TEAM_SURVIVORS) return;
	if (do_not_clear[client] > 0) do_not_clear[client]--;
	else
	{
		ClearClientData(client);
		SavedWeapons[client] = false;
	}
}

public Action:TimedKIFPlayerSpawnCheckHP(Handle:timer, any:client)
{
	if (GetConVarInt(hardmod_KIF) == 1 && IsValidClient(client)) PlayerSpawnCheckHP(client);
}

KIFPlayerSpawn(Handle:event)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	if (GetClientTeam(client) != TEAM_SURVIVORS) return;
	if (DefibUsedTime[client] + 3 >= GetTime()) return;
	ClearPlayerWeapons(client);
	CheatCMD(client, "give", "health");
	SetEntityHealth(client, 50);
	PlayerSpawnCheckHP(client);
	CreateTimer(0.1, TimedKIFPlayerSpawnCheckHP, client);
	SetEntProp(client, Prop_Send, "m_isGoingToDie", 0);
	SetEntProp(client, Prop_Send, "m_currentReviveCount", 0);
}

PlayerSpawnCheckHP(client)
{
	if (CheckGameMode("mutation3")) SwitchHealth(client);
	else if (GetHealth(client) > 1) RemoveTempHealth(client);
}

KIFDefibrillatorUsed(Handle:event)
{
	new client = GetClientOfUserId(GetEventInt(event, "subject"));
	DefibUsedTime[client] = GetTime();
}

public Action:Event_PTRoundStart(Handle:event, const String:name[], bool:dontBroadcast)
{
	if (GetConVarInt(hardmod_KIF) < 1) return;
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i))
		{
			if (IsClientConnected(i) && IsClientInGame(i))
			{
				if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
				{
					CreateTimer(0.5, RetryRestore, i);
	//				RestoreClientData(i);
				}
			}
		}
	}
}

public Action:RetryRestore(Handle:timer, any:client)
{
	if (!IsClientConnected(client) || !IsClientInGame(client) || GetClientTeam(client) != TEAM_SURVIVORS || !IsPlayerAlive(client)) return;
	RestoreClientData(client);
}

public Action:CycleRetryRestore(Handle:timer, any:client)
{
	if (!IsClientConnected(client) || !IsClientInGame(client)) return;
	if (GetClientTeam(client) != TEAM_SURVIVORS || !IsPlayerAlive(client))
	{
		CreateTimer(0.5, CycleRetryRestore, client);
		return;
	}
	RestoreClientData(client);
}

public RestoreClientData(client)
{
	if (!IsClientInGame(client) || GetClientTeam(client) != TEAM_SURVIVORS) return;
	for (new i = 0; i < 5; i++) if (GetPlayerWeaponSlot(client, i) > -1) RemovePlayerItem(client, GetPlayerWeaponSlot(client, i));
	if (GetConVarInt(hardmod_KIF_auth_check) > 0)
	{
		new String:clientauth[MAX_STEAM_LENGTH];
		GetClientAuthString(client, clientauth, sizeof(clientauth));
		if (!StrEqual(slotAuth[client], clientauth, false)) ClearClientData(client);
	}
	if (IsFakeClient(client))
	{
		CheatCMD(client, "give", "pistol");
		CheatCMD(client, "give", "health");
		SetEntProp(client, Prop_Send, "m_iHealth", 75, 1);
		RemoveTempHealth(client);
		if (CheckGameMode("mutation3")) SwitchHealth(client);
		return;
	}
	if (!StrEqual(slotPrimary[client], "", false))
	{
		CheatCMD(client, "give", slotPrimary[client]);
		//SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_iExtraPrimaryAmmo", priAmmo[client]);
		SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_bInReload", priAmmo[client]);
		SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_iClip1", priClip[client]);
		SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_upgradeBitVec", priUpgrade[client]);
		SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_nUpgradedPrimaryAmmoLoaded", priUpgrAmmo[client]);
	}
	if (StrEqual(slotSecondary[client], "", false)) CheatCMD(client, "give", "pistol");
	else if (StrEqual(slotSecondary[client], "dualpistol", false))
	{
		CheatCMD(client, "give", "pistol");
		CheatCMD(client, "give", "pistol");
	}
	else CheatCMD(client, "give", slotSecondary[client]);
	if (!StrEqual(slotThrowable[client], "", false)) CheatCMD(client, "give", slotThrowable[client]);
	if (!StrEqual(slotMedkit[client], "", false)) CheatCMD(client, "give", slotMedkit[client]);
	if (!StrEqual(slotPills[client], "", false)) CheatCMD(client, "give", slotPills[client]);
	CheatCMD(client, "give", "health");
	if (slotHealth[client] < 50) SetEntProp(client, Prop_Send, "m_iHealth", 50, 1);
	else SetEntProp(client, Prop_Send, "m_iHealth", slotHealth[client], 1);
	RemoveTempHealth(client);
	if (CheckGameMode("mutation3")) SwitchHealth(client);
}

public SaveClients()
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++) SaveClientData(i);
}

public SaveClientData(client)
{
	if (!IsClientInGame(client) || GetClientTeam(client) != TEAM_SURVIVORS || IsFakeClient(client))	return;
	if (GetConVarInt(hardmod_KIF_auth_check) > 0)
	{
		new String:clientauth[MAX_STEAM_LENGTH];
		GetClientAuthString(client, clientauth, sizeof(clientauth));
		slotAuth[client] = clientauth;
	}
	if (GetPlayerWeaponSlot(client, 0) == -1) slotPrimary[client] = "";
	else
	{
		GetWeaponNameAtSlot(client, 0, slotPrimary[client], MAX_LINE_WIDTH);
		if (slotPrimary[client][0] != 0)
		{
			priAmmo[client] = GetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_iExtraPrimaryAmmo");
			//priAmmo[client] = GetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_iClip2");
			priClip[client] = GetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_iClip1");
			priUpgrade[client] = GetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_upgradeBitVec");
			priUpgrAmmo[client] = GetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_nUpgradedPrimaryAmmoLoaded");
			SavedWeapons[client] = true;
		}
	}
	if (GetPlayerWeaponSlot(client, 1) == -1) slotSecondary[client] = "pistol";
	else
	{
		decl String:modelname[128];
		GetEntPropString(GetPlayerWeaponSlot(client, 1), Prop_Data, "m_ModelName", modelname, 128);
		if (StrEqual(modelname, "models/weapons/melee/v_fireaxe.mdl", false)) slotSecondary[client] = "fireaxe";
		else if (StrEqual(modelname, "models/weapons/melee/v_crowbar.mdl", false)) slotSecondary[client] = "crowbar";
		else if (StrEqual(modelname, "models/weapons/melee/v_cricket_bat.mdl", false)) slotSecondary[client] = "cricket_bat";
		else if (StrEqual(modelname, "models/weapons/melee/v_katana.mdl", false)) slotSecondary[client] = "katana";
		else if (StrEqual(modelname, "models/weapons/melee/v_bat.mdl", false)) slotSecondary[client] = "baseball_bat";
		else if (StrEqual(modelname, "models/v_models/v_knife_t.mdl", false)) slotSecondary[client] = "knife";
		else if (StrEqual(modelname, "models/weapons/melee/v_electric_guitar.mdl", false)) slotSecondary[client] = "electric_guitar";
		else if (StrEqual(modelname, "models/weapons/melee/v_frying_pan.mdl", false)) slotSecondary[client] = "frying_pan";
		else if (StrEqual(modelname, "models/weapons/melee/v_machete.mdl", false)) slotSecondary[client] = "machete";
		else if (StrEqual(modelname, "models/weapons/melee/v_golfclub.mdl", false)) slotSecondary[client] = "golfclub";
		else if (StrEqual(modelname, "models/weapons/melee/v_tonfa.mdl", false)) slotSecondary[client] = "tonfa";
		else if (StrEqual(modelname, "models/weapons/melee/v_riotshield.mdl", false)) slotSecondary[client] = "riotshield";
		else if (StrEqual(modelname, "models/v_models/v_dual_pistolA.mdl", false)) slotSecondary[client] = "dualpistol";
		else
		{
//			PrintToChatAll("\x03ModelName = %s", modelname);
			GetEdictClassname(GetPlayerWeaponSlot(client, 1), slotSecondary[client], MAX_LINE_WIDTH);
		}
	}
	if (GetPlayerWeaponSlot(client, 2) == -1) slotThrowable[client] = "";
	else GetWeaponNameAtSlot(client, 2, slotThrowable[client], MAX_LINE_WIDTH);
	if (GetPlayerWeaponSlot(client, 3) == -1) slotMedkit[client] = "";
	else GetWeaponNameAtSlot(client, 3, slotMedkit[client], MAX_LINE_WIDTH);
	if (GetPlayerWeaponSlot(client, 4) == -1) slotPills[client] = "";
	else GetWeaponNameAtSlot(client, 4, slotPills[client], MAX_LINE_WIDTH);
	slotHealth[client] = GetClientHealth(client);
	if (slotHealth[client] < 50) slotHealth[client] = 50;
}

public ClearClientData(client)
{
	if (!IsClientInGame(client) || GetClientTeam(client) != TEAM_SURVIVORS || IsFakeClient(client)) return;
	slotPrimary[client] = "";
	if (IsFirstMapFlag || rounds > 2)
	{
		if (GetRandomInt(1, 5) > 1) slotSecondary[client] = "pistol";
		else
		{
			if (GetRandomInt(1, 2) == 1) slotSecondary[client] = "pistol_magnum";
			else slotSecondary[client] = "chainsaw";
		}
	}
	else slotSecondary[client] = "pistol";
	slotThrowable[client] = "";
	if (IsFirstMapFlag || rounds > 2)
	{
		switch (GetRandomInt(1, 10))
		{
			case 1:
			{
				slotMedkit[client] = "first_aid_kit";
				slotPills[client] = "";
				slotHealth[client] = 50;
			}
			case 2:
			{
				slotMedkit[client] = "defibrillator";
				slotPills[client] = "";
				slotHealth[client] = 75;
			}
			case 3: 
			{
				slotMedkit[client] = "";
				slotPills[client] = "pain_pills";
				slotHealth[client] = 40;
			}
			case 4:
			{
				slotMedkit[client] = "";
				slotPills[client] = "adrenaline";
				slotHealth[client] = 60;
			}
			case 5:
			{
				slotMedkit[client] = "";
				slotPills[client] = "";
				slotHealth[client] = 100;
			}
			case 6:
			{
				slotMedkit[client] = "upgradepack_explosive";
				slotPills[client] = "";
				slotHealth[client] = 60;
			}
			case 7:
			{
				slotMedkit[client] = "upgradepack_incendiary";
				slotPills[client] = "";
				slotHealth[client] = 55;
			}
			case 8:
			{
				slotMedkit[client] = "molotov";
				slotPills[client] = "";
				slotHealth[client] = 65;
			}
			case 9:
			{
				slotMedkit[client] = "vomitjar";
				slotPills[client] = "";
				slotHealth[client] = 65;
			}
			case 10:
			{
				slotMedkit[client] = "pipe_bomb";
				slotPills[client] = "";
				slotHealth[client] = 65;
			}
		}
	}
	else
	{
		slotMedkit[client] = "";
		slotPills[client] = "";
		slotHealth[client] = 50;
	}
}

public FindSteamID(String:SteamID[])
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++) if (StrEqual(playerIds[i], SteamID)) return i;
	return -1;
}

public GetWeaponNameAtSlot(client, slot, String:weaponName[], maxlen)
{
	new wclient = GetPlayerWeaponSlot(client, slot);
	if (wclient < 0)
	{
		weaponName[0] = 0;
		return;
	}
	GetEdictClassname(wclient, weaponName, maxlen);
}

stock IsFirstMap()
{
	if (StrEqual(current_map, "c1m1_hotel", false)
	|| StrEqual(current_map, "c2m1_highway", false)
	|| StrEqual(current_map, "c3m1_plankcountry", false)
	|| StrEqual(current_map, "c4m1_milltown_a", false)
	|| StrEqual(current_map, "c5m1_waterfront", false)
	|| StrEqual(current_map, "c6m1_riverbank", false)
	|| StrEqual(current_map, "c7m1_docks", false)
	|| StrEqual(current_map, "c8m1_apartment", false)
	|| StrEqual(current_map, "c9m1_alleys", false)
	|| StrEqual(current_map, "c10m1_caves", false)
	|| StrEqual(current_map, "c11m1_greenhouse", false)
	|| StrEqual(current_map, "c12m1_hilltop", false)
	|| StrEqual(current_map, "c13m1_alpinecreek", false)
	|| StrEqual(current_map, "bloodtracks_01", false)
	|| StrEqual(current_map, "l4d2_darkblood01_tanker", false)
	|| StrEqual(current_map, "l4d_dbd2dc_anna_is_gone", false)
	|| StrEqual(current_map, "cdta_01detour", false)
	|| StrEqual(current_map, "l4d_ihm01_forest", false)
	|| StrEqual(current_map, "l4d2_diescraper1_apartment_31", false)
	|| StrEqual(current_map, "l4d_149_1", false)
	|| StrEqual(current_map, "gr-mapone-7", false)
	|| StrEqual(current_map, "qe_1_cliche", false)
	|| StrEqual(current_map, "l4d2_stadium1_apartment", false)
	|| StrEqual(current_map, "eu01_residential_b09", false)
	|| StrEqual(current_map, "wth_1", false)
	|| StrEqual(current_map, "2ee_01", false)
	|| StrEqual(current_map, "l4d2_city17_01", false)
	|| StrEqual(current_map, "l4d_deathaboard01_prison", false)
	|| StrEqual(current_map, "cwm1_intro", false)
	|| StrEqual(current_map, "2ee_01_deadlybeggining", false)
	|| StrEqual(current_map, "l4d_orange01_first", false)
	|| StrEqual(current_map, "hf01_theforest", false)
	|| StrEqual(current_map, "l4d2_deadcity01_riverside", false)
	|| StrEqual(current_map, "tutorial01", false)
	|| StrEqual(current_map, "tutorial_standards", false)
	|| StrEqual(current_map, "srocchurch", false)
	|| StrEqual(current_map, "l4d2_ravenholmwar_1", false))
	{
		return true;
	}
	return false;
}