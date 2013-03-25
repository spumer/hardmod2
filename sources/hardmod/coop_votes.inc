new Handle:hm_blockvote_kick;
new Handle:hm_blockvote_map;
new Handle:hm_allowvote_map_players;
new Handle:hm_allowvote_map_fails;
new Handle:hm_blockvote_lobby;
new Handle:hm_blockvote_restart;
new Handle:hm_blockvote_difficulty;
new Handle:hm_blockvote_difference;
new Handle:hm_allowvote_mission;

new String:CV_FileName[MAX_FILE_NAME_LENGTH];

public CVotesOnPluginStart()
{
	Version += 11;
	RegAdminCmd("sm_veto", Command_Veto, ADMFLAG_ROOT, "sm_veto");
	RegConsoleCmd("sm_kicktarget", Command_KickTarget);
	RegConsoleCmd("callvote", Callvote_Handler);
	hm_blockvote_kick = CreateConVar("hm_blockvote_kick", "1", "", FCVAR_PLUGIN);
	hm_blockvote_map = CreateConVar("hm_blockvote_map", "1", "", FCVAR_PLUGIN);
	hm_allowvote_map_players = CreateConVar("hm_allowvote_map_players", "6", "", FCVAR_PLUGIN);
	hm_allowvote_map_fails = CreateConVar("hm_allowvote_map_fails", "5", "", FCVAR_PLUGIN);
	hm_blockvote_lobby = CreateConVar("hm_blockvote_lobby", "1", "", FCVAR_PLUGIN);
	hm_blockvote_restart = CreateConVar("hm_blockvote_restart", "1", "", FCVAR_PLUGIN);
	hm_blockvote_difficulty = CreateConVar("hm_blockvote_difficulty", "1", "", FCVAR_PLUGIN);
	hm_blockvote_difference = CreateConVar("hm_blockvote_difference", "0", "", FCVAR_PLUGIN);
	hm_allowvote_mission = CreateConVar("hm_allowvote_mission", "20", "", FCVAR_PLUGIN);
	BuildPath(Path_SM, CV_FileName, sizeof(CV_FileName), "hardmod/forbiddenmaps.txt");
}

IsMissionAllowed(String:map_name[MAX_MAP_NAME_LENGTH])
{
	Log("coop_votes.inc / IsMissionAllowed() / 1", 4);
	new result = 1;
	new Handle:file = OpenFile(CV_FileName, "r");
	if (file == INVALID_HANDLE)
	{
		Log("coop_votes.inc / IsMissionAllowed() / file == INVALID_HANDLE", 1);
		return result;
	}
	
	FileSeek (file, 0, SEEK_SET);
	new String:CV_StoredMap[MAX_MAP_NAME_LENGTH];
	while (!IsEndOfFile(file))
	{
		// we read the line
		if (ReadFileLine(file, CV_StoredMap, sizeof(CV_StoredMap)))
		{
			TrimString(CV_StoredMap);
			if (StrEqual(map_name, CV_StoredMap, false))
			{
				result = 0;
				break;
			}
		}
		else
		{
			break;
		}
	}
	CloseHandle(file);
	return result;
}

public Action:Command_Veto(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsValidEntity(i) && IsClientInGame(i) && !IsFakeClient(i))
		{
			FakeClientCommand(i, "Vote No");
		}
	}
}

public Action:Command_KickTarget(client, args)
{
	if (client < 1)
	{
		return Plugin_Handled;
	}
	if (GetClientTeam(client) != TEAM_SURVIVORS)
	{
		return Plugin_Handled;
	}
	new target = GetClientAimTarget(client, false);
	if (!IsValidClient(target))
	{
		return Plugin_Handled;
	}
	if (GetClientTeam(target) != TEAM_SURVIVORS)
	{
		return Plugin_Handled;
	}
	FakeClientCommand(client, "callvote kick %d", GetClientUserId(target));
	return Plugin_Continue;
}

