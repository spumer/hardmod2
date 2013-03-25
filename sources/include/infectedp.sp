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
//		Infected Purchase Functions
//////////////////////////////////////////

public Action:TankBuyFunc(client)
{
	if (GetClientTeam(client) == 3)
	{
		if (TankActiveAmount >= GetConVarInt(TankActiveLimit) && TankNotAllowed < 1)
		{
			PrintToChat(client, "\x04[ \x03BUY \x04] \x03Tank Limit Reached.");
			PrintToChat(client, "Tanks Active: %d", TankActiveAmount);
			PrintToChat(client, "Tanks Active Limit: %d", GetConVarInt(TankActiveLimit));
		}
		else if (TankNotAllowed > 0)
		{
			PrintToChat(client, "\x04[ \x03BUY \x04] \x03Tank has Summoning Sickness.");
			PrintToChat(client, "\x04[ \x03BOY \x04] \x03Expires 120 seconds after death.");
			//PrintToChat(client, "\x04[ \x03BUY \x04] \x03Tank Has Summoning Sickness. \x04 120 \x03Seconds After Death.");
		}
		else if (points[client] >= GetConVarInt(TankCost) && TankNotAllowed < 1)
		{
			new String:RestrictedMap[32];
			RestrictedMap[0] = '\0';
			GetCurrentMap(RestrictedMap, sizeof(RestrictedMap));
	
			if (StrContains(RestrictedMap, "c1m4", true) > -1 || 
			StrContains(RestrictedMap, "c2m5", true) > -1 || 
			StrContains(RestrictedMap, "c3m4", true) > -1 || 
			StrContains(RestrictedMap, "c4m5", true) > -1 || 
			StrContains(RestrictedMap, "c5m5", true) > -1)
			{
				PrintToChat(client, "\x04[ \x03BUY \x04] \x03Cannot buy on this level!");
			}
			else if (TankActiveAmount < GetConVarInt(TankActiveLimit) && TankNotAllowed < 1)
			{
				if (TankNotAllowed < 1)
				{
					new spawnflags = GetCommandFlags("z_spawn");
					SetCommandFlags("z_spawn", spawnflags & ~FCVAR_CHEAT);
					TankActiveAmount++;
					FakeClientCommand(client, "z_spawn tank auto");
					//TankActiveAmount++;
					PrintToChatAll("\x03Tanks Active: %d", TankActiveAmount);
					PrintToChatAll("\x03Tanks allowed: %d", GetConVarInt(TankActiveLimit));
					SetCommandFlags("z_spawn", spawnflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(TankCost);
				}
			}
		}
		else
		{
			PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
		}
	}
	else
	{
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Infected Team Only.");
	}
}

public Action:WitchBuyFunc(client)
{
	new spawnflags = GetCommandFlags("z_spawn");
	SetCommandFlags("z_spawn", spawnflags & ~FCVAR_CHEAT);
	if (GetClientTeam(client) == 3)
	{
		if (WitchActiveAmount >= GetConVarInt(WitchActiveLimit))
		{
			PrintToChat(client, "\x04[ \x03BUY \x04] \x03Witch Limit Reached.");
		}
		else if (points[client] >= GetConVarInt(WitchCost))
		{
			WitchActiveAmount++;
			FakeClientCommand(client, "z_spawn witch auto");
			points[client] -= GetConVarInt(WitchCost);
		}
		else
		{
			PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
		}
	}
	else
	{
		PrintToChat(client, "\x04[ \x03BUY \x04] \x03Infected Team Only.");
	}
	SetCommandFlags("z_spawn", spawnflags|FCVAR_CHEAT);
}