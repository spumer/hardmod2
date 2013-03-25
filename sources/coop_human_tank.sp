#include <sourcemod>
#include <sdktools>
#include <sdktools_functions>

#pragma semicolon 1

#define L4D_ZOMBIECLASS_TANK 	5
#define L4D2_ZOMBIECLASS_TANK	8
new ZC_TANK;

// Sdk calls
new Handle:config = INVALID_HANDLE;
new Handle:setHumanSpec = INVALID_HANDLE;
new Handle:takeOverBot = INVALID_HANDLE;

#define TEAM_SPECTATORS 1
#define TEAM_SURVIVORS 2
#define TEAM_INFECTED 3

#define L4D_MAXPLAYERS 32

new PlayerPoints[L4D_MAXPLAYERS + 1];
new bool:NoTankPlayer[L4D_MAXPLAYERS + 1];

new JoinUsedTime = 0;

public Plugin:myinfo = 
{
	name = "Human tank in coop",
	author = "D1maxa",
	description = "Human plays for tank in coop",
	version = "1.1.j",
	url = "http://hl2.msk.su"
}

public OnPluginStart()
{		
	decl String:game[12];
	GetGameFolderName(game, sizeof(game));
	if (StrEqual(game,"left4dead2"))
	{
		ZC_TANK = L4D2_ZOMBIECLASS_TANK;
	}
	else
	{
		ZC_TANK = L4D_ZOMBIECLASS_TANK;
	}
	
	HookEvent("tank_killed", OnTankKilled, EventHookMode_Post);
	HookEvent("player_death", OnTankKilled, EventHookMode_Post);
	HookEvent("player_incapacitated", OnPlayerInc, EventHookMode_Post);
	HookEvent("round_start", OnRoundStart, EventHookMode_PostNoCopy);
	HookEvent("tank_frustrated", OnTankKilled, EventHookMode_Post);
	
	RegServerCmd("sm_spawntank", SpawnHumanTank, "Spawn human tank in coop");
	RegServerCmd("sm_restrict_tank", RestrictTank, "Restrict tank for player");
//	RegConsoleCmd("sm_join2", JoinSurvivors, "Join survivors team");
	
	// SDK Calls
	config = LoadGameConfigFile("l4d_coop_human_tank");
	
	StartPrepSDKCall(SDKCall_Player);
	PrepSDKCall_SetFromConf(config, SDKConf_Signature, "SetHumanSpec");
	PrepSDKCall_AddParameter(SDKType_CBasePlayer, SDKPass_Pointer);
	setHumanSpec = EndPrepSDKCall();
	
	StartPrepSDKCall(SDKCall_Player);
	PrepSDKCall_SetFromConf(config, SDKConf_Signature, "TakeOverBot");
	PrepSDKCall_AddParameter(SDKType_Bool, SDKPass_Plain);
	takeOverBot = EndPrepSDKCall();		
}

public Action:TimedJoinSurvivorsSimple(Handle:timer, any:client)
{
	FakeClientCommand(client, "sm_join");
}

public Action:TimedJoinSurvivors(Handle:timer, any:client)
{
	if (IsClientInGame(client) && GetClientTeam(client) != TEAM_SURVIVORS && !IsFakeClient(client))
	{
		if (GetTime() < JoinUsedTime)
		{
			CreateTimer(1.0, TimedJoinSurvivors, client, TIMER_FLAG_NO_MAPCHANGE);
		}
		else
		{
			SendHumanToSurvivors(INVALID_HANDLE, client);
		}
	}
}

public Action:JoinSurvivors(client, args)
{
	if (!client) return;
	if (GetClientTeam(client) == TEAM_SURVIVORS) return;
	new CurrentTime = GetTime();
	if (JoinUsedTime - CurrentTime > 10)
	{
		JoinUsedTime = 0;
	}
	if (CurrentTime < JoinUsedTime)
	{
		ReplyToCommand(client, "Please wait %d seconds and try again", JoinUsedTime - CurrentTime);
		return;
	}
	SendHumanToSurvivors(INVALID_HANDLE, client);
}

public Action:SpawnHumanTank(args)
{
	new surv;
	if (args < 1)
	{
		surv = GetHumanSurvivor();
	}
	else
	{
		decl String:arg[8];
		GetCmdArg(1, arg, sizeof(arg));	
		surv = GetHumanSurvivorEx(GetClientOfUserId(StringToInt(arg)));
	}
	if (surv == 0) return;
	/*
	PrintHintText(surv, "Prepare for playing as Tank");	
	CreateTimer(GetConVarFloat(l4d_tankmessage_duration),SpawnNewTank,surv,TIMER_FLAG_NO_MAPCHANGE);
	*/

	ChangeClientTeam(surv, 3);
	CreateTimer(1.0,CheckHumanTeam, surv, TIMER_FLAG_NO_MAPCHANGE);
		
	new String:command[] = "z_spawn";
	new flags = GetCommandFlags(command);
	SetCommandFlags(command, flags & ~FCVAR_CHEAT);
	FakeClientCommand(surv, "z_spawn tank auto");
	SetCommandFlags(command, flags);
}

