public CheatOnPluginStart()
{
	Version += 6;
	RegAdminCmd("sm_cmd", Command_CheatCmd, ADMFLAG_ROOT, "sm_cmd command parameters");
	RegAdminCmd("sm_cmdall", Command_CheatCmdAll, ADMFLAG_ROOT, "sm_cmdall command parameters");
	RegAdminCmd("sm_cmdtarget", Command_CheatCmdTarget, ADMFLAG_ROOT, "sm_cmdtarget <target in-game> command parameters");
	RegAdminCmd("sm_spawnnewitem", Command_SpawnNewItem, ADMFLAG_RCON, "sm_spawnnewitem <itemname>");
	RegAdminCmd("sm_spawnitem", Command_SpawnItem, ADMFLAG_CHANGEMAP, "sm_spawnitem <parameters>");	
}

public Action:Command_CheatCmd(client, args)
{
	if (args < 1)
	{
		return Plugin_Handled;
	}

	decl String:command_text[192];
	GetCmdArg(1, command_text, sizeof(command_text));
	
	if (args > 1)
	{
		decl String:parameters_text[192];
		parameters_text = "";
		decl String:temp_text[40];
		for (new i = 2; i <= args; i++)
		{
			GetCmdArg(i, temp_text, sizeof(temp_text));
			StrCat(parameters_text, sizeof(parameters_text), temp_text);
		}

		CheatCMD(client, command_text, parameters_text);
		return Plugin_Continue;
	}
	
	CheatCMD(client, command_text, "");
	return Plugin_Continue;
}

public Action:Command_CheatCmdAll(client, args)
{
	if (args < 1)
	{
		return Plugin_Handled;
	}

	decl String:command_text[192];
	GetCmdArg(1, command_text, sizeof(command_text));
	decl String:parameters_text[192];	
	if (args > 1)
	{
		parameters_text = "";
		decl String:temp_text[40];
		for (new i = 2; i <= args; i++)
		{
			GetCmdArg(i, temp_text, sizeof(temp_text));
			if (i < args) Format(temp_text, sizeof(temp_text), "%s ", temp_text);
			StrCat(parameters_text, sizeof(parameters_text), temp_text);
		}
	}
	else
	{
		parameters_text = "";
	}

	new CheatCMD_flags = GetCommandFlags(command_text);
	SetCommandFlags(command_text, CheatCMD_flags & ~FCVAR_CHEAT);
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i) && IsClientInGame(i) && !IsFakeClient(i))
		{
			FakeClientCommand(i, "%s %s", command_text, parameters_text);
		}
	}
	SetCommandFlags(command_text, CheatCMD_flags);	
	return Plugin_Continue;
}

public Action:Command_CheatCmdTarget(client, args)
{
	if (args < 1)
	{
		return Plugin_Handled;
	}
	new target = 0;
//	if (args < 3)
	{
		target = GetClientAimTarget(client, false);
		if (!IsValidClient(target))
		return Plugin_Handled;
	}

	decl String:command_text[192];
	GetCmdArg(1, command_text, sizeof(command_text));

	if (args > 1)
	{
		decl String:parameters_text[192];
		parameters_text = "";
		decl String:temp_text[40];
		for (new i = 2; i <= args; i++)
		{
			GetCmdArg(i, temp_text, sizeof(temp_text));
			if (i < args) Format(temp_text, sizeof(temp_text), "%s ", temp_text);
			StrCat(parameters_text, sizeof(parameters_text), temp_text);
		}

		CheatCMD(target, command_text, parameters_text);
		return Plugin_Continue;
	}

	CheatCMD(client, command_text, "");
	return Plugin_Continue;
}

public bool:IsCheat(const String:Command[])
{
	if (GetCommandFlags(Command) & FCVAR_CHEAT)
	{
		return true;
	}
	return false;
}

public Action:Command_SpawnNewItem(client, args)
{
	decl Float:VecOrigin[3], Float:VecAngles[3], Float:VecDirection[3];
	
	decl String:text[192];
	if (!GetCmdArgString(text, sizeof(text)))
	{
		return Plugin_Continue;
	}

	new startidx = 0;

	if (text[strlen(text) - 1] == '"')
	{
		text[strlen(text) - 1] = '\0';
		startidx = 1;
	}

	new NewItem = CreateEntityByName(text[startidx]);

	if (NewItem == -1 || NewItem == 0)
	{
		ReplyToCommand(client, "[SM] Spawn Failed: %s", text[startidx]);
	}

	DispatchKeyValue(NewItem, "model", "newitem");
	DispatchKeyValueFloat (NewItem, "MaxPitch", 360.00);
	DispatchKeyValueFloat (NewItem, "MinPitch", -360.00);
	DispatchKeyValueFloat (NewItem, "MaxYaw", 90.00);
	DispatchSpawn(NewItem);

	GetClientAbsOrigin(client, VecOrigin);
	GetClientEyeAngles(client, VecAngles);
	GetAngleVectors(VecAngles, VecDirection, NULL_VECTOR, NULL_VECTOR);
	VecOrigin[0] += VecDirection[0] * 32;
	VecOrigin[1] += VecDirection[1] * 32;
	VecOrigin[2] += VecDirection[2] * 1;   
	VecAngles[0] = 0.0;
	VecAngles[2] = 0.0;

	PrintToChat(client, "\x03sm_spawnitem %s %f %f %f %f %f %f %f", text[startidx], VecDirection[0], VecDirection[1], VecDirection[2], VecOrigin[0], VecOrigin[1], VecOrigin[2], VecAngles[1]);

	DispatchKeyValueVector(NewItem, "Angles", VecAngles);
	DispatchSpawn(NewItem);
	TeleportEntity(NewItem, VecOrigin, NULL_VECTOR, NULL_VECTOR);
	
	return Plugin_Continue;
}

