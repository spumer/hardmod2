#define OFF				0
#define ON				1

new bool:developerinfoflag = false;
new Handle:sm_developer = INVALID_HANDLE;

/* public bool:IsZombie(entity)
{
	if (GetEntProp(entity, Prop_Send, "m_zombieClass") != 8)
	{
		return false;
	}
	return true;
} */

#if defined HARDMOD
public BasicOnPluginStart()
{
	Version += 9;
	sm_developer = FindConVar("sm_developer");
	if (sm_developer == INVALID_HANDLE)
	{
		sm_developer = CreateConVar("sm_developer", "0", "", FCVAR_PLUGIN);
	}
	HookConVarChange(sm_developer, SMDeveloperChanged);
}
#endif

stock GetClientHealthTotal(client)
{
	return RoundToZero(GetClientTempHealth(client) * 1.0 + GetClientHealth(client) * 1.0);
}

public SMDeveloperChanged(Handle:hVariable, const String:strOldValue[], const String:strNewValue[])
{
	if (GetConVarInt(sm_developer) >= 1)
	{
		developerinfoflag = true;
	}
	else
	{
		developerinfoflag = false;
	}
}

stock PrintToAdmins(const String:format[], any:...)
{
	if (developerinfoflag == false)
	{
		return;
	}
	decl String:buffer[192];
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		new flags = GetAdminFlags(GetUserAdmin(i), Access_Effective);
		if (IsClientConnected(i) && !IsFakeClient(i))
		{
			if (IsClientInGame(i) && flags & ADMFLAG_ROOT)
			{ //  && flags & ADMFLAG_RCON
	//			SetGlobalTransTarget(i);
				VFormat(buffer, sizeof(buffer), format, 2);
				PrintToChat(i, "\x04• \x05%s", buffer);
	//			PrintToChat(i, "%s", buffer);
			}
		}
	}
}

public Action:TimedExecCfg(Handle:timer, any:client)
{
	ServerCommand("exec postload.cfg");
}

public Action:TimedKick(Handle:timer, any:client)
{
	KickClient(client);
}

public RemoveEntity(entity)
{
	if (IsValidEntity(entity) && IsValidEdict(entity))
	{
		RemoveEdict(entity);
	}
}

public Action:TimedRemoveEdict(Handle:timer, any:entity)
{
	if (IsValidEntity(entity) && IsValidEdict(entity))
	{
		RemoveEdict(entity);
	}
}

public Action:TimedRemoveEdictIfDead(Handle:timer, client, any:entity)
{
	if (IsValidEntity(entity) && IsValidEdict(entity))
	{
		RemoveEdict(entity);
	}
}

public GetAdminLvl(client)
{
	new user_level = GetAdminImmunityLevel(GetUserAdmin(client));
#if defined HARDMOD	
	#if PLAYERS_ENABLED
		new user_level_players = GetPlayerLvl(client);
		if (user_level < user_level_players)
		{
			user_level = user_level_players;
		}
	#endif
#endif
	return user_level;
}

public GetClientZC(client)
{
	if (!IsValidEntity(client) || !IsValidEdict(client))
	{
		return 0;
	}
	return GetEntProp(client, Prop_Send, "m_zombieClass");
}

public bool:IsIncapacitated(client)
{
	//FindSendPropInfo("CTerrorPlayer", "m_isIncapacitated")
	new isIncap = GetEntProp(client, Prop_Send, "m_isIncapacitated");

	if (isIncap)
	{
		return true;
	}

	return false;
}

public bool:IsGoingToDie(client)
{
	if (!IsValidEntity(client) || !IsValidEdict(client))
	{
		return false;
	}
	//FindSendPropInfo("CTerrorPlayer", "m_isIncapacitated")
	new m_isGoingToDie = GetEntProp(client, Prop_Send, "m_isGoingToDie");

	if (m_isGoingToDie > 1)
	{
		return true;
	}

	return false;
}

