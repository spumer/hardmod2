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

public Action:CostBuy(client, args)
{
	CostBuyFunc(client);
	return Plugin_Handled;
}

public Action:CostBuyFunc(client)
{
	PrintToChat(client, "\x04[ \x03BUY \x04] \x03Primary Tier 1: \x04 %d", GetConVarInt(Primary1Cost));
	PrintToChat(client, "\x04[ \x03BUY \x04] \x03Primary Tier 2: \x04 %d", GetConVarInt(Primary2Cost));
	PrintToChat(client, "\x04[ \x03BUY \x04] \x03Secondary Cost: \x04 %d", GetConVarInt(SecondaryCost));
	PrintToChat(client, "\x04[ \x03BUY \x04] \x03Exposive Costs: \x04 %d", GetConVarInt(ExplosiveCost));
	if (GetConVarInt(MolotovCost) > 0)
	{
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Molotov Cost: \x04 %d", GetConVarInt(MolotovCost));
	}
	if (GetConVarInt(PipebombCost) > 0)
	{
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Pipebomb Cost: \x04 %d", GetConVarInt(PipebombCost));
	}
	if (GetConVarInt(VomitjarCost) > 0)
	{
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Vomitjar Cost: \x04 %d", GetConVarInt(VomitjarCost));
	}
	PrintToChat(client, "\x04[ \x03BUY \x04] \x03Health Item Cost: \x04 %d", GetConVarInt(HealthItemCost));
	PrintToChat(client, "\x04[ \x03BUY \x04] \x03Respawn Cost: \x04 %d", GetConVarInt(RespawnCost));
	PrintToChat(client, "\x04[ \x03BUY \x04] \x03Saferoom Respawn Cost: \x04 %d", GetConVarInt(SafeRespawnCost));
	PrintToChat(client, "\x04[ \x03BUY \x04] \x03Upgrade Item Cost: \x04 %d", GetConVarInt(UpgradeCost));
}

public Action:DeckMeOut(client, args)
{
	DeckMeOutFunc(client);
	return Plugin_Handled;
}

public Action:DeckMeOutFunc(client)
{
	if (FunAssMode == 0)
	{
		FunAssMode = 1;
		PrintToChatAll("\x03Fun Ass Mode \x04Enabled");
		for (new i;i <= MaxClients;i++)
		{
			points[i] = 999;
		}
		PrintToChatAll("\x03All players now have \x04 999 \x03points.");
	}
	else if (FunAssMode == 1)
	{
		FunAssMode = 0;
		PrintToChatAll("\x03Fun Ass Mode \x04Disabled");
		for (new i;i <= MaxClients;i++)
		{
			points[i] = 0;
		}
	}
}

public Action:PotOfGold(client, args)
{
	PotOfGoldFunc(client);
	return Plugin_Handled;
}

public Action:PotOfGoldFunc(client)
{
	if (Luck[client] > 1)
	{
		PrintToChat(client, "\x03You ran out of \x04LUCK");
		return;
	}
	Luck[client]++;
	points[client] += 40;
	PrintToChat(client, "\x04P\x03o\x04t\x03O\x04f\x03G\x04o\x03L\x04d");
}

public Action:HealthBuy(client, args)
{
	HealthBuyFunc(client);
	return Plugin_Handled;
}

public Action:HealthBuyFunc(client)
{
	new giveflags = GetCommandFlags("give");
	SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
	if (GetClientTeam(client) == 2)
	{
		if (InstantAmount[client] >= GetConVarInt(InstantHealLimit) && GetConVarInt(InstantHealLimit) > 0)
		{
			PrintToChat(client, "\x04[ \x03BUY \x04] \x03Insta-Health limit reached!");
		}
		else if (points[client] >= GetConVarInt(HealCost))
		{
			InstantAmount[client]++;
			//Heal player
			FakeClientCommand(client, "give health");
			points[client] -= GetConVarInt(HealCost);
			if (GetConVarInt(InstantHealLimit) > 0)
			{
				PrintToChat(client, "\x04[ \x03BUY \x04] \x03Used \x04 %d \x03/ \x04 %d \x03Insta-Heals.", InstantAmount[client], GetConVarInt(InstantHealLimit));
			}
		}
		else
		{
			PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
		}
	}
	SetCommandFlags("give", giveflags|FCVAR_CHEAT);
}