public Action:Command_SpawnItem(client, args)
{
	if (args < 8)
	{
		ReplyToCommand(client, "[SM] Usage: sm_spawnitem <parameters>");
		return Plugin_Handled;
	}

	decl Float:VecDirection[3];
	decl Float:VecOrigin[3];
	decl Float:VecAngles[3];
	decl String:modelname[64];
	GetCmdArg(1, modelname, sizeof(modelname));

	decl String:TempString[20];
	GetCmdArg(2, TempString, sizeof(TempString));
	VecDirection[0] = StringToFloat(TempString);
	GetCmdArg(3, TempString, sizeof(TempString));
	VecDirection[1] = StringToFloat(TempString);
	GetCmdArg(4, TempString, sizeof(TempString));
	VecDirection[2] = StringToFloat(TempString);
	GetCmdArg(5, TempString, sizeof(TempString));
	VecOrigin[0] = StringToFloat(TempString);
	GetCmdArg(6, TempString, sizeof(TempString));
	VecOrigin[1] = StringToFloat(TempString);
	GetCmdArg(7, TempString, sizeof(TempString));
	VecOrigin[2] = StringToFloat(TempString);
	GetCmdArg(8, TempString, sizeof(TempString));
	VecAngles[0] = 0.0;
	VecAngles[1] = StringToFloat(TempString);
	VecAngles[2] = 0.0;
	
	new spawned_item = CreateEntityByName(modelname);
	if (!IsValidEntity(spawned_item))
	{
		if (client == 0)
		{
			PrintToServer("[sm_spawnitem] Invalid Item Name: %s", modelname);
		}
		else
		{
			ReplyToCommand(client, "[sm_spawnitem] Invalid Item Name: %s", modelname);
		}
		return Plugin_Handled;
	}

	DispatchKeyValue(spawned_item, "model", "Custom_Spawn");
	DispatchKeyValueFloat(spawned_item, "MaxPitch", 360.00);
	DispatchKeyValueFloat(spawned_item, "MinPitch", -360.00);
	DispatchKeyValueFloat(spawned_item, "MaxYaw", 90.00);
	DispatchSpawn(spawned_item);

	DispatchKeyValueVector(spawned_item, "Angles", VecAngles);
	DispatchSpawn(spawned_item);
	TeleportEntity(spawned_item, VecOrigin, NULL_VECTOR, NULL_VECTOR);

	return Plugin_Continue;
}

public CheatCMDRealClient(any:client, const String:Command[], const String:Parameters[])
{
	new realclient = GetAnyRealClient();
	if (realclient == 0)
	{
		return;
	}
	CheatCMD(realclient, Command, Parameters);
}

public CheatCMD(any:client, const String:Command[], const String:Parameters[])
{
	new CheatCMD_flags = GetCommandFlags(Command);
	SetCommandFlags(Command, CheatCMD_flags & ~FCVAR_CHEAT);
	if (client == 0)
	{
		client = GetAnyClient();
	}
	if (client == 0)
	{
		new fakeclient = 0;
		fakeclient = CreateFakeClient("FakeClient");
		if (fakeclient != 0)
		{
			FakeClientCommand(fakeclient, "%s %s", Command, Parameters);
			ServerKickClient(fakeclient);
			SetCommandFlags(Command, CheatCMD_flags);
			return;
		}
	}
	if (client == 0)
	{
		ServerCommand("%s %s", Command, Parameters);
	}
	else
	{
		FakeClientCommand(client, "%s %s", Command, Parameters);
	}
	SetCommandFlags(Command, CheatCMD_flags);
}

public SpawnItem(any:client, const String:ItemName[])
{
	decl Float:VecOrigin[3], Float:VecAngles[3], Float:VecDirection[3];

	new SpawnItemEntity = CreateEntityByName(ItemName);

	if (SpawnItemEntity == -1)
	{
		ReplyToCommand(client, "\x05[SM] \x03 Spawn Failed (\x01%s\x03)", ItemName);
	}

	DispatchKeyValue(SpawnItemEntity, "model", "spawn_entity_1");
	DispatchKeyValueFloat (SpawnItemEntity, "MaxPitch", 360.00);
	DispatchKeyValueFloat (SpawnItemEntity, "MinPitch", -360.00);
	DispatchKeyValueFloat (SpawnItemEntity, "MaxYaw", 90.00);
	DispatchSpawn(SpawnItemEntity);

	GetClientAbsOrigin(client, VecOrigin);
	GetClientEyeAngles(client, VecAngles);
	GetAngleVectors(VecAngles, VecDirection, NULL_VECTOR, NULL_VECTOR);
	VecOrigin[0] += VecDirection[0] * 32;
	VecOrigin[1] += VecDirection[1] * 32;
	VecOrigin[2] += VecDirection[2] * 1;   
	VecAngles[0] = 0.0;
	VecAngles[2] = 0.0;

	DispatchKeyValueVector(SpawnItemEntity, "Angles", VecAngles);
	DispatchSpawn(SpawnItemEntity);
	TeleportEntity(SpawnItemEntity, VecOrigin, NULL_VECTOR, NULL_VECTOR);
}