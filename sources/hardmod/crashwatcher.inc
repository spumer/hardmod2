new bool:just_started;
new String:CW_FileName[MAX_FILE_NAME_LENGTH];
new String:CW_StoredMap[MAX_MAP_NAME_LENGTH];
new Handle:Cvar_LoadLastMap;

public CWOnPluginStart()
{
	Version += 5;
	just_started = true;
	BuildPath(Path_SM, CW_FileName, sizeof(CW_FileName), "hardmod/lastmap.txt");
	Cvar_LoadLastMap = CreateConVar("hm_loadlastmap", "1", "", FCVAR_PLUGIN);
	RegConsoleCmd("sm_lastmap", Command_LastMap);
}

CWOnMapStart()
{
	if (just_started)
	{
		LoadFile();
		if (!StrEqual(CW_StoredMap, current_map, false) && !StrEqual(CW_StoredMap, "", false))
		{
			if (GetConVarInt(Cvar_LoadLastMap) > 0)
			{
				Log("crashwatcher.sp / CWOnMapStart() / crash found", 1);
//				CheatCMD(0, "give", "rifle_sg552");
//				CheatCMD(0, "give", "sniper_awp");
//				CheatCMD(0, "give", "sniper_scout");
//				CheatCMD(0, "give", "smg_mp5");
				SetConVarInt(Cvar_LoadLastMap, 0, false, false);
				ServerCommand("changelevel %s", CW_StoredMap);
//				CreateTimer(3.0, TimedChangeMap, TIMER_FLAG_NO_MAPCHANGE);
			}
		}
	}
	else
	{
		ClearFile();
		SaveFile();
	}
	just_started = false;
}

public Action:TimedChangeMap(Handle:timer, any:client)
{
	ServerCommand("changelevel %s", CW_StoredMap);
}

public Action:Command_LastMap(client, args)
{
	Log("crashwatcher.sp / Command_LastMap() / 1", 4);
	LoadFile();
	if (client == 0)
	{
		PrintToServer("LastMap = \x04%s", CW_StoredMap);
	}
	else
	{
		PrintToChat(client, "\x05LastMap = \x04%s", CW_StoredMap);
	}
}

ClearFile()
{
	Log("crashwatcher.sp / ClearFile() / 1", 4);
	new Handle:file = OpenFile(CW_FileName, "w+");
	if (file == INVALID_HANDLE)
	{
		Log("crashwatcher.sp / ClearFile() / file == INVALID_HANDLE", 4);
		return;
	}

	CloseHandle(file);
}

SaveFile()
{
	new Handle:file = OpenFile(CW_FileName, "a+");
	if (file == INVALID_HANDLE)
	{
		return;
	}

	// We find the end of the file
	FileSeek(file, 0, SEEK_SET);

	// We store the info into the file
	if (!WriteFileLine (file, "%s", current_map))
	{
		// Error, so we abort ...
		CloseHandle(file);
		return;
	}
	
//	CW_StoredMap = current_map;

	CloseHandle(file);
}

LoadFile()
{
	Log("crashwatcher.sp / LoadFile() / 1", 4);
	// We read the file
	new Handle:file = OpenFile(CW_FileName, "r");
	if (file == INVALID_HANDLE)
	{
		Log("crashwatcher.sp / LoadFile() / file == INVALID_HANDLE", 1);
		return;
	}

	FileSeek (file, 0, SEEK_SET);

	while (!IsEndOfFile(file))
	{
		// we read the line
		if (!ReadFileLine(file, CW_StoredMap, sizeof(CW_StoredMap)))
		{
			break;
		}
	}
	CloseHandle(file);
}