public Action:AmmoBuy(client, args)
{
	AmmoBuyFunc(client);
	return Plugin_Handled;
}

public Action:AmmoBuyFunc(client)
{
	new giveflags = GetCommandFlags("give");
	SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
	if (points[client] >= GetConVarInt(AmmoCost))
	{
		FakeClientCommand(client, "give ammo");
		points[client] -= GetConVarInt(AmmoCost);
	}
	else
	{
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
	}
	SetCommandFlags("give", giveflags|FCVAR_CHEAT);
}

public Action:AdrenalineBuy(client, args)
{
	AdrenalineBuyFunc(client);
	return Plugin_Handled;
}

public Action:AdrenalineBuyFunc(client)
{
	new giveflags = GetCommandFlags("give");
	SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
	if (GetClientTeam(client) == 2)
	{
		if (points[client] >= GetConVarInt(AdrenalineCost))
		{
			FakeClientCommand(client, "give adrenaline");
			points[client] -= GetConVarInt(AdrenalineCost);
		}
		else
		{
			PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
		}
	}
	SetCommandFlags("give", giveflags|FCVAR_CHEAT);
}

public Action:RespawnBuy(client, args)
{
	RespawnBuyFunc(client);
	return Plugin_Handled;
}

public Action:RespawnBuyFunc(client)
{
	new giveflags = GetCommandFlags("give");
	SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
	new String:RestrictedMap[32];
	RestrictedMap[0] = '\0';
	GetCurrentMap(RestrictedMap, sizeof(RestrictedMap));
	
	if (StrContains(RestrictedMap, "c1m4", true) > -1 || 
		StrContains(RestrictedMap, "c2m5", true) > -1 || 
		StrContains(RestrictedMap, "c3m4", true) > -1 || 
		StrContains(RestrictedMap, "c4m5", true) > -1 || 
		StrContains(RestrictedMap, "c5m5", true) > -1)
	{
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Cannot use on this level!");
	}
	else if (points[client] >= GetConVarInt(RespawnCost) && IsRespawn[client] < GetConVarInt(RespawnAmount) && !IsPlayerAlive(client))
	{
		SDKCall(hRoundRespawn, client);
		TeleportEntity(client, Float:vOrigin[client], NULL_VECTOR, NULL_VECTOR);
		
		if (SurvivorAliveCount < GetConVarInt(SurvivorStartAmount))
		{
			SurvivorAliveCount++;
		}
		IsRespawn[client]++;
		
		FakeClientCommand(client, "give pumpshotgun");
		
		points[client] -= GetConVarInt(RespawnCost);
		InfectedTeamAward();
		PrintToChatAll("\x04[ \x03BUY \x04] \x03A player respawned, Infected are awarded: \x03 %d \x04Point(s).", GetConVarInt(RespawnFee));
	}
	else if (IsRespawn[client] >= GetConVarInt(RespawnAmount))
	{
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Respawn Limit Reached.");
	}
	else if (IsPlayerAlive(client))
	{
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03You are not dead.");
	}
	else
	{
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
	}
	SetCommandFlags("give", giveflags|FCVAR_CHEAT);
	return Plugin_Handled;
}

public Action:SafeRespawnBuy(client, args)
{
	SafeRespawnBuyFunc(client);
	return Plugin_Handled;
}

public Action:SafeRespawnBuyFunc(client)
{
	new giveflags = GetCommandFlags("give");
	SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
	new String:RestrictedMap[32];
	RestrictedMap[0] = '\0';

	GetCurrentMap(RestrictedMap, sizeof(RestrictedMap));
	
	if (StrContains(RestrictedMap, "c1m4", true) > -1 || 
		StrContains(RestrictedMap, "c2m5", true) > -1 || 
		StrContains(RestrictedMap, "c3m4", true) > -1 || 
		StrContains(RestrictedMap, "c4m5", true) > -1 || 
		StrContains(RestrictedMap, "c5m5", true) > -1)
	{
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Cannot use on this level!");
	}
	else if (points[client] >= GetConVarInt(SafeRespawnCost) && IsSafeRespawn[client] < GetConVarInt(SafeRespawnAmount) && !IsPlayerAlive(client))
	{
		SDKCall(hRoundRespawn, client);
		if (SurvivorAliveCount < GetConVarInt(SurvivorStartAmount))
		{
			SurvivorAliveCount++;
		}
		IsSafeRespawn[client]++;
		
		FakeClientCommand(client, "give pumpshotgun");
		
		points[client] -= GetConVarInt(SafeRespawnCost);
		InfectedTeamAward();
		PrintToChatAll("\x04[ \x03BUY \x04] \x03A player respawned, Infected are awarded: \x03 %d \x04Point(s).", GetConVarInt(RespawnFee));
	}
	else if (IsSafeRespawn[client] >= GetConVarInt(SafeRespawnAmount))
	{
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Respawn Limit Reached.");
	}
	else if (IsPlayerAlive(client))
	{
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03You are not dead.");
	}
	else
	{
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
	}
	SetCommandFlags("give", giveflags|FCVAR_CHEAT);
	return Plugin_Handled;
}

