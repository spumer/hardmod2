#pragma semicolon 1
#include <sourcemod>
#define PLUGIN_VERSION "1.1.7j"

#define ZC_SMOKER 1
#define ZC_BOOMER 2
#define ZC_HUNTER 3
#define ZC_SPITTER 4
#define ZC_JOCKEY 5
#define ZC_CHARGER 6
#define ZC_TANK 8

#define L4D_MAXPLAYERS 32

public Plugin:myinfo =
{
	name = "[L4D2] Monster Bots",
	author = "Machine",
	description = "Automated Special Infected Creator",
	version = PLUGIN_VERSION,
	url = "www.AlliedMods.net"
};

//////////////////////////////////////////////////////////
//Monster Types
//
//0 = Random (Smoker,Boomer,Hunter,Spitter,Jockey,Charger)
//1 = Smoker
//2 = Boomer
//3 = Hunter
//4 = Spitter
//5 = Jockey
//6 = Charger
//7 = Witch
//8 = Tank
//////////////////////////////////////////////////////////

new Handle:monstermaxbots;
new Handle:monstertype;
new Handle:monsterbotson;
new Handle:monsterinterval;
new Handle:monsternodirector;
new timertick;

public OnPluginStart()
{
	CreateConVar("monsterbots_version", PLUGIN_VERSION, "L4D2 Monster Bots Version", FCVAR_PLUGIN|FCVAR_DONTRECORD);
	monstermaxbots = CreateConVar("monsterbots_maxbots", "5", "The maximum amount of monster bots", FCVAR_PLUGIN, true, 0.0);
	monstertype = CreateConVar("monsterbots_type", "9", "The type of monsters", FCVAR_PLUGIN, true, 0.0);
	monsterbotson = CreateConVar("monsterbots_on", "1", "Is monster bots on?", FCVAR_PLUGIN, true, 0.0);
	monsterinterval = CreateConVar("monsterbots_interval", "13", "How many seconds till another monster spawns", FCVAR_PLUGIN, true, 0.0);
	monsternodirector = CreateConVar("monsterbots_nodirector", "0", "Shutdown the director?", FCVAR_PLUGIN, true, 0.0);

	HookEvent("round_start", Event_RoundStart);
	HookEvent("round_end", Event_RoundEnd);
	HookConVarChange(monsterbotson, MonsterBots_Switch);

	CreateTimer(1.0, TimerUpdate, _, TIMER_REPEAT);
}

public Action:Event_RoundStart(Handle:event, const String:name[], bool:dontBroadcast)
{
	new flags = GetConVarFlags(FindConVar("z_max_player_zombies"));
	SetConVarBounds(FindConVar("z_max_player_zombies"), ConVarBound_Upper, false);
	SetConVarFlags(FindConVar("z_max_player_zombies"), flags & ~FCVAR_NOTIFY);
}

public Action:Event_RoundEnd(Handle:event, const String:name[], bool:dontBroadcast)
{
	for (new i=1; i<=MaxClients; i++)
	{
		if (IsClientInGame(i))
		{
			if (GetClientTeam(i) == 3)
			{
				if (IsFakeClient(i))
				{
					KickClient(i);
				}
			}
		}
	}
}

