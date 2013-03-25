#define ENTITY_GASCAN	"models/props_junk/gascan001a.mdl"
#define ENTITY_PROPANE	"models/props_junk/propanecanister001a.mdl"
#define ENTITY_OXYGEN	"models/props_equipment/oxygentank01.mdl"

#define SOUND_FREEZE	"physics/glass/glass_impact_bullet4.wav"
#define SOUND_DEFROST	"physics/glass/glass_sheet_break1.wav"

#define SOUND_IMPACT01	"animation/van_inside_hit_wall.wav"
#define SOUND_IMPACT02	"ambient/explosions/explode_3.wav"
#define SOUND_IMPACT03  "ui\bigreward.wav"

#define SPRITE_BEAM		"materials/sprites/laserbeam.vmt"
#define SPRITE_HALO		"materials/sprites/halo01.vmt"

new freeze[L4D_MAXPLAYERS + 1];
new g_GlowSprite;
new g_BeamSprite;
new g_HaloSprite;

new Float:hm_heal_radius = 300.0;
new Float:hm_freeze_radius = 300.0;
new Float:hm_freeze_time = 15.0;

static g_flLagMovement = 0;

public EffectsOnPluginStart()
{
	Version += 8;
	RegAdminCmd("sm_killallfreezes", Command_KillAllFreezes, ADMFLAG_CHEATS, "sm_killallfreezes");
	RegAdminCmd("sm_resetteamspeed", Command_ResetTeamSpeed, ADMFLAG_CHEATS, "sm_resetteamspeed");
}

public EffectsOnMapStart()
{
	g_flLagMovement = FindSendPropInfo("CTerrorPlayer", "m_flLaggedMovementValue");
	g_BeamSprite = PrecacheModel(SPRITE_BEAM);
	g_HaloSprite = PrecacheModel(SPRITE_HALO);
	PrecacheSound(SOUND_FREEZE, true);
	PrecacheSound(SOUND_IMPACT01, true);
	PrecacheSound(SOUND_IMPACT02, true);
	PrecacheSound(SOUND_IMPACT03, true);
}

public Action:Command_KillAllFreezes(client, args)
{
	KillAllFreezes();
}

public Action:Command_ResetTeamSpeed(client, args)
{
	ResetTeamSpeed();
}

public Boom(Float:position[3])
{
	new entity = CreateEntityByName("prop_physics");
	if (!IsValidEntity(entity))
	{
		return;
	}
	DispatchKeyValue(entity, "model", ENTITY_PROPANE);
	DispatchSpawn(entity);
	SetEntData(entity, GetEntSendPropOffs(entity, "m_CollisionGroup"), 1, 1, true);
	TeleportEntity(entity, position, NULL_VECTOR, NULL_VECTOR);
	AcceptEntityInput(entity, "break");
}

public Boom2(Float:position[3])
{
	new entity = CreateEntityByName("prop_physics");
	if (!IsValidEntity(entity))
	{
		return;
	}
	DispatchKeyValue(entity, "model", ENTITY_OXYGEN);
	DispatchSpawn(entity);
	SetEntData(entity, GetEntSendPropOffs(entity, "m_CollisionGroup"), 1, 1, true);
	TeleportEntity(entity, position, NULL_VECTOR, NULL_VECTOR);
	AcceptEntityInput(entity, "break");
}

public Fire(Float:position[3])
{
	new entity = CreateEntityByName("prop_physics");
	if (!IsValidEntity(entity))
	{
		return;
	}
	DispatchKeyValue(entity, "model", ENTITY_GASCAN);
	DispatchSpawn(entity);
	SetEntData(entity, GetEntSendPropOffs(entity, "m_CollisionGroup"), 1, 1, true);
	TeleportEntity(entity, position, NULL_VECTOR, NULL_VECTOR);
	AcceptEntityInput(entity, "break");
}

public Blizzard(client, Float:position[3])
{
	decl Float:client_pos[3];
	GetClientEyePosition(client, position);
	/* Emit impact sound */
	EmitAmbientSound(SOUND_IMPACT01, position);
	EmitAmbientSound(SOUND_IMPACT02, position);
	
	/* Laser effect */
//	CreateLaserEffect(client, 80, 80, 230, 230, 6.0, 1.0, VARTICAL);
	TE_SetupBeamRingPoint(position, 10.0, hm_freeze_radius,	g_BeamSprite, g_HaloSprite, 0, 10, 0.3, 10.0, 0.5, {40, 40, 230, 230}, 400, 0);
	TE_SendToAll();
	
	/* Freeze special infected and survivor in the radius */
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (!IsClientInGame(i) || !IsPlayerAlive(i))
			continue;
		GetClientEyePosition(i, client_pos);
		if (GetVectorDistance(position, client_pos) < hm_freeze_radius)
		{
			if (GetEntProp(i, Prop_Send, "m_zombieClass") != 8)
			{
				FreezePlayer(i, position);
			}
		}
	}
	
	/* Freeze infected in the radius */
	decl String:mName[64], Float:entPos[3];
	
//	MaxEntities = GetMaxEntities();
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEdict(i) && IsValidEntity(i))
		{
			GetEntPropString(i, Prop_Data, "m_ModelName", mName, sizeof(mName));
			if (StrContains(mName, "infected") != -1)
			{
				GetEntPropVector(i, Prop_Send, "m_vecOrigin", entPos);
				if (GetVectorDistance(position, entPos) < hm_freeze_radius)
				{
					FreezePlayer(i, position);
//					EmitAmbientSound(SOUND_FREEZE, entPos, i, SNDLEVEL_RAIDSIREN);
//					TE_SetupGlowSprite(entPos, g_GlowSprite, 5.0, 3.0, 130);
//					TE_SendToAll();
//					DamageEffect(i, 100.0);
				}
			}
		}
	}
}