stock IsTankAlive()
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i) && IsClientInGame(i))
		{
			if (IsPlayerAlive(i))
			{
				if (GetClientZC(i) == ZC_TANK && !IsIncapacitated(i))
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

stock CheckCvarRange(const Cvar_Value, Cvar_Value_Min, Cvar_Value_Max)
{
	if (Cvar_Value < Cvar_Value_Min)
	{
		return Cvar_Value_Min;
	}
	else if (Cvar_Value > Cvar_Value_Max)
	{
		return Cvar_Value_Max;
	}
	else
	{
		return Cvar_Value;
	}
}

public bool:CheckGameMode(const String:GameModeName[])
{
	new String:GameMode[24];
	new Handle:gamecvar_mp_gamemode = FindConVar("mp_gamemode");
	GetConVarString(gamecvar_mp_gamemode, GameMode, sizeof(GameMode));
	if (StrEqual(GameMode, GameModeName, false) == true)
	{
		return true;
	}
	return false;
}

stock GetBaseGameMode()
{
	new String:GameMode[24];
	new Handle:gamecvar_mp_gamemode = FindConVar("mp_gamemode");
	GetConVarString(gamecvar_mp_gamemode, GameMode, sizeof(GameMode));
	if (StrEqual(GameMode, "coop", false) == true)
	{
		return 1;
	}
	if (StrEqual(GameMode, "realism", false) == true)
	{
		return 1;
	}
	if (StrEqual(GameMode, "mutation1", false) == true)
	{
		return 1;
	}
	if (StrEqual(GameMode, "mutation2", false) == true)
	{
		return 1;
	}
	if (StrEqual(GameMode, "mutation3", false) == true)
	{
		return 1;
	}
	if (StrEqual(GameMode, "mutation4", false) == true)
	{
		return 1;
	}
	if (StrEqual(GameMode, "mutation5", false) == true)
	{
		return 1;
	}
	if (StrEqual(GameMode, "mutation7", false) == true)
	{
		return 1;
	}
	if (StrEqual(GameMode, "mutation8", false) == true)
	{
		return 1;
	}
	if (StrEqual(GameMode, "mutation9", false) == true)
	{
		return 1;
	}
	if (StrEqual(GameMode, "mutation10", false) == true)
	{
		return 1;
	}
	if (StrEqual(GameMode, "mutation14", false) == true)
	{
		return 1;
	}
	if (StrEqual(GameMode, "versus", false) == true)
	{
		return 2;
	}
	if (StrEqual(GameMode, "teamversus", false) == true)
	{
		return 2;
	}
	if (StrEqual(GameMode, "mutation11", false) == true)
	{
		return 2;
	}
	if (StrEqual(GameMode, "mutation12", false) == true)
	{
		return 2;
	}
	if (StrEqual(GameMode, "scavenge", false) == true)
	{
		return 3;
	}
	if (StrEqual(GameMode, "teamscavenge", false) == true)
	{
		return 3;
	}
	if (StrEqual(GameMode, "mutation15", false) == true)
	{
		return 3;
	}
	if (StrEqual(GameMode, "survival", false) == true)
	{
		return 4;
	}
	return 0;
}

stock GetClient()
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i) && IsClientInGame(i))
		{
			if (!IsFakeClient(i))
			{
				return i;
			}
		}
	}
	return 0;
}

stock GetFakeClient()
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i) && IsClientInGame(i))
		{
			if (IsFakeClient(i))
			{
				return i;
			}
		}
	}
	return 0;
}

stock GetAnyClient()
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i) && IsClientInGame(i))
		{
			return i;
		}
	}
	return 0;
}

#if !defined HARDMOD
	#define TEAM_SURVIVORS 2
#endif
stock GetAnyRealClient()
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i) && IsClientInGame(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			return i;
		}
	}
	return 0;
}

stock IsValidClient(client)
{
	if (!IsValidEntity(client))
	{
		return false;
	}
	if (client < 1 || client > L4D_MAXPLAYERS)
	{
		return false;
	}
	return true;
}

stock IsRealClient(client)
{
	if (!IsValidClient(client))
	{
		return false;
	}
	if (IsClientConnected(client) && IsClientInGame(client) && !IsFakeClient(client))
	{
		if (IsClientInGame(client))
		{
			if (!IsFakeClient(client))
			{
//				PrintToChatAll("\x05%N - !IsFakeClient(client)", client);
				return true;
			}
		}
	}
	return false;
}

stock FakeClientCount()
{
	new fake_client_count = 0;
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i))
		{
			if (IsFakeClient(i))
			{
				fake_client_count ++;
			}
		}
	}
	return fake_client_count;
}

stock GetRealClientCount(bool:inGameOnly = true)
{
	new clients = 0;
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i))
		{
			if (IsClientInGame(i) && !IsFakeClient(i))
			{
				clients++;
			}
		}
	}
	return clients;
}

stock GetSurvivorsCount(bool:inGameOnly = true)
{
	new clients = 0;

	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i) && IsClientInGame(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS)
			{
				clients++;
			}
		}
	}
	return clients;
}

public ServerKickClient(client)
{
	ServerCommand("kickid %d", GetClientUserId(client));
}

public KickFakeClients(kick_mode)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i))
		{
			if (IsFakeClient(i))
			{
				if (kick_mode < 0)
				{
					ServerKickClient(i);
				}
				else if (kick_mode == GetClientTeam(i))
				{
					ServerKickClient(i);
				}
			}
		}
	}
}

public KickTeam(team)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i) && IsClientInGame(i))
		{
			if (GetClientTeam(i) == team)
			{
				ServerKickClient(i);
			}
		}
	}
}

stock PrecacheParticle(String:ParticleName[])
{
	new Particle = CreateEntityByName("info_particle_system");
	if (IsValidEntity(Particle) && IsValidEdict(Particle))
	{
		DispatchKeyValue(Particle, "effect_name", ParticleName);
		DispatchSpawn(Particle);
		ActivateEntity(Particle);
		AcceptEntityInput(Particle, "start");
		CreateTimer(0.3, timerRemovePrecacheParticle, Particle, TIMER_FLAG_NO_MAPCHANGE);
	}
}

public Action:timerRemovePrecacheParticle(Handle:timer, any:Particle)
{
	if (IsValidEntity(Particle) && IsValidEdict(Particle))
	{
		AcceptEntityInput(Particle, "Kill");
	}
}