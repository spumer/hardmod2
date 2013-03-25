// © http://forums.alliedmods.net/showthread.php?p=1251446

#include <sdktools_functions>

#define FIRE_PARTICLE "gas_explosion_ground_fire"
#define EXPLOSION_PARTICLE "FluidExplosion_fps"
#define EXPLOSION_PARTICLE2 "weapon_grenade_explosion"
#define EXPLOSION_PARTICLE3 "explosion_huge_b"
#define EXPLOSION_SOUND "ambient/explosions/explode_1.wav"

new bool:g_bStrike = false;
new Float:g_cvarRainRadius = 300.0;
new g_cvarRadius = 500;
new g_cvarPower = 500;
new g_cvarDuration = 15;

new Handle:sdkCallPushPlayer = INVALID_HANDLE;
new Handle:g_hGameConf = INVALID_HANDLE;

public AirStrikeOnPluginStart()
{
	g_hGameConf = LoadGameConfigFile("l4d2customcmds");
	if (g_hGameConf == INVALID_HANDLE)
	{
		SetFailState("Couldn't find the offsets and signatures file. Please, check that it is installed correctly.");
	}
	
	StartPrepSDKCall(SDKCall_Player);
	PrepSDKCall_SetFromConf(g_hGameConf, SDKConf_Signature, "CTerrorPlayer_Fling");
	PrepSDKCall_AddParameter(SDKType_Vector, SDKPass_ByRef);
	PrepSDKCall_AddParameter(SDKType_PlainOldData, SDKPass_Plain);
	PrepSDKCall_AddParameter(SDKType_CBasePlayer, SDKPass_Pointer);
	PrepSDKCall_AddParameter(SDKType_Float, SDKPass_Plain);
	sdkCallPushPlayer = EndPrepSDKCall();
	if (sdkCallPushPlayer == INVALID_HANDLE)
	{
		SetFailState("Unable to find the 'CTerrorPlayer_Fling' signature, check the file version!");
	}
}
	
public AirStrikeOnMapStart()
{
	PrecacheSound(EXPLOSION_SOUND);
	PrefetchSound(EXPLOSION_SOUND);
	BBBPrecacheParticle(FIRE_PARTICLE);
	BBBPrecacheParticle(EXPLOSION_PARTICLE);
	BBBPrecacheParticle(EXPLOSION_PARTICLE2);
	BBBPrecacheParticle(EXPLOSION_PARTICLE3);
}

stock BBBPrecacheParticle(String:ParticleName[])
{
	new Particle = CreateEntityByName("info_particle_system");
	if (IsValidEntity(Particle) && IsValidEdict(Particle))
	{
		DispatchKeyValue(Particle, "effect_name", ParticleName);
		DispatchSpawn(Particle);
		ActivateEntity(Particle);
		AcceptEntityInput(Particle, "start");
		CreateTimer(0.3, BBBtimerRemovePrecacheParticle, Particle);
	}
}

public Action:BBBtimerRemovePrecacheParticle(Handle:timer, any:Particle)
{
	if (IsValidEntity(Particle) && IsValidEdict(Particle))
	{
		AcceptEntityInput(Particle, "Kill");
	}
}

Airstrike(client)
{
	g_bStrike = true;
	CreateTimer(8.0, timerStrikeTimeout);
	CreateTimer(0.9, timerStrike, client, TIMER_REPEAT);
}

public Action:timerStrikeTimeout(Handle:timer)
{
	g_bStrike = false;
}

public Action:timerStrike(Handle:timer, any:client)
{
	if (!g_bStrike)
	{
		return Plugin_Stop;
	}
	decl Float:position[3];
	GetClientAbsOrigin(client, position);
	new Float:radius = g_cvarRainRadius;
	position[0] += GetRandomFloat(radius*-1, radius);
	position[1] += GetRandomFloat(radius*-1, radius);
	Boom(position);
	CreateExplosion(position);
//	Fire(position);
	return Plugin_Continue;
}

