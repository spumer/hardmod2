new Handle:Medkit_Full_Health;
new Handle:HealOnEnd;

public HealingOnPluginStart()
{
	Version += 6;
	Medkit_Full_Health = CreateConVar("hm_medkitheal", "100", "", FCVAR_PLUGIN|FCVAR_SPONLY|FCVAR_NOTIFY);
	HealOnEnd = CreateConVar("hm_healonend", "1", "", FCVAR_PLUGIN|FCVAR_SPONLY|FCVAR_NOTIFY);
	HookEvent("heal_success", HealingEvent_MedkitUsed);
	RegAdminCmd("sm_normalizehealth", Command_NormalizeHealth, ADMFLAG_CHEATS, "sm_normalizehealth");
	RegAdminCmd("sm_health", Command_Health, ADMFLAG_CHEATS, "sm_health <count>");
}

public Action:Command_NormalizeHealth(client, args)
{
	NormalizeHealth();
	
	return Plugin_Continue;
}

public Action:Command_Health(client, args)
{
	new HP;
	if (args < 1)
	{
		HP = 100;
	}
	else
	{
		new String:string_health[8];
		GetCmdArg(1, string_health, sizeof(string_health));		
		HP = StringToInt(string_health);
	}
	new flags = GetCommandFlags("give");
	SetCommandFlags("give", flags & ~FCVAR_CHEAT);
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i) && IsClientInGame(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				FakeClientCommand(i, "give health");
				#if GLOW_ENABLED	
					if (GlowFlag[i])
					{
						GlowFlag[i] = false;
					}
				#endif				
				SetEntityHealth(i, HP);
				SetEntProp(i, Prop_Send, "m_isGoingToDie", 0);
				SetEntProp(i, Prop_Send, "m_currentReviveCount", 0);
				SetEntPropFloat(i, Prop_Send, "m_healthBuffer", 0.0); 
				SetEntPropFloat(i, Prop_Send, "m_healthBufferTime", 0.0);
				SetTempHealth(i, 0);
			}
		}
	}
	SetCommandFlags("give", flags);
}

//Used to set temp health, written by TheDanner.
public SetTempHealth(client, hp)
{
	SetEntPropFloat(client, Prop_Send, "m_healthBufferTime", GetGameTime());
	new Float:newOverheal = hp * 1.0; // prevent tag mismatch
	SetEntPropFloat(client, Prop_Send, "m_healthBuffer", newOverheal);
}

public GetHealth(client)
{
	return GetEntProp(client, Prop_Send, "m_iHealth");
}

public NormalizeHealth()
{
	new map_full_health = 0;
	if (StrEqual(current_map, "c1m3_mall", false) || StrEqual(current_map, "c4m2_sugarmill_a", false))
	{
		map_full_health = 1;
	}
	
	new HP;
	new flags = GetCommandFlags("give");
	SetCommandFlags("give", flags & ~FCVAR_CHEAT);
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i) && IsClientInGame(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				HP = GetHealth(i);
				
				if (!CheckGameMode("mutation3"))
				{
					if (map_full_health > 0)
					{
						FakeClientCommand(i, "give health");
						SetEntityHealth(i, 100);
					}
					else
					{
						if (HP < 51)
						{
							FakeClientCommand(i, "give health");
							SetEntityHealth(i, HP + 50);
							#if GLOW_ENABLED	
								if (GlowFlag[i])
								{
									GlowFlag[i] = false;
								}
							#endif							
						}
						else if (IsIncapacitated(i))
						{
							FakeClientCommand(i, "give health");
							SetEntityHealth(i, 50);
						}
						else
						{
							FakeClientCommand(i, "give health");
						}
					}
					SetEntProp(i, Prop_Send, "m_isGoingToDie", 0);
					SetEntProp(i, Prop_Send, "m_currentReviveCount", 0);
					SetEntPropFloat(i, Prop_Send, "m_healthBuffer", 0.0); 
					SetEntPropFloat(i, Prop_Send, "m_healthBufferTime", 0.0);
					SetTempHealth(i, 0);
				}
				else
				{
					FakeClientCommand(i, "give health");
					SetEntityHealth(i, 100);
					SetEntProp(i, Prop_Send, "m_isGoingToDie", 0);
					SetEntProp(i, Prop_Send, "m_currentReviveCount", 0);
					SetEntPropFloat(i, Prop_Send, "m_healthBuffer", 0.0); 
					SetEntPropFloat(i, Prop_Send, "m_healthBufferTime", 0.0);
					SetTempHealth(i, 0);
					SwitchHealth(i);
				}
			}
		}
	}
	SetCommandFlags("give", flags);
}

