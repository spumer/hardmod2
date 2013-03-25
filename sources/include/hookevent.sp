#define SPECTATORTEAM 1
#define SURVIVORTEAM 2
#define INFECTEDTEAM 3
#define ZOMBIECLASS_SMOKER 1
#define ZOMBIECLASS_BOOMER 2
#define ZOMBIECLASS_HUNTER 3
#define ZOMBIECLASS_SPITTER 4
#define ZOMBIECLASS_JOCKEY 5
#define ZOMBIECLASS_CHARGER 6
#define ZOMBIECLASS_WITCH 7
#define ZOMBIECLASS_TANK 8
#define MAX_PLAYERS 32
#define CVAR_FLAGS FCVAR_PLUGIN
#pragma semicolon 1
#include <sourcemod>
#include <sdktools>
#include <adminmenu>

//////////////////////////////////////////
//		Shortcut Commands
//////////////////////////////////////////

public Action:Player_Death(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new giveflags = GetCommandFlags("give");
    new upgradeflags = GetCommandFlags("upgrade_add");
	SetCommandFlags("upgrade_add", upgradeflags|FCVAR_CHEAT);
	SetCommandFlags("give", giveflags|FCVAR_CHEAT);
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	new attacker = GetClientOfUserId(GetEventInt(event, "attacker"));
	GetClientAbsOrigin(client,Float:vOrigin[client]);					// For use with respawning a dead player. Gathers position x,y,z.
	IsPlayerTankHeal[client] = 0;
	if (GetClientTeam(client) == 2)
	{
		SurvivorAliveCount--;
		if (points[client] >= 15 && showRespawn[client] <= 3)
		{
			PrintToChat(client, "\x04[ \x03BUY \x04] \x03You have \x04 %d \x03Point(s).", points[client]);
			if (points[client] >= 20 && IsRespawn[client] < GetConVarInt(RespawnAmount))
			{
				PrintToChat(client, "\x04[ \x03BUY \x04] \x03Type \x04/respawn \x03And come back to life!");
			}
			if (points[client] >= 15 && IsSafeRespawn[client] < GetConVarInt(SafeRespawnAmount))
			{
				PrintToChat(client, "\x04[ \x03BUY \x04] \x03Type \x04/saferespawn \x03And come back to life!");
			}
			showRespawn[client]++;
		}
		points[attacker] += GetConVarInt(IsKilled);
		PrintToChat(attacker, "\x04[ \x03BUY \x04] \x03Survivor Killed: \x04 %d \x03Point(s)", GetConVarInt(IsKilled));
		new SurvivorIsKilled = GetClientOfUserId(GetEventInt(event, "userid"));
		decl String:sBuffer[128];
		Format(sBuffer,sizeof(sBuffer),"Survivor %N has been killed!", SurvivorIsKilled);
		TellAll(sBuffer);
	}
	else if (GetClientTeam(client) == 3)
	{
		if (points[client] >= 3 && showReport[client] <= 3)
		{
			PrintToChat(client, "\x04[ \x03BUY \x04] \x03You have \x04 %d \x03Point(s).", points[client]);
			PrintToChat(client, "\x04[ \x03BUY \x04] \x03Type \x04/up \x03And buy something!");
			showReport[client]++;
		}
		points[attacker] += GetConVarInt(IsKilled);
		killMulti[attacker]++;
		if (killMulti[attacker] >= GetConVarInt(killCount))
		{
			points[attacker] += GetConVarInt(killPoints);
			killMulti[attacker] = 0;
			PrintToChat(attacker, "\x04[ \x03BUY \x04] \x03Infected Kill Multiplier: \x04 %d \x03Point(s)", GetConVarInt(killPoints));
		}
		teamMulti++;
		if (teamMulti >= GetConVarInt(teamCount))
		{
			for (new fuck; fuck <= MaxClients; fuck++)
			{
			if (!IsClientInGame(fuck))
				continue; // client isn't ingame, can't give him points
			if (GetClientTeam(fuck) != SURVIVORTEAM)
				continue; // client isn't survivor, he doesn't derserve points

			points[fuck] += GetConVarInt(teamPoints); // This client is ingame and survivor, give him pointz
			}
			PrintToChatAll("\x04[ \x03BUY \x04] \x03Survivor Teamwork Multiplier: \x04 %d \x03Point(s)", GetConVarInt(teamPoints));
			teamMulti = 0;
		}
		if (GetClientTeam(attacker) == 2)
		{
			points[attacker] += GetConVarInt(IsSpecialInfected);
			PrintToChat(attacker, "\x04[ \x03BUY \x04] \x03Special Infected Killed: \x04 %d \x03Point(s)", GetConVarInt(IsSpecialInfected));
		}
		decl String:victim[64];
		GetEventString(event, "victimname", victim, sizeof(victim));
		if (StrEqual(victim, "Hunter", true))
		{
			HunterActiveAmount--;
			if (HunterActiveAmount < 0) { HunterActiveAmount = 0; }
		}
		else if (StrEqual(victim, "Smoker", true))
		{
			SmokerActiveAmount--;
			if (SmokerActiveAmount < 0) { SmokerActiveAmount = 0; }
			PrintToChatAll("\x04Bot Smoker has died %d", SmokerActiveAmount);
		}
		else if (StrEqual(victim, "Boomer", true))
		{
			BoomerActiveAmount--;
			if (BoomerActiveAmount < 0) { BoomerActiveAmount = 0; }
		}
		else if (StrEqual(victim, "Jockey", true))
		{
			JockeyActiveAmount--;
			if (JockeyActiveAmount < 0) { JockeyActiveAmount = 0; }
		}
		else if (StrEqual(victim, "Charger", true))
		{
			ChargerActiveAmount--;
			if (ChargerActiveAmount < 0) { ChargerActiveAmount = 0; }
		}
		else if (StrEqual(victim, "Spitter", true))
		{
			SpitterActiveAmount--;
			if (SpitterActiveAmount < 0) { SpitterActiveAmount = 0; }
		}
		if (GetConVarInt(Remove_Grenade) == 1)
		{
			new EntCount = GetEntityCount();
			new String:EdictName[128];
			for (new i = 0; i <= EntCount; i++)
			{
				if (IsValidEntity(i))
				{
					GetEdictClassname(i, EdictName, sizeof(EdictName));
					if (StrContains(EdictName, "weapon_grenade", false) != -1)
					{
						AcceptEntityInput(i, "Kill");
						continue;
					}
				}
			}
		}
	}
	SetCommandFlags("upgrade_add", upgradeflags|FCVAR_CHEAT);
	SetCommandFlags("give", giveflags|FCVAR_CHEAT);
	SetCommandFlags("upgrade_add", upgradeflags|FCVAR_CHEAT);
}

