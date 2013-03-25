#define SOUND_1 "ui/beepclear.wav"

new Handle:IsGlowEnabled;

new bool:GlowFlag[L4D_MAXPLAYERS + 1];
new bool:ColorFlag[L4D_MAXPLAYERS + 1];
new bool:SoundFlag[L4D_MAXPLAYERS + 1];
new bool:l4d_healthglow = false;
new CycleState[L4D_MAXPLAYERS + 1];
new Float:PulseDelay = 0.12;

public GlowOnPluginStart()
{
	Version += 16;
	IsGlowEnabled = CreateConVar("hm_glow_mode", "1", "0 - off | 1 - auto | 2 - on", FCVAR_PLUGIN);
	RegConsoleCmd("sm_glow", Command_Glow);
	RegAdminCmd("sm_glowstop", Command_GlowStop, ADMFLAG_CHEATS, "sm_glowstop");
}

public GlowOnMapStart()
{
	PrecacheSound(SOUND_1, true);
	if (FindConVar("l4d2_healthglows_version") != INVALID_HANDLE) l4d_healthglow = true;
	else l4d_healthglow = false;
}

public Action:Command_Glow(client, args)
{
	if (client < 1 || !IsPlayerAlive(client)) return;
	else
	{
		if (!GlowFlag[client])
		{
			if (GetClientTeam(client) != TEAM_SURVIVORS)
			{
				return;
			}
			CycleState[client] = 0;
			GlowFlag[client] = true;
			ColorFlag[client] = true;
			SoundFlag[client] = true;
			SetPlayerGlow(client, true);
			CreateTimer(PulseDelay, PulseTimer, client);
		}
		else
		{
			CycleState[client] = 0;
			GlowFlag[client] = false;
			ColorFlag[client] = false;
			SoundFlag[client] = false;
			SetPlayerGlow(client, false);
			SetEntityRenderMode(client, RENDER_NORMAL);
			SetEntityRenderColor(client, 255, 255, 255, 255);
		}
	}
}

public Action:Command_GlowStop(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		CycleState[i] = 0;
		GlowFlag[i] = false;
		ColorFlag[i] = false;
		SoundFlag[i] = false;
	}
}

public Action:PulseTimer(Handle:timer, any:client)
{
	if (!IsValidClient(client)) return Plugin_Continue;
	if (!IsPlayerAlive(client))
	{
		CycleState[client] = 0;
		SetPlayerGlow(client, false);
		GlowFlag[client] = false;
		ColorFlag[client] = false;
		SoundFlag[client] = false;
		#if COOP_STATS_ENABLED
			GrantPlayerColor(client);
		#else
			SetClientColor(client, {255, 255, 255, 255});
		#endif
	}
	if (!GlowFlag[client])
	{
		CycleState[client] = 0;
		SetPlayerGlow(client, false)
		ColorFlag[client] = false;
		SoundFlag[client] = false;
		SetPlayerGlow(client, false);
		#if COOP_STATS_ENABLED
			GrantPlayerColor(client);
		#else
			SetClientColor(client, {255, 255, 255, 255});
		#endif
		return Plugin_Continue;
	}
	if (CycleState[client] > 1) CycleState[client] = 0;
	switch (CycleState[client])
	{
		case 0:
		{
			SetPlayerGlow(client, false);
			if (SoundFlag[client]) PlayClientSound(client)
			if (ColorFlag[client])
			{
			#if COOP_STATS_ENABLED
				GrantPlayerColor(client);
			#else
				SetClientColor(client, {255, 255, 255, 255});
			#endif
			}
		}
		case 1:
		{
			if (GetConVarInt(FindConVar("sv_disable_glow_survivors")) > 0) SetPlayerGlow(client, false);
			else
			{
				if (GetClientTeam(client) == TEAM_SURVIVORS) SetPlayerGlow(client, true);
				else SetPlayerGlow(client, false);
			}
//			if (SoundFlag[client])
//			{
//				PlayClientSound(client)
//			}
			if (ColorFlag[client]) SetClientColor(client, {255, 75, 0, 75});
		}
	}

	CycleState[client]++;
	CreateTimer(PulseDelay, PulseTimer, client);
	return Plugin_Continue;
}

public SetPlayerGlow(client, bool:flag)
{
	if (!IsValidClient(client) || !GetConVarInt(IsGlowEnabled)) return;
	else if (GetConVarInt(IsGlowEnabled) == 1 && l4d_healthglow) return;
	if (flag) SetEntProp(client, Prop_Send, "m_iGlowType", 3);
	else SetEntProp(client, Prop_Send, "m_iGlowType", 0);
}

PlayClientSound(client)
{
	if (!IsClientInGame(client)) return;
	new Float:position[3];
	GetClientAbsOrigin(client, position);
	position[2] += 10;
	EmitAmbientSound(SOUND_1, position, client, SNDLEVEL_NORMAL);	
}

SetClientColor(client, color[4])
{
	if (!IsValidClient(client)) return;
	SetEntityRenderMode(client, RENDER_TRANSALPHA);
	SetEntityRenderColor(client, color[1], color[2], color[3], color[0]);
//	SetEntityRenderMode(client, RENDER_NORMAL);
//	SetEntityRenderColor(client, 255, 255, 255, 255);
}

public GlowOnClientConnected(client)
{
	CycleState[client] = 0;
	SetPlayerGlow(client, false);
	GlowFlag[client] = false;
	ColorFlag[client] = false;
	SoundFlag[client] = false;
}

public GlowEvent_PlayerDeath(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	CycleState[client] = 0;
	GlowFlag[client] = false;
	ColorFlag[client] = false;
	SoundFlag[client] = false;
	SetPlayerGlow(client, false);
}

public GlowEvent_HealSuccess(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "subject"));
	GlowFlag[client] = false;
	ColorFlag[client] = false;
	SoundFlag[client] = false;
	SetPlayerGlow(client, false);
}

public GlowEvent_PillsUsed(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "subject"));
	if (GetEntProp(client, Prop_Send, "m_currentReviveCount") > 1) return;
	GlowFlag[client] = false;
	ColorFlag[client] = false;
	SoundFlag[client] = false;
	SetPlayerGlow(client, false);
}

public GlowEvent_DefibrillatorUsed(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "subject"));
//	if (!IsGoingToDie(client))
//	{
//		return;
//	}
	if (!ColorFlag[client])
	{
		GlowFlag[client] = true;
		ColorFlag[client] = true;
		SoundFlag[client] = false;
		CreateTimer(PulseDelay, PulseTimer, client);
	}
}

public GlowEvent_ReviveSuccess(Handle:event, const String:name[], bool:dontBroadcast)
{
	if (!GetEventBool(event, "lastlife")) return;
	new client = GetClientOfUserId(GetEventInt(event, "subject"));
	if (!ColorFlag[client])
	{
		GlowFlag[client] = true;
		ColorFlag[client] = true;
		SoundFlag[client] = false;
		CreateTimer(PulseDelay, PulseTimer, client);
	}
}