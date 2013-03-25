new Handle:l4d2_blockvote_kick;
new Handle:l4d2_blockvote_map;
new Handle:l4d2_blockvote_lobby;
new Handle:l4d2_blockvote_restart;

public SVotesOnPluginStart()
{
	RegConsoleCmd("callvote", Callvote_Handler);
	l4d2_blockvote_kick = CreateConVar("l4d2_blockvote_kick", "1", "", FCVAR_PLUGIN);
	l4d2_blockvote_map = CreateConVar("l4d2_blockvote_map", "1", "", FCVAR_PLUGIN);
	l4d2_blockvote_lobby = CreateConVar("l4d2_blockvote_lobby", "1", "", FCVAR_PLUGIN);
	l4d2_blockvote_restart = CreateConVar("l4d2_blockvote_restart", "1", "", FCVAR_PLUGIN);
}

public Action:Callvote_Handler(client, args)
{
	if (client == 0)
	{
		PrintToChatAll("\x05Server \x04started the voting");
		return Plugin_Continue;
	}

	decl String:voteName[32];
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
		if (GetConVarInt(l4d2_blockvote_lobby) > 0)
		{
			PrintToChat(client, "\x04\"Vote\" access denied");
			return Plugin_Handled;
		}
		else
		{
			PrintToChat(client, "\x04\"Vote\" access granted");
			PrintToChatAll("\x04%N started the voting", client);
			return Plugin_Continue;
		}
	}
	if (strcmp(voteName, "ChangeMission", false) == 0 || strcmp(voteName, "ChangeChapter", false) == 0) 
	{
		if (GetConVarInt(l4d2_blockvote_map) > 0 && rounds < 1)
		{
			new AdminId:ClientAdminId = GetUserAdmin(client);
			new flags = GetAdminFlags(ClientAdminId, Access_Effective);
			if (flags & ADMFLAG_VOTE || flags & ADMFLAG_CHANGEMAP || flags & ADMFLAG_ROOT)
			{
				PrintToChat(client, "\x04\"Vote\" access granted");
				PrintToChatAll("\x04%N started the voting", client);
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
			PrintToChat(client, "\x04\"Vote\" access granted");
			PrintToChatAll("\x04%N started the voting", client);
			return Plugin_Continue;
		}
	}
	if (strcmp(voteName, "RestartGame", false) == 0) 
	{
		if (GetConVarInt(l4d2_blockvote_restart) < 1)
		{
			PrintToChat(client, "\x04\"Vote\" access granted");
			return Plugin_Continue;
		}
		else
		{
			if (GetConVarInt(l4d2_blockvote_restart) == 1)
			{
				if (IsPlayerAlive(client))
				{
					PrintToChat(client, "\x05\"Vote\" access granted");
					PrintToChatAll("\x04%N started the voting", client);
					return Plugin_Continue;
				}
				else
				{
					new AdminId:ClientAdminId = GetUserAdmin(client);
					new flags = GetAdminFlags(ClientAdminId, Access_Effective);
					if (flags & ADMFLAG_VOTE || flags & ADMFLAG_CHANGEMAP || flags & ADMFLAG_ROOT)
					{
						PrintToChat(client, "\x04\"Vote\" access granted");
						PrintToChatAll("\x04%N started the voting", client);
						return Plugin_Continue;			
					}
					else
					{
						PrintToChat(client, "\x04\"Vote\" access denied");
						return Plugin_Handled;
					}
				}
			}
		}
	}
	return Plugin_Continue;
}

public Action:Kick_Vote_Logic(client, args)
{
	decl String:initiatorName[MAX_NAME_LENGTH];
	GetClientName(client, initiatorName, sizeof(initiatorName));

	decl String:arg2[12];
	GetCmdArg(2, arg2, sizeof(arg2));
	new target = GetClientOfUserId(StringToInt(arg2));

	new AdminId:ClientAdminId = GetUserAdmin(client);
	new AdminId:TargetAdminId = GetUserAdmin(target);
	
	new flags = GetAdminFlags(ClientAdminId, Access_Effective);
	if (flags & ADMFLAG_VOTE || flags & ADMFLAG_ROOT || GetConVarInt(l4d2_blockvote_kick) < 1) 
	{
		if (GetAdminImmunityLevel(ClientAdminId) >= GetAdminImmunityLevel(TargetAdminId))
		{
			new flags2 = GetAdminFlags(TargetAdminId, Access_Effective);
			if (flags2 & ADMFLAG_GENERIC || flags2 & ADMFLAG_ROOT)
			{
				PrintToChat(client, "\x04\"Vote\" access denied");
				return Plugin_Handled;
			}
			else
			{
				PrintToChat(client, "\x04\"Vote\" access granted");
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
	PrintToChatAll("%N started the voting", client);
	return Plugin_Continue;
}