public Action:Melee_Kill(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	meleeMulti[client]++;
	if (meleeMulti[client] == GetConVarInt(meleeCount))
	{
		points[client] += GetConVarInt(meleePoints);
		killMulti[client] = 0;
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Melee Kill Multiplier: \x04 %d \x03Point(s)", GetConVarInt(meleePoints));
	}
	teamMulti++;
	if (teamMulti == GetConVarInt(teamCount))
	{
		for (new i; i <= MaxClients; i++)
		{
			if (!IsClientInGame(i))
				continue; // client isn't ingame, can't give him points
			if (GetClientTeam(i) != SURVIVORTEAM)
				continue; // client isn't survivor, he doesn't derserve points

			points[i] += GetConVarInt(teamPoints); // This client is ingame and survivor, give him pointz
		}
		teamMulti = 0;
		PrintToChatAll("\x04[ \x03BUY \x04] \x03Survivor Teamwork Multiplier: \x04 %d \x03Point(s)", GetConVarInt(teamPoints));
	}
	killMulti[client]++;
	if (killMulti[client] == GetConVarInt(killCount))
	{
		points[client] += GetConVarInt(killPoints);
		killMulti[client] = 0;
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Infected Kill Multiplier: \x04 %d \x03Point(s)", GetConVarInt(killPoints));
	}
}