public Action:RestrictTank(args)
{
	new target;
	new NoTankOption;
	if (args < 2)
	{
	
	}
	else
	{
		decl String:str_target[8];
		decl String:str_option[8];
		GetCmdArg(1, str_target, sizeof(str_target));	
		target = GetClientOfUserId(StringToInt(str_target));
		GetCmdArg(2, str_option, sizeof(str_option));	
		NoTankOption = StringToInt(str_option);
	}
	if (target == 0) return;
	if (NoTankOption > 0)
	{
		NoTankPlayer[target] = true;
	}
	else
	{
		NoTankPlayer[target] = false;
	}
}

/*
public Action:SpawnNewTank(Handle:timer, any:surv)
{
	if (!IsClientInGame(surv) || IsFakeClient(surv) || GetClientTeam(surv) != 2)
	{
		surv = GetHumanSurvivor();
		if (surv == 0) return;
	}
	...
}
*/

IsTeamKilled()
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i) && IsClientInGame(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				if (GetEntProp(i, Prop_Send, "m_isIncapacitated") < 1)
				{
					return 0;
				}
			}
		}
	}
	return 1;
}

Player_Killed(client, attacker)
{
	if (attacker > 0)
	{
		if (GetClientTeam(attacker) != TEAM_INFECTED)
		{
			if (client > 0 && client <= L4D_MAXPLAYERS && IsClientInGame(client) && !IsFakeClient(client))
			{
				if (GetClientTeam(client) == TEAM_INFECTED)
				{
					ServerCommand("sm_tankresults %d %d", GetClientUserId(client), PlayerPoints[client]);
				}
			}
			return;
		}
	}
	if (attacker)
	{
		PlayerPoints[attacker] += 1;
		PrintHintText(attacker, "Tank Points: %d", PlayerPoints[attacker]);
	}
}

public OnTankKilled(Handle:event, const String:name[], bool:dontBroadcast)
{	
	new client = GetClientOfUserId(GetEventInt(event,"userid"));
	if (client != 0 && !IsFakeClient(client) && GetClientTeam(client) == TEAM_INFECTED)
	{
//		PrintToChatAll("\x05EVENT: %s", name);
//		PrintHintText(client, "You will be returned to survivors in %i seconds", GetConVarInt(l4d_returnmessage_duration));
		ChangeClientTeam(client, 1);
		CreateTimer(1.0, TimedJoinSurvivorsSimple, client, TIMER_FLAG_NO_MAPCHANGE);
//		CreateTimer(1.0, TimedJoinSurvivors, client, TIMER_FLAG_NO_MAPCHANGE);
//		CreateTimer(GetConVarFloat(l4d_returnmessage_duration),  SendHumanToSurvivors, client, TIMER_FLAG_NO_MAPCHANGE);
	}
	else
	{
		new attacker = GetClientOfUserId(GetEventInt(event,"attacker"));
		if (client != attacker && client > 0)
		{
			if (GetClientTeam(client) == TEAM_SURVIVORS)
			{
				Player_Killed(client, attacker);
			}
		}
	}
}

public OnPlayerInc(Handle:event, const String:name[], bool:dontBroadcast)
{	
	new attacker = GetClientOfUserId(GetEventInt(event,"attacker"));
	new client = GetClientOfUserId(GetEventInt(event,"userid"));
	if (client > 0 && client <= L4D_MAXPLAYERS)
	{
		if (IsClientInGame(client) && !IsFakeClient(client))
		{
			if (GetClientTeam(client) == TEAM_INFECTED && GetEntProp(client, Prop_Send, "m_isIncapacitated") > 0)
			{
				ServerCommand("sm_tankresults %d %d", GetClientUserId(client), PlayerPoints[client]);
			}
		}
	}
	if (!attacker || !client || client == attacker)
	{
//		PrintToChatAll("\x04if (!attacker || !client || client == attacker)");
		return;
	}
	if (!IsClientInGame(attacker) || !IsClientInGame(client))
	{
//		PrintToChatAll("\x04if (!IsClientInGame(attacker) || !IsClientInGame(client))");
		return;
	}
	if (GetClientTeam(client) == TEAM_SURVIVORS && GetClientTeam(attacker) == TEAM_INFECTED && !IsFakeClient(attacker))
	{
//		PrintToChatAll("\x03if (GetClientTeam(client) == TEAM_SURVIVORS && GetClientTeam(attacker) == TEAM_INFECTED)");
		if (IsTeamKilled() > 0)
		{
			PlayerPoints[attacker] += 3;
			ServerCommand("sm_tankresults %d %d", GetClientUserId(attacker), PlayerPoints[attacker]);
		}
//		PrintToChatAll("\x05Tank Points: %d", PlayerPoints[attacker]);
		PrintHintText(attacker, "Tank Points: %d", PlayerPoints[attacker]);
	}
}

