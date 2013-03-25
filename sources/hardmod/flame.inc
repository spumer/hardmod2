// © http://forums.alliedmods.net/showthread.php?p=1251446

#define BURN_IGNITE_PARTICLE "fire_small_01"
#define FLAME_FULL_HEALTH true

FlameOnMapStart()
{
	PrecacheParticle(BURN_IGNITE_PARTICLE);
}

stock IgnitePlayer(client, Float:duration)
{
	new team = GetClientTeam(client);
	if(team != 2)
	{
		IgniteEntity(client, duration);
	}
	else
	{
		decl Float:pos[3];
		GetClientAbsOrigin(client, pos);
		decl String:sUser[256];
		IntToString(GetClientUserId(client)+25, sUser, sizeof(sUser));
		CreateParticle(client, BURN_IGNITE_PARTICLE, true, duration);
		new Damage = CreateEntityByName("point_hurt");
		if (IsIncapacitated(client))
		{
			DispatchKeyValue(Damage, "Damage", "4");
		}
		else
		{
			DispatchKeyValue(Damage, "Damage", "1");
		}
		DispatchKeyValue(Damage, "DamageType", "8");
		DispatchKeyValue(client, "targetname", sUser);
		DispatchKeyValue(Damage, "DamageTarget", sUser);
		DispatchSpawn(Damage);
		TeleportEntity(Damage, pos, NULL_VECTOR, NULL_VECTOR);
		AcceptEntityInput(Damage, "Hurt");
#if FLAME_FULL_HEALTH		
		SetEntityHealth(client, 150);
		SetTempHealth(client, 0);
#endif		
		ChangeSpeed(client, 2.0);
		CreateTimer(0.1, timerHurtMe, Damage, TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
		CreateTimer(duration, timerResetSpeed, client, TIMER_FLAG_NO_MAPCHANGE);
		CreateTimer(duration, timerStopAndRemoveParticle, Damage, TIMER_FLAG_NO_MAPCHANGE);
	}
}

stock CreateParticle(client, String:Particle_Name[], bool:Parent, Float:duration)
{
	decl Float:pos[3], String:sName[64], String:sTargetName[64];
	new Particle = CreateEntityByName("info_particle_system");
	GetClientAbsOrigin(client, pos);
	TeleportEntity(Particle, pos, NULL_VECTOR, NULL_VECTOR);
	DispatchKeyValue(Particle, "effect_name", Particle_Name);
	
	if(Parent)
	{
		new userid = GetClientUserId(client);
		Format(sName, sizeof(sName), "%d", userid+25);
		DispatchKeyValue(client, "targetname", sName);
		GetEntPropString(client, Prop_Data, "m_iName", sName, sizeof(sName));
		
		Format(sTargetName, sizeof(sTargetName), "%d", userid+1000);
		DispatchKeyValue(Particle, "targetname", sTargetName);
		DispatchKeyValue(Particle, "parentname", sName);
	}
	DispatchSpawn(Particle);
	DispatchSpawn(Particle);
	if(Parent)
	{
		SetVariantString(sName);
		AcceptEntityInput(Particle, "SetParent", Particle, Particle);
	}
	ActivateEntity(Particle);
	AcceptEntityInput(Particle, "start");
	CreateTimer(duration, timerStopAndRemoveParticle, Particle, TIMER_FLAG_NO_MAPCHANGE);
}

public Action:timerHurtMe(Handle:timer, any:hurt)
{
	if(IsValidEntity(hurt) && IsValidEdict(hurt))
	{
		AcceptEntityInput(hurt, "Hurt");
		return Plugin_Continue;
	}
	return Plugin_Stop;
}

public Action:timerResetSpeed(Handle:timer, any:client)
{
	ChangeSpeed(client, 1.0);
}

public Action:timerStopAndRemoveParticle(Handle:timer, any:entity)
{
	if(entity > 0 && IsValidEntity(entity) && IsValidEdict(entity))
	{
		AcceptEntityInput(entity, "Kill");
	}
}