public Action:TimerUpdate(Handle:timer)
{
	if (!IsServerProcessing())
	{
		return;
	}

	if (GetConVarBool(monsterbotson))
	{
		if (GetConVarBool(monsternodirector))
		{
			new anyclient = GetAnyClient();
			if (anyclient > 0)
			{
				DirectorCommand(anyclient, "director_stop");
			}
			SetConVarInt(FindConVar("director_no_bosses"), 1);
			SetConVarInt(FindConVar("director_no_specials"), 1);
			SetConVarInt(FindConVar("director_no_mobs"), 1);
		}
		SetConVarInt(FindConVar("z_max_player_zombies"), 32);
		
		timertick += 1;
		if (timertick >= GetConVarInt(monsterinterval))
		{
			if (CountMonsters() < GetConVarInt(monstermaxbots))
			{
				new monster = GetConVarInt(monstertype);
				switch(monster)
				{
					case 0:
					{
						new bot = CreateFakeClient("Monster");
						if (bot)
						{
							new random = GetRandomInt(1, 6);
							switch(random)
							{
								case 1:
								SpawnCommand(bot, "z_spawn", "smoker auto");
								case 2:
								SpawnCommand(bot, "z_spawn", "boomer auto");
								case 3:
								SpawnCommand(bot, "z_spawn", "hunter auto");
								case 4:
								SpawnCommand(bot, "z_spawn", "spitter auto");
								case 5:
								SpawnCommand(bot, "z_spawn", "jockey auto");
								case 6:
								SpawnCommand(bot, "z_spawn", "charger auto");
							}
						}
					}
					case 1:
					SpawnCommand(GetAnyClient(), "z_spawn", "smoker auto");
					case 2:
					SpawnCommand(GetAnyClient(), "z_spawn", "boomer auto");
					case 3:
					SpawnCommand(GetAnyClient(), "z_spawn", "hunter auto");
					case 4:
					SpawnCommand(GetAnyClient(), "z_spawn", "spitter auto");
					case 5:
					SpawnCommand(GetAnyClient(), "z_spawn", "jockey auto");
					case 6:
					SpawnCommand(GetAnyClient(), "z_spawn", "charger auto");
					case 7:
					SpawnCommand(GetAnyClient(), "z_spawn", "witch auto");
					case 8:
					SpawnCommand(GetAnyClient(), "z_spawn", "tank auto");
					case 9:
					{
						new bot = CreateFakeClient("Monster");
						if (bot)
						{
							new random = GetRandomInt(1, 3);
							switch(random)
							{
								case 1:
								SpawnCommand(bot, "z_spawn", "smoker auto");
								case 2:
								SpawnCommand(bot, "z_spawn", "boomer auto");
								case 3:
								SpawnCommand(bot, "z_spawn", "hunter auto");
							}
						}
					}
					case 10:
					{
						new bot = CreateFakeClient("Monster");
						if (bot)
						{
							new random = GetRandomInt(1, 13);
							switch(random)
							{
								case 1, 2, 3:
								SpawnCommand(bot, "z_spawn", "boomer auto");
								case 4, 5:
								SpawnCommand(bot, "z_spawn", "smoker auto");
								case 6, 7:
								SpawnCommand(bot, "z_spawn", "hunter auto");
								case 8, 9, 10:
								SpawnCommand(bot, "z_spawn", "spitter auto");
								case 11, 12:
								SpawnCommand(bot, "z_spawn", "jockey auto");
								case 13:
								SpawnCommand(bot, "z_spawn", "charger auto");
							}
						}
					}
					case 11:
					{
						new bot = CreateFakeClient("Monster");
						if (bot)
						{
							new random = GetRandomInt(1, 16);
							switch(random)
							{
								case 1, 2, 3, 4:
								SpawnCommand(bot, "z_spawn", "boomer auto");
								case 5, 6, 7:
								SpawnCommand(bot, "z_spawn", "smoker auto");
								case 8, 9, 10:
								SpawnCommand(bot, "z_spawn", "hunter auto");
								case 11, 12, 13:
								SpawnCommand(bot, "z_spawn", "spitter auto");
								case 14, 15:
								SpawnCommand(bot, "z_spawn", "jockey auto");
								case 16:
								SpawnCommand(bot, "z_spawn", "charger auto");
							}
						}
					}
					case 12:
					{
						new SpawnLimits[10];
						new allowed_monsters = 0;
						SpawnLimits[0] = 0;
						SpawnLimits[ZC_SMOKER] = GetConVarInt(FindConVar("z_smoker_limit"));
						SpawnLimits[ZC_BOOMER] = GetConVarInt(FindConVar("z_boomer_limit"));
						SpawnLimits[ZC_HUNTER] = GetConVarInt(FindConVar("z_hunter_limit"));
						SpawnLimits[ZC_SPITTER] = GetConVarInt(FindConVar("z_spitter_limit"));
						SpawnLimits[ZC_JOCKEY] = GetConVarInt(FindConVar("z_jockey_limit"));
						SpawnLimits[ZC_CHARGER] = GetConVarInt(FindConVar("z_charger_limit"));
						for (new i = 1; i <= L4D_MAXPLAYERS; i++) SpawnLimits[GetClientZC(i)]--;
						for (new i = 1; i < 7; i++)
						{
							if (SpawnLimits[i] < 1) SpawnLimits[i] = 0;
							else allowed_monsters += SpawnLimits[i];
						}
						if (allowed_monsters)
						{
							new lottery = GetRandomInt(1, allowed_monsters);
							new ZOMBIE_WINNER = 0;
							while (lottery > 0)
							{
								ZOMBIE_WINNER++;
								if (lottery >= SpawnLimits[ZOMBIE_WINNER]) lottery -= SpawnLimits[ZOMBIE_WINNER];
								else lottery = 0;
							}
//							PrintToChatAll("ZOMBIE_WINNER = %d", ZOMBIE_WINNER);
							new bot = CreateFakeClient("Monster");
							if (bot > 0) switch (ZOMBIE_WINNER)
							{
								case ZC_SMOKER: SpawnCommand(bot, "z_spawn", "smoker auto");
								case ZC_BOOMER: SpawnCommand(bot, "z_spawn", "boomer auto");
								case ZC_HUNTER: SpawnCommand(bot, "z_spawn", "hunter auto");
								case ZC_SPITTER: SpawnCommand(bot, "z_spawn", "spitter auto");
								case ZC_JOCKEY: SpawnCommand(bot, "z_spawn", "jockey auto");
								case ZC_CHARGER: SpawnCommand(bot, "z_spawn", "charger auto");
							}
//							else PrintToServer("Failed to create fake client");
						}
//						else PrintToChatAll("allowed_monsters = false");
					}
				}
			}
//			else PrintToChatAll("Max zombies reached");
			timertick = 0;
		}
	}
}