public Action:HealingEvent_MedkitUsed(Handle:event, const String:name[], bool:dontBroadcast)
{
	if (GetConVarInt(HealOnEnd) < 1)
	{
		return Plugin_Continue;
	}
//	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	new target = GetClientOfUserId(GetEventInt(event, "subject"));

	if (GetConVarInt(Medkit_Full_Health) < GetHealth(target))
	{
		return Plugin_Continue;
	}

	SetEntityHealth(target, GetConVarInt(Medkit_Full_Health));
	SetEntProp(target, Prop_Send, "m_isGoingToDie", 0);
	SetEntProp(target, Prop_Send, "m_currentReviveCount", 0);
	SetEntPropFloat(target, Prop_Send, "m_healthBuffer", 0.0); 
	SetEntPropFloat(target, Prop_Send, "m_healthBufferTime", 0.0);
	SetTempHealth(target, 0);
	
	if (CheckGameMode("mutation3"))
	{
		SwitchHealth(target);
	}
	
	return Plugin_Continue;
}

HealRoundStartEvent()
{
	if (CheckGameMode("mutation3")) return;
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i))
		{
			if (IsClientConnected(i) && IsClientInGame(i) && IsPlayerAlive(i))
			{
				if (GetClientTeam(i) == TEAM_SURVIVORS)
				{
					SwitchHealthNormal(i);
					RemoveTempHealth(i);
				}
			}
		}
	}
}

stock GetClientTempHealth(client)
{
	//First filter -> Must be a valid client, successfully in-game and not an spectator (The dont have health).
    if (!client
    || !IsValidEntity(client)
    || !IsClientInGame(client)
	|| !IsPlayerAlive(client)
    || IsClientObserver(client)
	|| GetClientTeam(client) != TEAM_SURVIVORS)
    {
        return -1;
    }
    
    //First, we get the amount of temporal health the client has
    new Float:buffer = GetEntPropFloat(client, Prop_Send, "m_healthBuffer");
    
    //We declare the permanent and temporal health variables
    new Float:TempHealth;
    
    //In case the buffer is 0 or less, we set the temporal health as 0, because the client has not used any pills or adrenaline yet
    if (buffer <= 0.0)
    {
        TempHealth = 0.0;
    }
    
    //In case it is higher than 0, we proceed to calculate the temporl health
    else
    {
        //This is the difference between the time we used the temporal item, and the current time
        new Float:difference = GetGameTime() - GetEntPropFloat(client, Prop_Send, "m_healthBufferTime");
        
        //We get the decay rate from this convar (Note: Adrenaline uses this value)
        new Float:decay = GetConVarFloat(FindConVar("pain_pills_decay_rate"));
        
        //This is a constant we create to determine the amount of health. This is the amount of time it has to pass
        //before 1 Temporal HP is consumed.
        new Float:constant = 1.0/decay;
        
        //Then we do the calcs
        TempHealth = buffer - (difference / constant);
    }
    
    //If the temporal health resulted less than 0, then it is just 0.
    if (TempHealth < 0.0)
    {
        TempHealth = 0.0;
    }
    
    //Return the value
    return RoundToFloor(TempHealth);
}

stock RemoveTempHealth(client)
{
	if (!client
    || !IsValidEntity(client)
    || !IsClientInGame(client)
	|| !IsPlayerAlive(client)
    || IsClientObserver(client)
	|| GetClientTeam(client) != TEAM_SURVIVORS)
    {
        return;
    }
	SetTempHealth(client, 0);
}

public HealPlayer(entity)
{
	CheatCMD(entity, "give", "health");
	SetTempHealth(entity, 0);
	if (CheckGameMode("mutation3"))
	{
		SwitchHealth(entity);
	}
#if GLOW_ENABLED	
	if (GlowFlag[entity])
	{
		GlowFlag[entity] = false;
	}
#endif
}

stock SwitchHealth(client)
{
	new Float:iTempHealth = GetClientTempHealth(client) * 1.0;
	new Float:iPermHealth = GetClientHealth(client) * 1.0;
	new Float:flTotal = iTempHealth + iPermHealth;
	SetEntityHealth(client, 1);
	RemoveTempHealth(client);
	SetTempHealth(client, RoundToZero(flTotal));
}

stock SwitchHealthNormal(client)
{
	new iTempHealth = GetClientTempHealth(client);
	new iPermHealth = GetClientHealth(client);
	RemoveTempHealth(client);
	SetEntityHealth(client, iTempHealth+iPermHealth);
}