public Action:Defibrillator_Used(Handle:event, String:event_name[], bool:dontBroadcast)
{
	if (SurvivorAliveCount < GetConVarInt(SurvivorStartAmount))
	{
		SurvivorAliveCount++;
	}
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	points[client] += GetConVarInt(IsDefibbed);
	PrintToChat(client, "\x04[ \x03BUY \x04] \x03Used Defibrillator: \x04 %d \x03Point(s)", GetConVarInt(IsDefibbed));
}

public Action:Round_Start(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new EntCount = GetEntityCount();
	new String:EdictName[128];
	for (new i = 0; i <= EntCount; i++)
	{
		if (IsValidEntity(i))
		{
			GetEdictClassname(i, EdictName, sizeof(EdictName));
			if (StrContains(EdictName, "first_aid", false) != -1)
			{
				AcceptEntityInput(i, "Kill");
				continue;
			}
		}
	}
	HunterActiveAmount = 0;
	SmokerActiveAmount = 0;
	BoomerActiveAmount = 0;
	JockeyActiveAmount = 0;
	ChargerActiveAmount = 0;
	SpitterActiveAmount = 0;
	WitchActiveAmount = 0;
	TankActiveAmount = 0;
	TankNotAllowed = 0;
	CheckTanks = 0;
	//teamMulti = 0;
	
	SurvivorAliveCount = GetConVarInt(SurvivorStartAmount);
	
	if (GetConVarInt(ResetRound) == 1)
	{
		for (new i; i <= MaxClients; i++)
		{
			IsPlayerTank[i] = 0;
			IsPlayerHunter[i] = 0;
			IsPlayerSmoker[i] = 0;
			IsPlayerBoomer[i] = 0;
			IsPlayerJockey[i] = 0;
			IsPlayerSpitter[i] = 0;
			IsPlayerCharger[i] = 0;
			MedkitAmount[i] = 0;
			DefibAmount[i] = 0;
			Luck[i] = 0;
			InstantAmount[i] = 0;
			points[i] = 0;
			IsRespawn[i] = 0;
			IsSafeRespawn[i] = 0;
			killMulti[i] = 0;
			hurtMulti[i] = 0;
			primaryMulti[i] = 0;
			secondaryMulti[i] = 0;
			explosiveMulti[i] = 0;
			meleeMulti[i] = 0;
			igniteMulti[i] = 0;
		}
		PrintToChatAll("\x04[ \x03BUY \x04] \x03Points Reset.");
	}
	if (FunAssMode == 1)
	{
		FunAssMode = 0;
		PrintToChatAll("\x03Fun Ass Mode \x04Disabled");
		for (new i;i <= MaxClients;i++)
		{
			points[i] = 0;
		}
	}
	GetCurrentMap(CurrentMap, sizeof(CurrentMap));
	if (GetConVarInt(ResetMap) == 1)
	{
		for (new i; i <= MaxClients; i++)
		{
			points[i] = 0;
		}
		PrintToChatAll("\x04[ \x03BUY \x04] \x03Points Reset.");
	}
	if (GetConVarInt(Remove_Grenade) == 1)
	{
		new EntCount2 = GetEntityCount();
		new String:EdictName2[128];
		for (new i = 0; i <= EntCount2; i++)
		{
			if (IsValidEntity(i))
			{
				GetEdictClassname(i, EdictName2, sizeof(EdictName2));
				if (StrContains(EdictName2, "weapon_grenade", false) != -1)
				{
					AcceptEntityInput(i, "Kill");
					continue;
				}
			}
		}
	}
}

