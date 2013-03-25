SpawnLamp(vPos, vAng, color, brightness, glow);
{

}

SpawnLamp(const Float:vOrigin[3], const Float:vAngles[3], color, Float:brightness, Float:glow)
{
	// GET INDEX
	new index = -1;
	for( new i = 0; i < MAX_ALLOWED; i++ )
	{
		if( !IsValidEntRef(g_iEntities[i][0]) )
		{
			index = i;
			break;
		}
	}

	if( index == -1 )
		return -1;

	// CREATE ENTITY
	new entity;

	entity = CreateEntityByName("prop_dynamic");
	DispatchKeyValue(entity, "solid", "6");

	// SET MODEL
	DispatchKeyValue(entity, "model", "models/props/cs_office/light_inset.mdl");

	// SET SKIN
	SetEntProp(entity, Prop_Send, "m_nSkin", 1);

	// DISPATCH
	DispatchKeyValue(entity, "health", "50");
	DispatchSpawn(entity);

	// SET POSITION
	new Float:vAng[3];
	new target = entity;

	entity = MakeLightDynamic(Float:{ 0.0, 0.0, -20.0 }, vAng, color, brightness);

	ParentEntities(target, entity);
	TeleportEntity(target, vOrigin, vAngles, NULL_VECTOR);

	return index;
}

MakeLightDynamic(Float:vOrigin[3], Float:vAngles[3], color, Float:brightness)
{
	new entity = CreateEntityByName("light_dynamic");
	if( entity == -1)
		return 0;

	DispatchKeyValue(entity, "_light", "0 0 0 255");
	DispatchKeyValue(entity, "brightness", "1");
	DispatchKeyValueFloat(entity, "spotlight_radius", 32.0);
	DispatchKeyValueFloat(entity, "distance", brightness);
	DispatchKeyValue(entity, "style", "0");
	SetEntProp(entity, Prop_Send, "m_clrRender", color);

	DispatchSpawn(entity);
	AcceptEntityInput(entity, "TurnOn");
	TeleportEntity(entity, vOrigin, vAngles, NULL_VECTOR);

	return entity;
}

GetColor(String:sTemp[])
{
	decl String:sColors[3][4];
	ExplodeString(sTemp, " ", sColors, 3, 4);

	new color;
	color = StringToInt(sColors[0]);
	color += 256 * StringToInt(sColors[1]);
	color += 65536 * StringToInt(sColors[2]);
	return color;
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