public Action:Callvote_Handler(client, args)
{
	if (client == 0)
	{
//		PrintToChatAll("\x05Server \x04 the voting");
		return Plugin_Continue;
	}

	decl String:voteName[MAX_NAME_LENGTH];
	decl String:initiatorName[MAX_NAME_LENGTH];
	GetClientName(client, initiatorName, sizeof(initiatorName));
	GetCmdArg(1, voteName, sizeof(voteName));
	
//	PrintToChatAll("\x03VOTE = \x05%s", voteName);
	
	if (strcmp(voteName,"Kick", false) == 0)
	{
		// this function must return either Plugin_Handled or Plugin_Continue
		return Kick_Vote_Logic(client, args);
	}
	if (strcmp(voteName, "ReturnToLobby", false) == 0) 
	{
		if (GetConVarInt(hm_blockvote_lobby) > 0)
		{
			PrintToChat(client, "\x04\"Vote\" access denied");
			return Plugin_Handled;
		}
		else
		{
			PrintToChat(client, "\x03\"Vote\" access granted");
//			PrintToChatAll("\x04%N started the voting", client);
			return Plugin_Continue;
		}
	}
	if (strcmp(voteName, "ChangeMission", false) == 0 || strcmp(voteName, "ChangeChapter", false) == 0) 
	{
		decl String:map_name[MAX_MAP_NAME_LENGTH];
		GetCmdArg(2, map_name, sizeof(map_name));
		if (!IsMissionAllowed(map_name))
		{
			new AdminId:ClientAdminId = GetUserAdmin(client);
			new flags = GetAdminFlags(ClientAdminId, Access_Effective);
			if (flags & ADMFLAG_VOTE || flags & ADMFLAG_CHANGEMAP) //  || flags & ADMFLAG_ROOT
			{
				CPrintToChat(client, "{red}Warning! This campaign is forbidden!\n\x03\"Vote\" access granted");
				return Plugin_Continue;
			}
			PrintToChat(client, "\x04\"Vote\" access denied [this campaign is forbidden]");
			return Plugin_Handled;
		}
		if (GetConVarInt(hm_allowvote_map_players) >= GetRealClientCount())
		{
			PrintToChat(client, "\x03\"Vote\" access granted");
//			PrintToChatAll("\x04%N started the voting", client);
			return Plugin_Continue;
		}
		if (GetConVarInt(hm_blockvote_map) > 0 && rounds < GetConVarInt(hm_allowvote_map_fails))
		{
			new AdminId:ClientAdminId = GetUserAdmin(client);
			new flags = GetAdminFlags(ClientAdminId, Access_Effective);
#if PLAYERS_ENABLED			
			if (PlayerVIP[client] > 0)
			{
				PrintToChat(client, "\x05[VIP] \x03\"Vote\" access granted");
//				PrintToChatAll("\x04%N started the voting", client);
				return Plugin_Continue;			
			}
#endif
			if (GetAdminLvl(client) > GetConVarInt(hm_allowvote_mission))
			{
				PrintToChat(client, "\x03\"Vote\" access granted");
//				PrintToChatAll("\x04%N started the voting", client);
				return Plugin_Continue;			
			}
			if (flags & ADMFLAG_VOTE || flags & ADMFLAG_CHANGEMAP) //  || flags & ADMFLAG_ROOT
			{
				PrintToChat(client, "\x03\"Vote\" access granted");
//				PrintToChatAll("\x04%N started the voting", client);
				return Plugin_Continue;
			}
			else
			{
				PrintToChat(client, "\x04\"Vote\" access denied [%d < %d]", GetAdminLvl(client), GetConVarInt(hm_allowvote_mission));
				return Plugin_Handled;
			}
		}
		else
		{
			PrintToChat(client, "\x03\"Vote\" access granted");
//			PrintToChatAll("\x04%N started the voting", client);
			return Plugin_Continue;
		}
	}
	if (strcmp(voteName, "RestartGame", false) == 0) 
	{
		if (GetConVarInt(hm_blockvote_restart) < 1)
		{
			PrintToChat(client, "\x03\"Vote\" access granted");
			return Plugin_Continue;
		}
		else
		{
			if (GetConVarInt(hm_blockvote_restart) == 1)
			{
				if (IsPlayerAlive(client))
				{
					PrintToChat(client, "\x05\"Vote\" access granted");
//					PrintToChatAll("\x04%N started the voting", client);
					return Plugin_Continue;
				}
				else
				{
					new AdminId:ClientAdminId = GetUserAdmin(client);
					new flags = GetAdminFlags(ClientAdminId, Access_Effective);
					if (flags & ADMFLAG_VOTE || flags & ADMFLAG_CHANGEMAP || flags & ADMFLAG_ROOT)
					{
						PrintToChat(client, "\x03\"Vote\" access granted");
//						PrintToChatAll("\x04%N started the voting", client);
						return Plugin_Continue;			
					}
					else
					{
						PrintToChat(client, "\x04\"Vote\" access denied");
						return Plugin_Handled;
					}
				}
			}
			else
			{
				PrintToChat(client, "\x04\"Vote\" access denied");
				return Plugin_Handled;
			}
		}
	}
	if (strcmp(voteName, "ChangeDifficulty", false) == 0) 
	{
		if (GetConVarInt(hm_blockvote_difficulty) > 0)
		{
			new AdminId:ClientAdminId = GetUserAdmin(client);
			new flags = GetAdminFlags(ClientAdminId, Access_Effective);
			if (flags & ADMFLAG_VOTE & ADMFLAG_CONVARS || flags & ADMFLAG_ROOT)
			{
				PrintToChat(client, "\x03\"Vote\" access granted");
//				PrintToChatAll("\x04%N started the voting", client);
				return Plugin_Continue;			
			}
			else
			{
				PrintToChat(client, "\x04\"Vote\" access denied");
				return Plugin_Handled;
			}
		}
		else
		{
			PrintToChat(client, "\x03\"Vote\" access granted");
//			PrintToChatAll("\x04%N started the voting", client);
			return Plugin_Continue;
		}
	}

//	PrintToChatAll("VOTE = %s", voteName);
	return Plugin_Continue;
}