public Action:Round_End(Handle:event, String:event_name[], bool:dontBroadcast)
{
	//new client = GetClientOfUserId(GetEventInt(event, "userid"));
	for (new client;client <= MaxClients;client++)
	{
		points[client] = 0;
	}
	GetCurrentMap(PreviousMap, sizeof(PreviousMap));
	HunterActiveAmount = 0;
	SmokerActiveAmount = 0;
	BoomerActiveAmount = 0;
	JockeyActiveAmount = 0;
	ChargerActiveAmount = 0;
	SpitterActiveAmount = 0;
	WitchActiveAmount = 0;
	TankActiveAmount = 0;
	TankNotAllowed = 0;
	PrintToChatAll("\x04Round Has Ended. \x03Tanks Reset.");
}

public Action:Heal_Success(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	new healed = GetClientOfUserId(GetEventInt(event, "subject"));
	if (client != healed)
	{
		points[client] += GetConVarInt(IsHealed);
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Healed Teammate: \x04 %d \x03Point(s)", GetConVarInt(IsHealed));
	}
}

public Action:Revive_Success(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	new target = GetClientOfUserId(GetEventInt(event, "subject"));
	if (client != target)
	{
		points[client] += GetConVarInt(IsRevived);
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Revived Teammate: \x04 %d \x03Point(s)", GetConVarInt(IsRevived));
	}
}