CreateExplosion(Float:position[3])
{
	decl String:sRadius[256];
	decl String:sPower[256];
	new Float:flMaxDistance = g_cvarRadius * 1.0;
	new Float:power = g_cvarPower * 1.0;
	IntToString(g_cvarRadius, sRadius, sizeof(sRadius));
	IntToString(g_cvarPower, sPower, sizeof(sPower));
	new exParticle = CreateEntityByName("info_particle_system");
	new exParticle2 = CreateEntityByName("info_particle_system");
	new exParticle3 = CreateEntityByName("info_particle_system");
	new exTrace = CreateEntityByName("info_particle_system");
	new exEntity = CreateEntityByName("env_explosion");
	new exPhys = CreateEntityByName("env_physexplosion");
	new exHurt = CreateEntityByName("point_hurt");
	/*new exPush = CreateEntityByName("point_push");*/
	
	//Set up the particle explosion
	DispatchKeyValue(exParticle, "effect_name", EXPLOSION_PARTICLE);
	DispatchSpawn(exParticle);
	ActivateEntity(exParticle);
	TeleportEntity(exParticle, position, NULL_VECTOR, NULL_VECTOR);
	
	DispatchKeyValue(exParticle2, "effect_name", EXPLOSION_PARTICLE2);
	DispatchSpawn(exParticle2);
	ActivateEntity(exParticle2);
	TeleportEntity(exParticle2, position, NULL_VECTOR, NULL_VECTOR);
	
	DispatchKeyValue(exParticle3, "effect_name", EXPLOSION_PARTICLE3);
	DispatchSpawn(exParticle3);
	ActivateEntity(exParticle3);
	TeleportEntity(exParticle3, position, NULL_VECTOR, NULL_VECTOR);
	
	DispatchKeyValue(exTrace, "effect_name", FIRE_PARTICLE);
	DispatchSpawn(exTrace);
	ActivateEntity(exTrace);
	TeleportEntity(exTrace, position, NULL_VECTOR, NULL_VECTOR);
	
	
	//Set up explosion entity
	DispatchKeyValue(exEntity, "fireballsprite", "sprites/muzzleflash4.vmt");
	DispatchKeyValue(exEntity, "iMagnitude", sPower);
	DispatchKeyValue(exEntity, "iRadiusOverride", sRadius);
	DispatchKeyValue(exEntity, "spawnflags", "828");
	DispatchSpawn(exEntity);
	TeleportEntity(exEntity, position, NULL_VECTOR, NULL_VECTOR);
	
	//Set up physics movement explosion
	DispatchKeyValue(exPhys, "radius", sRadius);
	DispatchKeyValue(exPhys, "magnitude", sPower);
	DispatchSpawn(exPhys);
	TeleportEntity(exPhys, position, NULL_VECTOR, NULL_VECTOR);
	
	//Set up hurt point
	DispatchKeyValue(exHurt, "DamageRadius", sRadius);
	DispatchKeyValue(exHurt, "DamageDelay", "0.1");
	DispatchKeyValue(exHurt, "Damage", "1");
	DispatchKeyValue(exHurt, "DamageType", "8");
	DispatchSpawn(exHurt);
	TeleportEntity(exHurt, position, NULL_VECTOR, NULL_VECTOR);
	
	EmitSoundToAll(EXPLOSION_SOUND, exParticle);
	
	//BOOM!
	AcceptEntityInput(exParticle, "Start");
	AcceptEntityInput(exParticle2, "Start");
	AcceptEntityInput(exParticle3, "Start");
	AcceptEntityInput(exTrace, "Start");
	AcceptEntityInput(exEntity, "Explode");
	AcceptEntityInput(exPhys, "Explode");
	AcceptEntityInput(exHurt, "TurnOn");
	
	new Handle:pack = CreateDataPack();
	WritePackCell(pack, exTrace);
	WritePackCell(pack, exHurt);
	CreateTimer(g_cvarDuration * 1.0, timerStopFire, pack);
	
	decl Float:distance[3], Float:tpos[3], Float:ratio[3], Float:addVel[3], Float:tvec[3];
	for(new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (i == 0 || !IsValidEntity(i) || !IsClientInGame(i) || !IsPlayerAlive(i))
		{
			continue;
		}
		if (GetClientTeam(i) != TEAM_SURVIVORS)
		{
			continue;
		}
		GetEntPropVector(i, Prop_Data, "m_vecOrigin", tpos);
		distance[0] = (position[0] - tpos[0]);
		distance[1] = (position[1] - tpos[1]);
		distance[2] = (position[2] - tpos[2]);
		
		new Float:realdistance = SquareRoot(FloatMul(distance[0], distance[0]) + FloatMul(distance[1], distance[1]));
		if (realdistance <= flMaxDistance)
		{			
			GetEntPropVector(i, Prop_Data, "m_vecVelocity", tvec);
			ratio[0] =  FloatDiv(distance[0], SquareRoot(distance[1] * distance[1] + distance[0] * distance[0])); //Ratio x/hypo
			ratio[1] =  FloatDiv(distance[1], SquareRoot(distance[1] * distance[1] + distance[0] * distance[0])); //Ratio y/hypo
			
			addVel[0] = FloatMul(ratio[0] * -1, power);
			addVel[1] = FloatMul(ratio[1] * -1, power);
			addVel[2] = power;
			FlingPlayer(i, addVel, i);
		}
	}
}

public Action:timerStopFire(Handle:timer, Handle:pack)
{
	ResetPack(pack);
	new particle = ReadPackCell(pack);
	new hurt = ReadPackCell(pack);
	if (IsValidEntity(particle))
	{
		AcceptEntityInput(particle, "Stop");
	}
	if (IsValidEntity(hurt))
	{
		AcceptEntityInput(hurt, "TurnOff");
	}
}

stock FlingPlayer(target, Float:vector[3], attacker, Float:stunTime = 3.0)
{
	SDKCall(sdkCallPushPlayer, target, vector, 96, attacker, stunTime);
}