public OnRoundStart(Handle:event, const String:name[], bool:dontBroadcast)
{
	for(new i=1; i <= L4D_MAXPLAYERS; i++)
	{
		PlayerPoints[i] = 0;
		if (IsValidEntity(i) && IsClientInGame(i) && GetClientTeam(i) == TEAM_INFECTED && !IsFakeClient(i)) 
			SendHumanToSurvivors(INVALID_HANDLE, i);
	}
}

public Action:CheckHumanTeam(Handle:timer, any:surv)
{
	if (IsClientInGame(surv) &&	GetClientTeam(surv) == TEAM_INFECTED && !IsFakeClient(surv))
	{
		if (!IsPlayerTank(surv) || !IsPlayerAlive(surv))
			SendHumanToSurvivors(INVALID_HANDLE, surv);
	}
}

public bool:IsPlayerTank (client)
{
	return (GetEntProp(client, Prop_Send, "m_zombieClass") == ZC_TANK);
}

public Action:SendHumanToSurvivors(Handle:timer, any:surv)
{
	if (!IsClientInGame(surv) || GetClientTeam(surv) == TEAM_SURVIVORS || IsFakeClient(surv))
		return;

	ChangeClientTeam(surv, TEAM_SPECTATORS); 
	// Search for an empty bot
	new bot = 1;
	while (bot <= L4D_MAXPLAYERS && !(IsClientInGame(bot) && IsFakeClient(bot) && GetClientTeam(bot) == TEAM_SURVIVORS && IsPlayerAlive(bot)))
	{
		bot++;
	}
	if (bot > MaxClients)
	{
		bot = 1;
		while (bot <= L4D_MAXPLAYERS && !(IsClientInGame(bot) && IsFakeClient(bot) && GetClientTeam(bot) == TEAM_SURVIVORS))
		{
			bot++;
		}
		if (bot > L4D_MAXPLAYERS)
		{
			bot = CreateFakeClient("SurvivorBot");
			if (bot == 0)
			{
				bot = CreateFakeClient("SurvivorBot");
				// try another one?
			}
			if (bot != 0)
			{
				ChangeClientTeam(bot, TEAM_SURVIVORS);
				if(DispatchKeyValue(bot, "classname", "SurvivorBot") == false)
				{
					LogError("Failed to set bot's classname");
				}
				CreateTimer(0.1, kickbot, bot, TIMER_FLAG_NO_MAPCHANGE);
				CreateTimer(0.2, SendHumanToSurvivors, surv, TIMER_FLAG_NO_MAPCHANGE);
			}
			else
			{
				ReplyToCommand(surv, "Failed to join survivors team, try again later");
//				CreateTimer(5.0, SendHumanToSurvivors, surv, TIMER_FLAG_NO_MAPCHANGE);
			}
			return;
		}
	}

	JoinUsedTime = GetTime() + 3;
	SDKCall(setHumanSpec, bot, surv);
	SDKCall(takeOverBot, surv, true);
	CreateTimer(1.0, CheckTeam, surv, TIMER_FLAG_NO_MAPCHANGE);
}

public Action:CheckTeam(Handle:timer, any:surv)
{
	if(IsClientInGame(surv) && GetClientTeam(surv) != TEAM_SURVIVORS && !IsFakeClient(surv))
		SendHumanToSurvivors(INVALID_HANDLE, surv);
}

public Action:kickbot(Handle:timer, any:bot)
{
	if (IsClientConnected(bot) && IsFakeClient(bot))
		KickClient(bot,"fake player");
}

GetHumanSurvivor()
{
	new size = 0;
	new human[L4D_MAXPLAYERS + 1];
	for(new i=1; i <= L4D_MAXPLAYERS; i++)
	{
		if(IsClientInGame(i) && GetClientTeam(i) == TEAM_SURVIVORS && !IsFakeClient(i) && !NoTankPlayer[i])
		{
			human[size] = i;
			size++;
		}
	}
	if (size == 0) return 0;
	if (size == 1) return human[0];
	return human[GetRandomInt(0, size-1)];
}

GetHumanSurvivorEx(client)
{
	new size = 0;
	new human[L4D_MAXPLAYERS + 1];
	for(new i=1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientInGame(i) && !IsFakeClient(i) && GetClientTeam(i) == TEAM_SURVIVORS && !NoTankPlayer[i])
		{
			if (!IsPlayerAlive(i))
			{
				human[size] = i;
				size++;
			}
		}
	}
	if (size == 0) return client;
	if (size == 1) return human[0];
	return human[GetRandomInt(0, size-1)];
}