public Action:Infected_Death(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new giveflags = GetCommandFlags("give");
    new upgradeflags = GetCommandFlags("upgrade_add");
	SetCommandFlags("upgrade_add", upgradeflags|FCVAR_CHEAT);
	SetCommandFlags("give", giveflags|FCVAR_CHEAT);
	new client = GetClientOfUserId(GetEventInt(event, "attacker"));
	new String:weaponused[64];
	GetClientWeapon(client, weaponused, sizeof(weaponused));
   	if (StrEqual(weaponused, "weapon_pistol", true) || StrEqual(weaponused, "weapon_pistol_magnum", true))
   	{
		secondaryMulti[client]++;
		if (secondaryMulti[client] == GetConVarInt(secondaryCount))
		{
			points[client] += GetConVarInt(secondaryPoints);
			secondaryMulti[client] = 0;
			PrintToChat(client, "\x04[ \x03BUY \x04] \x03Secondary Multiplier: \x04 %d \x03Point(s)", GetConVarInt(secondaryPoints));
		}
	}
	else if (StrEqual(weaponused, "weapon_molotov", true) || StrEqual(weaponused, "weapon_pipe_bomb", true))
	{
		explosiveMulti[client]++;
		if (explosiveMulti[client] == GetConVarInt(explosiveCount))
		{
			points[client] += GetConVarInt(explosivePoints);
			explosiveMulti[client] = 0;
			PrintToChat(client, "\x04[ \x03BUY \x04] \x03Explosive Multiplier: \x04 %d \x03Point(s)", GetConVarInt(explosivePoints));
		}
	}
	else
	{
		primaryMulti[client]++;
		if (primaryMulti[client] >= GetConVarInt(primaryCount))
		{
			points[client] += GetConVarInt(primaryPoints);
			primaryMulti[client] = 0;
			PrintToChat(client, "\x04[ \x03BUY \x04] \x03Primary Multiplier: \x04 %d \x03Point(s)", GetConVarInt(primaryPoints));
		}
	}
	teamMulti++;
	if (teamMulti == GetConVarInt(teamCount))
	{
		teamMulti = 0;
		for (new i; i <= MaxClients; i++)
		{
			if (!IsClientInGame(i))
				continue; // client isn't ingame, can't give him points
			if (GetClientTeam(i) != SURVIVORTEAM)
				continue; // client isn't survivor, he doesn't derserve points
					
			points[i] += GetConVarInt(teamPoints); // This client is ingame and survivor, give him pointz
		}
		PrintToChatAll("\x04[ \x03BUY \x04] \x03Survivor Teamwork Multiplier: \x04 %d \x03Point(s)", GetConVarInt(teamPoints));
	}
	killMulti[client]++;
	if (killMulti[client] == GetConVarInt(killCount))
	{
		points[client] += GetConVarInt(killPoints);
		killMulti[client] = 0;
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Infected Kill Multiplier: \x04 %d \x03Point(s)", GetConVarInt(killPoints));
	}
	if (GetClientTeam(client) != INFECTEDTEAM)
	{
		CommonMulti[client]++;
	}
	if (CommonMulti[client] == GetConVarInt(IsCommonCount))
	{
		points[client] += GetConVarInt(IsCommonPoints);
		CommonMulti[client] = 0;
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Common Kill Multiplier: \x04 %d \x03Point(s)", GetConVarInt(IsCommonPoints));
	}
	decl String:victim2[64];
	GetEventString(event, "gender", victim2, sizeof(victim2));
	if (StrEqual(victim2, "Hunter", true))
	{
		HunterActiveAmount--;
		if (HunterActiveAmount < 0) { HunterActiveAmount = 0; }
	}
	else if (StrEqual(victim2, "Smoker", true))
	{
		SmokerActiveAmount--;
		if (SmokerActiveAmount < 0) { SmokerActiveAmount = 0; }
		PrintToChatAll("\x04Bot Smoker has died %d", SmokerActiveAmount);
	}
	else if (StrEqual(victim2, "Boomer", true))
	{
		BoomerActiveAmount--;
		if (BoomerActiveAmount < 0) { BoomerActiveAmount = 0; }
	}
	else if (StrEqual(victim2, "Jockey", true))
	{
		JockeyActiveAmount--;
		if (JockeyActiveAmount < 0) { JockeyActiveAmount = 0; }
	}
	else if (StrEqual(victim2, "Charger", true))
	{
		ChargerActiveAmount--;
		if (ChargerActiveAmount < 0) { ChargerActiveAmount = 0; }
	}
	else if (StrEqual(victim2, "Spitter", true))
	{
		SpitterActiveAmount--;
		if (SpitterActiveAmount < 0) { SpitterActiveAmount = 0; }
	}
	else if (StrEqual(victim2, "Tank", true))
	{
		points[client] += GetConVarInt(IsTankInfected);
		PrintToChatAll("\x04[ \x03BUY \x04] \x03Bot Tank Kill: \x04 %d \x03Point(s)", GetConVarInt(IsTankInfected));
		if (TankActiveAmount >= 1)
		{
			TankActiveAmount--;
		}
		TankNotAllowed = 1;
		if (TankActiveAmount < 0)
		{
			TankActiveAmount = 0;
		}
		CreateTimer(120.0,TankHasDiedCount);
	}
	if (GetConVarInt(Remove_Grenade) == 1)
	{
		new EntCount = GetEntityCount();
		new String:EdictName[128];
		for (new i = 0; i <= EntCount; i++)
		{
			if (IsValidEntity(i))
			{
				GetEdictClassname(i, EdictName, sizeof(EdictName));
				if (StrContains(EdictName, "weapon_grenade", false) != -1)
				{
					AcceptEntityInput(i, "Kill");
					continue;
				}
			}
		}
	}
	SetCommandFlags("upgrade_add", upgradeflags|FCVAR_CHEAT);
	SetCommandFlags("give", giveflags|FCVAR_CHEAT);
	SetCommandFlags("upgrade_add", upgradeflags|FCVAR_CHEAT);
}

public Action:Witch_Spawn(Handle:event, String:event_name[], bool:dontBroadcast)
{
	WitchActiveAmount++;
	PrintToChatAll("\x04[ \x03BUY \x04] \x03A Witch has appeared...");
}