public MonsterBots_Switch(Handle:hVariable, const String:strOldValue[], const String:strNewValue[])
{
    	if (GetConVarInt(monsterbotson) == 0) 
	{
		if (GetConVarBool(monsternodirector))
		{
			SetConVarInt(FindConVar("director_no_bosses"), 0);
			SetConVarInt(FindConVar("director_no_specials"), 0);
			SetConVarInt(FindConVar("director_no_mobs"), 0);

			new anyclient = GetAnyClient();
			if (anyclient > 0) DirectorCommand(anyclient, "director_start");
		}
	}
}

public Action:Kickbot(Handle:timer, any:client)
{
	if (IsClientInGame(client))
	{
		if (IsFakeClient(client))
		{
			KickClient(client);
		}
	}
}

stock CountMonsters()
{
	new j = 0;
	new count = 0;
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		j = GetClientZC(i);
		if (j == ZC_SMOKER || j == ZC_BOOMER || j == ZC_HUNTER || j == ZC_SPITTER || j == ZC_JOCKEY || j == ZC_CHARGER || j == ZC_TANK) count++;
	}
	return count;
}

stock GetAnyClient()
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++) if (IsClientInGame(i) && (!IsFakeClient(i))) return i;
	return 0;
}

stock SpawnCommand(client, String:command[], String:arguments[] = "")
{
	if (client)
	{
//		PrintToChatAll("SpawnCommand(%s %s)", command, arguments);
		if (!IsFakeClient(client)) return;
		ChangeClientTeam(client, 3);
		new flags = GetCommandFlags(command);
		SetCommandFlags(command, flags & ~FCVAR_CHEAT);
		FakeClientCommand(client, "%s %s", command, arguments);
		SetCommandFlags(command, flags);
		CreateTimer(0.1, Kickbot, client);
	}
}

stock DirectorCommand(client, String:command[])
{
	if (client)
	{
		new flags = GetCommandFlags(command);
		SetCommandFlags(command, flags & ~FCVAR_CHEAT);
		FakeClientCommand(client, "%s", command);
		SetCommandFlags(command, flags);
	}
}

GetClientZC(client)
{
	if (client < 1 || client > L4D_MAXPLAYERS) return 0;
	if (!IsValidEntity(client) || !IsValidEdict(client)) return 0;
	if (!IsClientInGame(client) || GetClientTeam(client) != 3) return 0;
	return GetEntProp(client, Prop_Send, "m_zombieClass");
}