public HealCircle(client, Float:position[3])
{
	decl Float:client_pos[3];
	GetClientEyePosition(client, position);
	/* Emit impact sound */
	EmitAmbientSound(SOUND_IMPACT03, position);
	
	TE_SetupBeamRingPoint(position, 10.0, hm_heal_radius,	g_BeamSprite, g_HaloSprite, 0, 10, 0.3, 10.0, 0.5, {40, 40, 230, 230}, 400, 0);
	TE_SendToAll();
	
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (!IsClientInGame(i) || !IsPlayerAlive(i))
			continue;
		GetClientEyePosition(i, client_pos);
		if (GetVectorDistance(position, client_pos) < hm_heal_radius)
		{
			if (GetEntProp(i, Prop_Send, "m_zombieClass") != 8)
			{
				HealPlayer(i);
			}
		}
	}
	
	decl String:mName[64], Float:entPos[3];
	
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEdict(i) && IsValidEntity(i))
		{
			GetEntPropString(i, Prop_Data, "m_ModelName", mName, sizeof(mName));
			if (StrContains(mName, "infected") != -1)
			{
				GetEntPropVector(i, Prop_Send, "m_vecOrigin", entPos);
				if (GetVectorDistance(position, entPos) < hm_heal_radius)
				{
					HealPlayer(i);
				}
			}
		}
	}
}

public FreezePlayer(entity, Float:position[3])
{
	SetEntityMoveType(entity, MOVETYPE_NONE);
	SetEntityRenderColor(entity, 102, 204, 255, 195);
	ScreenFade(entity, 0, 128, 255, 192, RoundToZero(hm_freeze_time * 1000), 1);
	EmitAmbientSound(SOUND_FREEZE, position, entity, SNDLEVEL_RAIDSIREN);
	TE_SetupGlowSprite(position, g_GlowSprite, hm_freeze_time, 0.5, 130);
	TE_SendToAll();
	freeze[entity] = ON;
	CreateTimer(hm_freeze_time, DefrostPlayer, entity);
}

public Action:DefrostPlayer(Handle:timer, any:entity)
{
	if (IsValidEdict(entity) && IsValidEntity(entity))
	{
		Func_DefrostPlayer(entity);
		#if COOP_STATS_ENABLED
			GrantPlayerColor(entity);
		#endif
	}
}

Func_DefrostPlayer(client)
{
	decl Float:entPos[3];
	GetEntPropVector(client, Prop_Send, "m_vecOrigin", entPos);
	EmitAmbientSound(SOUND_DEFROST, entPos, client, SNDLEVEL_RAIDSIREN);
	SetEntityMoveType(client, MOVETYPE_WALK);
	SetEntityRenderColor(client, 255, 255, 255, 255);
	ScreenFade(client, 0, 0, 0, 0, 0, 1);
	freeze[client] = OFF;
}

public ScreenFade(target, red, green, blue, alpha, duration, type)
{
	new Handle:msg = StartMessageOne("Fade", target);
	BfWriteShort(msg, 500);
	BfWriteShort(msg, duration);
	if (type == 0)
		BfWriteShort(msg, (0x0002 | 0x0008));
	else
		BfWriteShort(msg, (0x0001 | 0x0010));
	BfWriteByte(msg, red);
	BfWriteByte(msg, green);
	BfWriteByte(msg, blue);
	BfWriteByte(msg, alpha);
	EndMessage();
}

KillAllFreezes()
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientInGame(i) && IsPlayerAlive(i))
		{
			Func_DefrostPlayer(i);
		}
	}
}

ResetTeamSpeed()
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientInGame(i) && IsPlayerAlive(i))
		{
			ChangeSpeed(i, 1.0);
		}
	}
}

public CreateRingEffect(client, colRed, colGre, colBlu, alpha, Float:width)
{ // stock TE_SetupBeamRingPoint(const Float:center[3], Float:Start_Radius, Float:End_Radius, ModelIndex, HaloIndex, StartFrame, 
 //       FrameRate, Float:Life, Float:Width, Float:Amplitude, const Color[4], Speed, Flags)
	decl color[4];
	color[0] = colRed;
	color[1] = colGre;
	color[2] = colBlu;
	color[3] = alpha;
	
	new Float:position[3];
	GetEntPropVector(client, Prop_Send, "m_vecOrigin", position);
	position[2] += 10;
	TE_SetupBeamRingPoint(position, 10.0, 50.0, g_BeamSprite, g_HaloSprite, 0, 10, 0.3, width, 1.5, color, 300, 0);
	TE_SendToAll();
}

stock PanicEvent() // from http://forums.alliedmods.net/showthread.php?p=1251446
{
	new Director = CreateEntityByName("info_director");
	DispatchSpawn(Director);
	AcceptEntityInput(Director, "ForcePanicEvent");
	AcceptEntityInput(Director, "Kill");
}

ChangeSpeed(target, Float:newspeed)
{
	if (!IsValidEntity(target)) return;
	SetEntDataFloat(target, g_flLagMovement, newspeed, true);
}