public Action:Witch_Killed(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	points[client] += GetConVarInt(IsWitchInfected);
	PrintToChat(client, "\x04[ \x03BUY \x04] \x03Witch Killed: \x04 %d \x03Point(s)", GetConVarInt(IsWitchInfected));
	if (WitchActiveAmount >= 1)
	{
		WitchActiveAmount--;
	}
	/*
	teamMulti++;
	if (teamMulti == GetConVarInt(teamCount))
	{
		for (new i; i <= MaxClients; i++)
		{
			if (!IsClientInGame(i))
				continue; // client isn't ingame, can't give him points
			if (GetClientTeam(i) != SURVIVORTEAM)
				continue; // client isn't survivor, he doesn't derserve points

			points[i] += GetConVarInt(teamPoints); // This client is ingame and survivor, give him pointz
		}
		teamMulti = 0;
		PrintToChatAll("\x04[ \x03BUY \x04] \x03Survivor Teamwork Multiplier: \x04 %d \x03Point(s)", GetConVarInt(teamPoints));
	}
	*/
	killMulti[client]++;
	if (killMulti[client] == GetConVarInt(killCount))
	{
		points[client] += GetConVarInt(killPoints);
		killMulti[client] = 0;
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Infected Kill Multiplier: \x04 %d \x03Point(s)", GetConVarInt(killPoints));
	}
}

public Action:Zombie_Ignited(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new attacker = GetClientOfUserId(GetEventInt(event, "userid"));
	decl String:victim[64];
	GetEventString(event, "victimname", victim, sizeof(victim));
	if (StrEqual(victim, "Tank", true))
	{
		points[attacker] += GetConVarInt(IsTankOnFire);
		PrintToChat(attacker, "\x04[ \x03BUY \x04] \x03Tank On Fire: \x04 %d \x03Point(s)", GetConVarInt(IsTankOnFire));
	}
	else if (StrEqual(victim, "Infected", true))
	{
		if (IsUncommonActive == 1 || IsJimmyActive == 1 || IsBoomerSquadActive == 1)
		{
			igniteMulti[attacker]++;
			if (igniteMulti[attacker] == GetConVarInt(igniteCount))
			{
				points[attacker] += GetConVarInt(ignitePoints);
				igniteMulti[attacker] = 0;
				PrintToChat(attacker, "\x04[ \x03BUY \x04] \x03Ignite Multiplier: \x04 %d \x03Point(s)", GetConVarInt(ignitePoints));
			}
		}
	}
}

public Action:Player_Hurt(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	new attacker = GetClientOfUserId(GetEventInt(event, "attacker"));
	if (GetClientTeam(attacker) == 3 && GetClientTeam(client) == 2)
	{
		hurtMulti[attacker]++;
		if (hurtMulti[attacker] == GetConVarInt(hurtCount))
		{
			points[attacker] += GetConVarInt(hurtPoints);
			PrintToChat(attacker, "\x04[ \x03BUY \x04] \x03Hurt Survivor Multiplier: \x04 %d \x03Point(s)", GetConVarInt(hurtPoints));
			hurtMulti[attacker] = 0;
		}
	}
	decl infectedclass;
	infectedclass = GetEntProp(client,Prop_Send,"m_zombieClass");
	if (infectedclass == ZOMBIECLASS_TANK)
	{
		IsPlayerTank[client] = 1;
	}
}

public Action:Player_Incapacitated(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	new attacker = GetClientOfUserId(GetEventInt(event, "attacker"));
	if (GetClientTeam(attacker) == 3 && GetClientTeam(client) == 2)
	{
		points[attacker] += GetConVarInt(IsIncap);
		PrintToChat(attacker, "\x04[ \x03BUY \x04] \x03Incapacitated Survivor: \x04 %d \x03Point(s)", GetConVarInt(IsIncap));
	}
	new zombieclass = GetEntProp(client, Prop_Send, "m_zombieClass");
	if (zombieclass == 8)
	{
		points[client] += GetConVarInt(IsTankInfected);
		if (TankActiveAmount >= 1)
		{
			TankActiveAmount--;
		}
		PrintToChatAll("HUMAN TANKS ACTIVE: %d", TankActiveAmount);
		TankNotAllowed = 1;
		if (TankActiveAmount < 0)
		{
			TankActiveAmount = 0;
		}
		CreateTimer(120.0, TankHasDiedCount);
	}
	if (IsPlayerTank[client] == 1)
	{
		if (TankActiveAmount >= GetConVarInt(TankActiveLimit))
		{
			if (TankActiveAmount >= 1)
			{
				TankActiveAmount--;
			}
			IsPlayerTank[client] = 0;
			if (TankActiveAmount < 0)
			{
				TankActiveAmount = 0;
			}
		}
	}
}