public Action:LaserBuy(client,args)
{
	LaserBuyFunc(client);
	return Plugin_Handled;
}

public Action:LaserBuyFunc(client)
{
	new upgradeflags = GetCommandFlags("upgrade_add");
	SetCommandFlags("upgrade_add", upgradeflags & ~FCVAR_CHEAT);
	if (GetClientTeam(client) == 2)
	{
		if (points[client] >= GetConVarInt(UpgradeCost))
		{
			FakeClientCommand(client, "upgrade_add LASER_SIGHT");
			points[client] -= GetConVarInt(UpgradeCost);
		}
		else
		{
			PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
		}
	}
	else
	{
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03You are not a Survivor.");
	}
	SetCommandFlags("upgrade_add", upgradeflags|FCVAR_CHEAT);
}

public Action:PointsChooseMenu(client,args)
{
	if (GetClientTeam(client) == 2)
	{
		SurvivorMenuFunc(client, args);
	}
	else if (GetClientTeam(client) == 3)
	{
		InfectedMenuFunc(client, args);
	}
	return Plugin_Handled;
}

public Action:PanicTooSoonIsActive(Handle:timer)
{
	IsPanicTooSoon = 0;
}

public Action:CommonTooSoonIsActive(Handle:timer)
{
	IsCommonTooSoon = 0;
}

public Action:HowToPlay(client,args)
{
	HowToPlayFunc(client);
	return Plugin_Handled;
}

public Action:HowToPlayFunc(client)
{
	PrintToChat(client, "\x03/up \x04to open the buy menu.");
	PrintToChat(client, "\x03/ammo \x04to purchase ammo.");
	PrintToChat(client, "\x03/haste \x04to purchase adrenaline.");
	PrintToChat(client, "\x03/respawn \x04to return to life.");
	PrintToChat(client, "\x03/saferespawn \x04to return to life at start.");
	PrintToChat(client, "\x03/health \x04to buy health.");
	PrintToChat(client, "\x03/laser \x04to buy laser sight.");
	PrintToChat(client, "\x03/tank \x04to buy a tank.");
	PrintToChat(client, "\x03/witch \x04to buy a witch.");
	PrintToChat(client, "\x03/kill \x04to suicide.");
	PrintToChat(client, "\x04You can buy weapons, items, explosives, etc. through \x03/up.");
	PrintToChat(client, "\x04Experiment to see what you can purchase!");
	PrintToChat(client, "\x04Type \x03/htp \x04to see this menu again.");
	
	return Plugin_Handled;
}

public Action:TankBuy(client,args)
{
	TankBuyFunc(client);
	return Plugin_Handled;
}

public Action:WitchBuy(client,args)
{
	WitchBuyFunc(client);
	return Plugin_Handled;
}

public Action:KillBuy(client,args)
{
	KillBuyFunc(client);
	return Plugin_Handled;
}

public Action:KillBuyFunc(client)
{
	new killflags = GetCommandFlags("kill");
	SetCommandFlags("kill", killflags & ~FCVAR_CHEAT);
	if (client != 0)
	{
		FakeClientCommand(client, "kill");
	}
	SetCommandFlags("kill", killflags|FCVAR_CHEAT);
}

TellAll(String:msg[])
{
	for (new i = 1; i <= MaxClients; i++)
	{
		if (!IsClientInGame(i))
			continue;
		
		if (GetClientTeam(i) > 1)
			PrintHintText(i, msg);
	} 	
}