public Action:Kick_Vote_Logic(client, args)
{
	decl String:initiatorName[MAX_NAME_LENGTH];
	GetClientName(client, initiatorName, sizeof(initiatorName));

	decl String:arg2[12];
	GetCmdArg(2, arg2, sizeof(arg2));
	new target = GetClientOfUserId(StringToInt(arg2));
	
	if (!target) return Plugin_Handled;

	new AdminId:ClientAdminId = GetUserAdmin(client);
	new AdminId:TargetAdminId = GetUserAdmin(target);
	
	if (GetConVarInt(hm_blockvote_kick) < 1)
	{
		PrintToChat(client, "\x03\"Vote\" access granted");
		return Plugin_Continue;
	}
	
	new flags = GetAdminFlags(ClientAdminId, Access_Effective);
	if (flags & ADMFLAG_VOTE || flags & ADMFLAG_ROOT || GetConVarInt(hm_blockvote_kick) < 2) 
	{
		if (GetAdminLvl(client) - GetConVarInt(hm_blockvote_difference) >= GetAdminLvl(target))
		{
			new flags2 = GetAdminFlags(TargetAdminId, Access_Effective);
			if (flags2 & ADMFLAG_GENERIC || flags2 & ADMFLAG_ROOT)
			{
				PrintToChat(client, "\x04\"Vote\" access denied");
				return Plugin_Handled;
			}
			else
			{
				PrintToChat(client, "\x03\"Vote\" access granted");
			}
		}
		else
		{
			PrintToChat(client, "\x04\"Vote\" access denied");
			return Plugin_Handled;
		}
	}
	else
	{
		PrintToChat(client, "\x04\"Vote\" access denied");
		return Plugin_Handled;
	}
//	PrintToChatAll("%N started the voting", client);
	return Plugin_Continue;
}