public Action:Lunge_Pounce(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	points[client] += GetConVarInt(PouncePoints);
	PrintToChat(client, "\x04[ \x03BUY \x04] \x03Pounced Survivor: \x04 %d \x03Point(s)", GetConVarInt(PouncePoints));
}

public Action:Tongue_Grab(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	points[client] += GetConVarInt(GrabPoints);
	PrintToChat(client, "\x04[ \x03BUY \x04] \x03Grabbed Survivor: \x04 %d \x03Point(s)", GetConVarInt(GrabPoints));
}

public Action:Choke_Start(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	points[client] += GetConVarInt(ChokePoints);
	PrintToChat(client, "\x04[ \x03BUY \x04] \x03Choked Survivor: \x04 %d \x03Point(s)", GetConVarInt(ChokePoints));
}

public Action:Player_Now_It(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "attacker"));
	points[client] += GetConVarInt(BoomPoints);
	PrintToChat(client, "\x04[ \x03BUY \x04] \x03Boomed Survivor: \x04 %d \x03Point(s)", GetConVarInt(BoomPoints));
}

public Action:Fatal_Vomit(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "attacker"));
	new victim = GetClientOfUserId(GetEventInt(event, "userid"));
	if (GetClientTeam(victim) == 2)
	{
		points[client] += GetConVarInt(FatalPoints);
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Incap/Kill'd Biled Survivor: \x04 %d \x03Point(s)", GetConVarInt(FatalPoints));
	}
}

public Action:Jockey_Ride(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	points[client] += GetConVarInt(RidePoints);
	PrintToChat(client, "\x04[ \x03BUY \x04] \x03Riding Survivor: \x04 %d \x03Point(s)", GetConVarInt(RidePoints));
}

public Action:Jockey_Ride_End(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	new iPointsRide = GetConVarInt(RideTime);
	new iPointsEarned = RoundFloat(GetEventFloat(event, "ride_length"));
	if ((((iPointsRide*iPointsEarned)/2)/2) < 1)
	{
		points[client]++;
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Ride Multiplier: \x04 1 \x03Point");
	}
	else
	{
		new iPointsTotal = ((iPointsRide*iPointsEarned)/2)/2;
		points[client] += iPointsTotal;
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Ride Multiplier: \x04 %d \x03Point(s)", iPointsTotal);
	}
}

public Action:Charger_Carry_Start(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	points[client] += GetConVarInt(CarryPoints);
	PrintToChat(client, "\x04[ \x03BUY \x04] \x03Carry Survivor: \x04 %d \x03Point(s)", GetConVarInt(CarryPoints));
}

public Action:Charger_Pummel_Start(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	points[client] += GetConVarInt(PummelPoints);
	PrintToChat(client, "\x04[ \x03BUY \x04] \x03Pummeling Survivor: \x04 %d \x03Point(s)", GetConVarInt(PummelPoints));
}

public Action:Charger_Impact(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	points[client] += GetConVarInt(ImpactPoints);
	PrintToChat(client, "\x04[ \x03BUY \x04] \x03Impacted Survivor: \x04 %d \x03Point(s)", GetConVarInt(ImpactPoints));
}

public Action:Player_Team(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	new disconnect = GetEventBool(event, "disconnect");
	if (GetConVarInt(ResetTeam) >= 1)
	{
		if (disconnect)
		{
			points[client] = 0;
		}
		if (points[client] != 0)
		{
			PrintToChat(client,"\x04[ \x03BUY \x04] \x03Switched Teams. Points Reset.");
			points[client] = 0;
		}
	}
}