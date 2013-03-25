// Based on http://forums.alliedmods.net/showthread.php?p=1140170
// Based on http://forums.alliedmods.net/showthread.php?p=1140170
#include <sourcemod>
#pragma semicolon 1

#define DATA_FILENAME "players_stats.txt"
#define HEAL_POINTS true
#define REMOVEDEADSTATS_DAYS 21

#define STATS_VERSION 62

new ChachedPoints[L4D_MAXPLAYERS + 1];
new Float:LastPrintTime[L4D_MAXPLAYERS + 1];

new Witch_Points_Cost = 15;
new Witch_Points_Crown_Cost = 50;
new Common_Points = 1;
new Common_Points_HeadShot = 2;
new Boss_Points = 5;
new Boss_Points_HeadShot = 8;
new Player_Suicide_Points = -50;
new Player_Death_Points = -50;
new Player_SelfIncap_Points = -7;
new Player_Incap_Points = -3;
new Defib_Points = 25;
new Vomit_Self_Points = -15;
new Vomit_Teammate_Points = 5;
new Vomit_Tank_Points = 60;
new PrizeBox_Penalty = -75;
new FirstRoundPoints = 50;
new Weapon_Pain_Pills_Points = 25;
new Weapon_Adrenaline_Points = 10;
new Weapon_MedKit_Points = 50;
new Weapon_Defibrillator_Points = 25;
new FirstRoundTimerCountdown;
new bool:FirstRoundTimerEnabled = false;
#if HEAL_POINTS
	new Heal_Points_Cost_Alive = -3;
	new Heal_Points_Cost_Bad = -7;
	new Heal_Points_Cost_Really_Bad = 15;
	new Pills_Points_Cost_Alive = -1;
	new Pills_Points_Cost_Bad = -3;
	new Pills_Points_Cost_Really_Bad = 5;
#endif
new Handle:hm_stats_file;
new Handle:hm_statsmode;
new Handle:hm_usemapmultipliers;
new Handle:hm_countaliveplayers;
new Handle:hm_saveplayerscountry;
new Handle:hm_stats_maxplayers;
new Handle:hm_stats_checkonfirstround;
new Handle:hm_stats_checkonmapstart;
new Handle:hm_stats_colors;
new Handle:hm_stats_bot_colors;

new bool:Top15Used = false;
new bool:draw_points = true;
new bool:draw_chached_points = true;
new bool:save_points_flag = false;
new bool:show_stats_messages = true;

new ClientRank[L4D_MAXPLAYERS + 1];
new ClientPoints[L4D_MAXPLAYERS + 1];
new bool:ShowPointsTimerEnabled[L4D_MAXPLAYERS + 1];

new Total_Players = 0;
new Alive_Players;

new String:datafilepath[PLATFORM_MAX_PATH];

public CoopStatsOnPluginStart()
{
	hm_stats_file = CreateConVar("hm_stats_file", DATA_FILENAME, "", FCVAR_PLUGIN);
	hm_statsmode = CreateConVar("hm_statsmode", "2", "", FCVAR_PLUGIN);
	hm_usemapmultipliers = CreateConVar("hm_usemapmultipliers", "1", "", FCVAR_PLUGIN);
	hm_countaliveplayers = CreateConVar("hm_countaliveplayers", "1", "", FCVAR_PLUGIN);
	hm_saveplayerscountry = CreateConVar("hm_saveplayerscountry", "0", "", FCVAR_PLUGIN);
	hm_stats_maxplayers = CreateConVar("hm_stats_maxplayers", "1400", "", FCVAR_PLUGIN);
	hm_stats_checkonfirstround = CreateConVar("hm_stats_checkonfirstround", "0", "", FCVAR_PLUGIN);
	hm_stats_checkonmapstart = CreateConVar("hm_stats_checkonmapstart", "1", "", FCVAR_PLUGIN);
	hm_stats_colors = CreateConVar("hm_stats_colors", "0", "", FCVAR_PLUGIN);
	hm_stats_bot_colors = CreateConVar("hm_stats_bot_colors", "1", "", FCVAR_PLUGIN);
	
	RegAdminCmd("sm_setmapmultiplier", Command_SetupMapMultiplier, ADMFLAG_CUSTOM1, "sm_setmapmultiplier <float [0.0]...[X.0]>");
	RegAdminCmd("sm_getmapmultiplier", Command_ReadMapMultiplier, ADMFLAG_CUSTOM1, "sm_getmapmultiplier");
	RegAdminCmd("sm_resetmapmultipliers", Command_ResetMapMultipliers, ADMFLAG_CUSTOM1, "sm_resetmapmultipliers <float [0.0]...[X.0]>");
	RegAdminCmd("sm_randompoints", Command_RandomPoints, ADMFLAG_ROOT, "sm_randompoints");
	RegAdminCmd("sm_savepoints", Command_SavePoints, ADMFLAG_ROOT, "sm_savepoints");
	RegAdminCmd("sm_clearallpoints", Command_ClearAllPoints, ADMFLAG_ROOT, "sm_clearallpoints");
	RegAdminCmd("sm_addpoints", Command_AddPoints, ADMFLAG_ROOT, "sm_addpoints <steamid> <points>");
	RegAdminCmd("sm_top_all", Command_TopAll, ADMFLAG_ROOT, "sm_top_all <filename>");
	RegAdminCmd("sm_top30", Command_Top30, ADMFLAG_ROOT, "sm_top30");
	
	RegAdminCmd("sm_removedeadstats", Command_RemoveDeadStats, ADMFLAG_ROOT, "sm_removedeadstats <days, default = 21>");
	RegAdminCmd("sm_statschecklimit", Command_StatsCheckLimit, ADMFLAG_ROOT, "sm_statschecklimit <days, default = 21>");

	RegConsoleCmd("sm_maptop", Command_MapTop);
	RegConsoleCmd("sm_top15", Command_Top15);
	RegConsoleCmd("sm_rank", Command_Rank);
	RegConsoleCmd("sm_points", Command_Points);

	HookEvent("witch_killed", StatsEvent_WitchKilled);
	HookEvent("infected_death", StatsEvent_InfectedDeath);
	HookEvent("player_death", StatsEvent_PlayerDeath);
	HookEvent("player_incapacitated", StatsEvent_PlayerIncap);
#if HEAL_POINTS
	HookEvent("heal_success", StatsEvent_MedkitUsed, EventHookMode_Pre);
#endif
	HookEvent("defibrillator_used", StatsEvent_PlayerDefibed);
	HookEvent("finale_win", StatsEvent_FinalWin);
	
	BuildPath(Path_SM, datafilepath, sizeof(datafilepath), "hardmod/%s", DATA_FILENAME);
	HookConVarChange(hm_stats_file, hm_stats_file_changed);
}

public hm_stats_file_changed(Handle:hVariable, const String:strOldValue[], const String:strNewValue[])
{
	BuildStatsPath();
}

BuildStatsPath()
{
	new String:cvar_hm_stats_file[MAX_FILE_NAME_LENGTH];
	GetConVarString(hm_stats_file, cvar_hm_stats_file, sizeof(cvar_hm_stats_file));
	if (StrEqual(cvar_hm_stats_file, "", false) == true) return;
	decl String:str_date_yearly[10];
	FormatTime(str_date_yearly, sizeof(str_date_yearly), "%Y", GetTime());
	decl String:str_date_monthly[30];
	FormatTime(str_date_monthly, sizeof(str_date_monthly), "%Y_%B", GetTime());
	decl String:str_date_daily[30];
	FormatTime(str_date_daily, sizeof(str_date_daily), "%Y_%b_%d", GetTime());
	ReplaceString(cvar_hm_stats_file, sizeof(cvar_hm_stats_file), "%yearly%", str_date_yearly, false);
	ReplaceString(cvar_hm_stats_file, sizeof(cvar_hm_stats_file), "%monthly%", str_date_monthly, false);
	ReplaceString(cvar_hm_stats_file, sizeof(cvar_hm_stats_file), "%daily%", str_date_daily, false);
	BuildPath(Path_SM, datafilepath, sizeof(datafilepath), "hardmod/%s", cvar_hm_stats_file);
	if (!FileExists(datafilepath, false)) ServerCommand("exec hardmod/stats_update");
}

ShowStatsSavingMessage(message_n)
{
	decl String:text[192];
	switch (message_n)
	{
		case 1: text = "Saving points...";
		case 2: text = "Saving points completed";
	}
	
	new String:HostName[48];
	new Handle:gamecvar_hostname = FindConVar("hostname");
	GetConVarString(gamecvar_hostname, HostName, sizeof(HostName));

	PrintToChatAll("\x04%s\x01 : %s", HostName, text);
}

public CoopStatsOnClientPutInServer(client)
{
}

CStatsOnMapStart()
{
	Log("coop_stats.inc / CStatsOnMapStart() / 1", 2);
	BuildStatsPath();
	FirstRoundTimerCountdown = -1;
	FirstRoundTimerEnabled = false;

	new real_clients_count = GetRealClientCount(true);
	if (rounds == 1 && IsFirstMapFlag) // && IsServerProcessing())
	{
		if (real_clients_count > 0)
		{
			Log("coop_stats.inc / CStatsOnMapStart() / StartFirstRoundTimer()", 2);
			StartFirstRoundTimer(130);
		}
	}
	PrintToServer("CStatsOnMapStart(): rounds = %d, IsFirstMap = %b, IsSP = %b, clients = %d", rounds, IsFirstMapFlag, IsServerProcessing(), real_clients_count);
	save_points_flag = false;
	if (GetConVarInt(hm_stats_checkonmapstart) > 0)
	{
		Command_StatsCheckLimit(0, 0);
	}
}

public Action:Command_ReadMapMultiplier(client, args)
{
	if (args > 0)
	{
		decl String:value_map[36];
		GetCmdArg(1, value_map, sizeof(value_map));
		if (client > 0)
		{
			
			ReplyToCommand(client, "\x05Map Multiplier (\x04%s\x05) = \x04%f", value_map, ReadMapMultiplier(value_map));
		}
		else
		{
			ReplyToCommand(client, "Map Multiplier (%s) = %f", value_map, ReadMapMultiplier(value_map));
		}
	}
	else
	{
		if (client > 0)
		{
			
			ReplyToCommand(client, "\x05Map Multiplier (\x04%s\x05) = \x04%f", current_map, ReadMapMultiplier(current_map));
		}
		else
		{
			ReplyToCommand(client, "Map Multiplier (%s) = %f", current_map, ReadMapMultiplier(current_map));
		}
	}
	return Plugin_Continue;
}

public Action:Command_SetupMapMultiplier(client, args)
{
	if (args < 1)
	{
		return Plugin_Handled;
	}
	decl String:arg_multiplier[8];
	GetCmdArg(1, arg_multiplier, sizeof(arg_multiplier));
	if (args > 1)
	{
		decl String:arg_map[36];
		GetCmdArg(2, arg_map, sizeof(arg_map));
		SetupMapMultiplier(arg_map, StringToFloat(arg_multiplier));
	}
	else
	{
		SetupMapMultiplier(current_map, StringToFloat(arg_multiplier));
	}
	
	return Plugin_Continue;
}

public Action:Command_ResetMapMultipliers(client, args)
{
	if (args < 1)
	{
		return Plugin_Handled;
	}

	decl String:arg[8];
	GetCmdArg(1, arg, sizeof(arg));
	new Float:MapMultiplier = StringToFloat(arg);
	SetupMapMultiplier("c1m1_hotel", MapMultiplier);
	SetupMapMultiplier("c1m2_streets", MapMultiplier);
	SetupMapMultiplier("c1m3_mall", MapMultiplier);
	SetupMapMultiplier("c1m4_atrium", MapMultiplier);
	SetupMapMultiplier("c2m1_highway", MapMultiplier);
	SetupMapMultiplier("c2m2_fairgrounds", MapMultiplier);
	SetupMapMultiplier("c2m3_coaster", MapMultiplier);
	SetupMapMultiplier("c2m4_barns", MapMultiplier);
	SetupMapMultiplier("c2m5_concert", MapMultiplier);
	SetupMapMultiplier("c3m1_plankcountry", MapMultiplier);
	SetupMapMultiplier("c3m2_swamp", MapMultiplier);
	SetupMapMultiplier("c3m3_shantytown", MapMultiplier);
	SetupMapMultiplier("c3m4_plantation", MapMultiplier);
	SetupMapMultiplier("c4m1_milltown_a", MapMultiplier);
	SetupMapMultiplier("c4m2_sugarmill_a", MapMultiplier);
	SetupMapMultiplier("c4m3_sugarmill_b", MapMultiplier);
	SetupMapMultiplier("c4m4_milltown_b", MapMultiplier);
	SetupMapMultiplier("c4m5_milltown_escape", MapMultiplier);
	SetupMapMultiplier("c5m1_waterfront", MapMultiplier);
	SetupMapMultiplier("c5m2_park", MapMultiplier);
	SetupMapMultiplier("c5m3_cemetery", MapMultiplier);
	SetupMapMultiplier("c5m4_quarter", MapMultiplier);
	SetupMapMultiplier("c5m5_bridge", MapMultiplier);
	SetupMapMultiplier("c6m1_riverbank", MapMultiplier);
	SetupMapMultiplier("c6m2_bedlam", MapMultiplier);
	SetupMapMultiplier("c6m3_port", MapMultiplier);
	SetupMapMultiplier("c7m1_docks", MapMultiplier);
	SetupMapMultiplier("c7m2_barge", MapMultiplier);
	SetupMapMultiplier("c7m3_port", MapMultiplier);
	SetupMapMultiplier("c8m1_apartment", MapMultiplier);
	SetupMapMultiplier("c8m2_subway", MapMultiplier);
	SetupMapMultiplier("c8m3_sewers", MapMultiplier);
	SetupMapMultiplier("c8m4_interior", MapMultiplier);
	SetupMapMultiplier("c8m5_rooftop", MapMultiplier);
	SetupMapMultiplier("c9m1_alleys", MapMultiplier);
	SetupMapMultiplier("c9m2_lots", MapMultiplier);
	SetupMapMultiplier("c10m1_caves", MapMultiplier);
	SetupMapMultiplier("c10m2_drainage", MapMultiplier);
	SetupMapMultiplier("c10m3_ranchhouse", MapMultiplier);
	SetupMapMultiplier("c10m4_mainstreet", MapMultiplier);
	SetupMapMultiplier("c10m5_houseboat", MapMultiplier);
	SetupMapMultiplier("c11m1_greenhouse", MapMultiplier);
	SetupMapMultiplier("c11m2_offices", MapMultiplier);
	SetupMapMultiplier("c11m3_garage", MapMultiplier);
	SetupMapMultiplier("c11m4_terminal", MapMultiplier);
	SetupMapMultiplier("c11m5_runway", MapMultiplier);
	SetupMapMultiplier("c12m1_hilltop", MapMultiplier);
	SetupMapMultiplier("c12m2_traintunnel", MapMultiplier);
	SetupMapMultiplier("c12m3_bridge", MapMultiplier);
	SetupMapMultiplier("c12m4_barn", MapMultiplier);
	SetupMapMultiplier("c12m5_cornfield", MapMultiplier);
	SetupMapMultiplier("c13m1_alpinecreek", MapMultiplier);
	SetupMapMultiplier("c13m2_southpinestream", MapMultiplier);
	SetupMapMultiplier("c13m3_memorialbridge", MapMultiplier);
	SetupMapMultiplier("c13m4_cutthroatcreek", MapMultiplier);
	return Plugin_Continue;
}

public Action:Command_RandomPoints(client, args)
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		ChachedPoints[i] = GetRandomInt(-100, 100);
	}
}

public Action:Command_SavePoints(client, args)
{
	ShowStatsSavingMessage(1);
	Alive_Players = AlivePlayers();
	save_points_flag = true;
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsRealClient(i))
		{
			SaveCachedPoints(i);
		}
		ChachedPoints[i] = 0;
	}
	save_points_flag = false;
	ShowStatsSavingMessage(2);
}

public Action:Command_ClearAllPoints(client, args)
{
	ClearAllPoints(client);
}

public Action:Command_AddPoints(client, args)
{
	Log("coop_stats.inc / Command_AddPoints() / 1", 4);
	if (Top15Used)
	{
		Log("coop_stats.inc / Command_AddPoints() / Top15Used", 3);
		if (client == 0)
		{
			PrintToServer("Error: Stats is busy");
		}
		else
		{
			ReplyToCommand(client, "Error: Stats is busy");
		}
	}
	
	if (args < 2)
	{
		if (client == 0)
		{
			PrintToServer("Usage: sm_addpoints <steamid> <points>");
		}
		else
		{
			ReplyToCommand(client, "Usage: sm_addpoints <steamid> <points>");
		}
		return Plugin_Handled;
	}
	new String:clientauth[32];
	new String:points_str[32];
	new String:clientname[MAX_NAME_LENGTH];
	new points_add;
	GetCmdArg(1, clientauth, sizeof(clientauth));
	GetCmdArg(2, points_str, sizeof(points_str));
	points_add = StringToInt(points_str);
	
	Top15Used = true;

	new Handle:Data = CreateKeyValues("points_data");
	new count, points; //
	FileToKeyValues(Data, datafilepath);

	//search data or create it
	KvJumpToKey(Data, "data", true);

	//user in list?
	if (!KvJumpToKey(Data, clientauth))
	{
		//add to count 1 or create the count
		KvGoBack(Data);
//		KvJumpToKey(Data, "points_data", true);
//		KvGotoFirstSubKey(Data);
		KvJumpToKey(Data, "info", true);
			
		count = KvGetNum(Data, "count", 0);
		count++;
			
		KvSetNum(Data, "count", count);
		KvGoBack(Data);
			
		//add auth key
//		KvGotoFirstSubKey(Data);
//		KvJumpToKey(Data, "points_data");
		KvJumpToKey(Data, "data", true);
		KvJumpToKey(Data, clientauth, true);
	}

		//get point number
	new current_points = KvGetNum(Data, "points", 0);
	points = current_points + points_add;

		//set points and name
	KvSetNum(Data, "points", points);
	KvGetString(Data, "name", clientname, MAX_NAME_LENGTH, "---");
	if (!StrEqual(clientname, "---", false))
	{
		KvSetString(Data, "name", "no-name-specified");
	}

	//save it
	KvRewind(Data);
	KeyValuesToFile(Data, datafilepath);

	CloseHandle(Data);
	Top15Used = false;
	return Plugin_Continue;
}

public Action:Command_RemoveDeadStats(client, args)
{
	if (Top15Used)
	{
		Log("coop_stats.inc / Command_RemoveDeadStats() / Top15Used", 2);
		PrintToServer("Top15Used: YES");
	}
	else  PrintToServer("Top15Used: NO");
	if (players_data_used)
	{
		Log("coop_stats.inc / Command_RemoveDeadStats() / players_data_used", 2);
		PrintToServer("players_data_used: YES");
	}
	else PrintToServer("players_data_used: NO");
	if (Top15Used || players_data_used)
	{
		if (client == 0)
		{
			PrintToServer("Error: Stats is busy");
		}
		else
		{
			ReplyToCommand(client, "Error: Stats is busy");
		}
		return Plugin_Handled;
	}
	new start_time = GetTime();
	new deadline;
	if (args < 1)
	{
		deadline = GetTime() - 86400 * REMOVEDEADSTATS_DAYS;
	}
	else
	{
		new String:string_days[8];
		GetCmdArg(1, string_days, sizeof(string_days));
		new days = 86400 * StringToInt(string_days);
		deadline = GetTime() - days;
	}
	
	Top15Used = true;

	new Handle:Data = CreateKeyValues("points_data");
	if (!FileToKeyValues(Data, datafilepath))
	{
		Log("coop_stats.inc / Command_RemoveDeadStats() / No data found (points_data)", 2);
		ReplyToCommand(client, "[SM] No data found.");
		return Plugin_Continue;
	}	

//	KvGotoFirstSubKey(Data);
	KvJumpToKey(Data, "info");
	
	new count = KvGetNum(Data, "count", 0);
	PrintToServer("[Stats] Count: %d", count);
	new removed_items = 0;
	new keep_items = 0;
	
	KvGoBack(Data);
//	KvGotoFirstSubKey(Data);
	KvJumpToKey(Data, "data");
	KvGotoFirstSubKey(Data);
	
	new String:clientauth[MAX_STEAM_LENGTH];
	
	players_data_used = true;
	new Handle:PData = CreateKeyValues("players_data");
	FileToKeyValues(PData, PlayersFilePath);
	if (!FileToKeyValues(PData, PlayersFilePath))
	{
		Log("coop_stats.inc / Command_RemoveDeadStats() / !FileToKeyValues(PData, PlayersFilePath)", 2);
		if (client == 0)
		{
			PrintToServer("Error: Can't open players database");
		}
		else
		{
			ReplyToCommand(client, "\x05Error: Can't open players database");
		}
		CloseHandle(Data);
		Top15Used = false;
		CloseHandle(PData);
		players_data_used = false;
		return Plugin_Handled;
	}
//	if (!KvGotoFirstSubKey(PData, true))
//	{
//		PrintToChatAll("ERROR?: !KvGotoFirstSubKey(PData)");
//	}
	new client_time = 0;
	if (client > 0 || show_stats_messages)
	{
		PrintToChatAll("\x05[Stats] Removing dead players...");
	}
	KvJumpToKey(Data, "data");
	KvJumpToKey(PData, "players_data");
	
//	new String:sectionname[128];
	
	for (new i = 0; i < count; i++)
	{	
		if (KvGetSectionName(Data, clientauth, sizeof(clientauth)))
		{
//			PrintToChatAll("\x05 clientauth = %s", clientauth);
//			KvGetSectionName(PData, sectionname, sizeof(sectionname));
//			PrintToChatAll("sectionname = %s", sectionname);
			KvJumpToKey(PData, "players_data");
//			KvGotoFirstSubKey(PData, true);
			if (KvJumpToKey(PData, clientauth))
			{
				client_time = KvGetNum(PData, FILEMSG_LASTEXIT, 0);
				KvGoBack(PData);
			}
			else
			{
//				PrintToChatAll("\x05No client found or DB read problems");
				client_time = 0;
			}
			if (client_time <= deadline)
			{
//				PrintToServer("[Stats %d of %d] Removing %s (%d <= %d)", i + 1, count, clientauth, client_time, deadline);
//				PrintToChatAll("[Stats %d of %d] Removing %s (%d <= %d)", i + 1, count, clientauth, client_time, deadline);
				KvDeleteThis(Data);
				removed_items++;
			}
			else
			{
				keep_items++;
//				PrintToServer("[Stats %d of %d] Keep %s (DeadLine OK)", i + 1, count, clientauth);
				KvGotoNextKey(Data);
			}
		}
	}
	if (client > 0 || show_stats_messages)
	{
		PrintToChatAll("\x05[Stats] Removing dead complite.");
	}
	CloseHandle(PData);
	players_data_used = false;
	
	Total_Players = count - removed_items;
	
	PrintToServer("[Stats] Removed items: %d", removed_items);
	PrintToServer("[Stats] Keep items: %d", keep_items);
	if (client > 0 || show_stats_messages)
	{
		PrintToChatAll("\x05[Stats] Removed players: %d", removed_items);
		PrintToChatAll("\x05[Stats] Keep players: %d", keep_items);
	}
	KvRewind(Data);
	KeyValuesToFile(Data, datafilepath);

	KvGotoFirstSubKey(Data);
	KvJumpToKey(Data, "info");
	KvSetNum(Data, "count", Total_Players);
	KvRewind(Data);
	KeyValuesToFile(Data, datafilepath);
	CloseHandle(Data);
	Top15Used = false;
	
	PrintToServer("[Stats] Work took %d second(s)", GetTime() - start_time);
	if (client > 0)
	{
		PrintToChatAll("\x05[Stats] Work took %d second(s)", GetTime() - start_time);
	}
	return Plugin_Continue;
}

public Action:Command_StatsCheckLimit(client, args)
{
	Log("coop_stats.inc / Command_StatsCheckLimit() / 1", 3);
	if (Total_Players == 0 && !Top15Used)
	{
		Top15Used = true;

		new Handle:Data = CreateKeyValues("points_data");
		if (!FileToKeyValues(Data, datafilepath))
		{
			ReplyToCommand(client, "[SM] No data found.");
			return Plugin_Handled;
		}	

		KvGotoFirstSubKey(Data);
		KvJumpToKey(Data, "info", true);
		
		Total_Players = KvGetNum(Data, "count", 0);
		CloseHandle(Data);
		Top15Used = false;
	}
	else
	{
		Log("coop_stats.inc / Command_StatsCheckLimit() / Top15Used", 2);
		return Plugin_Handled;
	}
	if (GetConVarInt(hm_stats_maxplayers) < Total_Players)
	{
		Command_RemoveDeadStats(client, args);
	}
	return Plugin_Continue;
}

public Action:Command_MapTop(client, args)
{
	if (!IsPlayerCPUAllowed(client)) return;
	PrintMapTop(client);
}

public Action:Command_Top15(client, args)
{
	if (!IsPlayerCPUAllowed(client)) return;
	ShowTop15(client);
}

public Action:Command_Top30(client, args)
{
	if (!IsPlayerCPUAllowed(client)) return;
	if (Top15Used)
	{
		Log("coop_stats.inc / Command_Top30() / Top15Used", 2);
		return;
	}
	Log("coop_stats.inc / Command_Top30() / 1", 3);
	Top15Used = true;
	new Handle:Data = CreateKeyValues("points_data"); 
	new count;
	
	if (!FileToKeyValues(Data, datafilepath))
	{
		Log("coop_stats.inc / Command_Top30() / No data found.", 2);
		PrintToChat(client, "[SM] No data found.");
		Top15Used = false;
		return;
	}
	
//	KvGotoFirstSubKey(Data);
	KvJumpToKey(Data, "info", true);
	
	//get count
	count = KvGetNum(Data, "count", 0);
	if (count < 1)
	{
		Log("coop_stats.inc / Command_Top30() / count < 1", 2);
		Top15Used = false;
		return;	
	}
	
	Log("coop_stats.inc / Command_Top30() / 1", 3);
	
	new String:SteamIDs[count][MAX_STEAM_LENGTH];
	new String:names[count][MAX_NAME_LENGTH];
	new String:countries[count][46];
	new points[count][2];
	new totalpoints = 0;
	
	KvGoBack(Data);
	KvJumpToKey(Data, "data", true);
	KvGotoFirstSubKey(Data);
	
	Log("coop_stats.inc / Command_Top30() / 2", 3);
	
	//save name and points in 2 different arrays (association via i)
	for (new i = 0; i < count; i++)
	{
		KvGetString(Data, "name", names[i], MAX_NAME_LENGTH, "---");
		KvGetString(Data, "country", countries[i], 46, "");
		KvGetSectionName(Data, SteamIDs[i], MAX_STEAM_LENGTH);
		points[i][0] = i;
		points[i][1] = KvGetNum(Data, "points", 0);
		totalpoints += points[i][1];
		KvGotoNextKey(Data);
	}
	//sort points
	SortCustom2D(points, count, Sort_Function);
	PrintToConsole(client, "Top 30:");
	
	new String:text[64];
	
	Log("coop_stats.inc / Command_Top30() / 3", 3);
	
	//add 5 best to panel
	if (count > 30)
	{
		count = 30;
	}
	for (new i = 0; i < count; i++)
	{
		if (draw_points)
		{
			Format(text, sizeof(text), "%d (%d): %s ", i + 1, points[i][1], names[points[i][0]]); // , points[i][1]
		}
		else
		{
			Format(text, sizeof(text), "%d: %s ", i + 1, names[points[i][0]]); // , points[i][1]
		}
		if (client)
		{
			PrintToConsole(client, "[%s] %s", SteamIDs[points[i][0]], text);
		}
		else
		{
			PrintToServer("\"%s\"     \"25\"", SteamIDs[points[i][0]]);
		}
	}
	CloseHandle(Data);
	Top15Used = false;
	Log("coop_stats.inc / Command_Top30() / 4", 3);
}

public Action:Command_TopAll(client, args)
{
	if (!IsPlayerCPUAllowed(client)) return;
	if (Top15Used)
	{
		Log("coop_stats.inc / Command_TopAll() / Top15Used", 2);
		return;
	}
	if (client) return;
	new String:CS_FileName[MAX_FILE_NAME_LENGTH];
	new Handle:file;
	new bool:WriteToFile = false;
	if (args > 0)
	{
		decl String:arg[256];
		GetCmdArg(1, arg, sizeof(arg));
		BuildPath(Path_SM, CS_FileName, sizeof(CS_FileName), "gamedata/%s.stats", arg);
		file = OpenFile(CS_FileName, "a+");
		if (file != INVALID_HANDLE) WriteToFile = true;
		FileSeek(file, 0, SEEK_SET);
	}
	Log("coop_stats.inc / Command_TopAll() / 1", 3);
	Top15Used = true;
	new Handle:Data = CreateKeyValues("points_data"); 
	new count;
	
	if (!FileToKeyValues(Data, datafilepath))
	{
		Log("coop_stats.inc / Command_TopAll() / No data found.", 2);
		PrintToChat(client, "[SM] No data found.");
		Top15Used = false;
		return;
	}
	
//	KvGotoFirstSubKey(Data);
	KvJumpToKey(Data, "info", true);
	
	//get count
	count = KvGetNum(Data, "count", 0);
	if (count < 1)
	{
		Log("coop_stats.inc / Command_TopAll() / count < 1", 2);
		Top15Used = false;
		return;	
	}
	
	Log("coop_stats.inc / Command_TopAll() / 1", 3);
	
	new String:SteamIDs[count][MAX_STEAM_LENGTH];
	new String:names[count][MAX_NAME_LENGTH];
	new String:countries[count][46];
	new points[count][2];
	new totalpoints = 0;
	
	KvGoBack(Data);
	KvJumpToKey(Data, "data", true);
	KvGotoFirstSubKey(Data);
	
	Log("coop_stats.inc / Command_TopAll() / 2", 3);
	
	//save name and points in 2 different arrays (association via i)
	for (new i = 0; i < count; i++)
	{
		KvGetString(Data, "name", names[i], MAX_NAME_LENGTH, "---");
		KvGetString(Data, "country", countries[i], 46, "");
		KvGetSectionName(Data, SteamIDs[i], MAX_STEAM_LENGTH);
		points[i][0] = i;
		points[i][1] = KvGetNum(Data, "points", 0);
		totalpoints += points[i][1];
		KvGotoNextKey(Data);
	}
	//sort points
	SortCustom2D(points, count, Sort_Function);
	PrintToConsole(client, "Top All:");
	
	new String:text[64];
	
	Log("coop_stats.inc / Command_TopAll() / 3", 3);
	new String:file_text[128];
	for (new i = 0; i < count; i++)
	{
		if (draw_points)
		{
			Format(text, sizeof(text), "%d (%d): %s ", i + 1, points[i][1], names[points[i][0]]); // , points[i][1]
		}
		else
		{
			Format(text, sizeof(text), "%d: %s ", i + 1, names[points[i][0]]); // , points[i][1]
		}
		if (WriteToFile)
		{
			Format(file_text, sizeof(file_text), "[%s] %s ", SteamIDs[points[i][0]], text);
			if (!WriteFileLine (file, "%s", file_text))
			{
				WriteToFile = false;
				CloseHandle(file);
			}
		}
		PrintToServer("[%s] %s", SteamIDs[points[i][0]], text);
	}
	CloseHandle(Data);
	Top15Used = false;
	if (WriteToFile) CloseHandle(file);
	Log("coop_stats.inc / Command_TopAll() / 4", 3);
}

PrintPoints(client)
{
	PrintToChat(client, "\x05Your total points: \x04%d \x05, Your map points: \x04%d", GetPoints(client), ChachedPoints[client]);
}

ShowTop15(client)
{
	if (!Top15Used)
	{
		PrintTopList(client);
	}
	else
	{
		CreateTimer(1.0, TimedTop15, client);
	}
}

public Action:TimedTop15(Handle:timer, any:client)
{
	ShowTop15(client);
}

public Action:TimedAddPoints(Handle:timer, any:client, any:add_points)
{
	if (!Top15Used)
	{
		AddPoints(client, add_points);
	}
	else
	{
		PrintToChatAll("\x05WARNING, CRASH PROTECTION, %N \x05LOOSE HIS MAP POINTS", client);
	}
}

public Action:TimedRank(Handle:timer, any:client)
{
	if (!IsClientConnected(client) || IsFakeClient(client))
	{
		return;
	}
	if (IsClientInGame(client))
	{
		PrintRank(client);
	}
}

public Action:TimedFirstRoundPoints(Handle:timer, any:client)
{
	Log("coop_stats.inc / Action:TimedFirstRoundPoints() / 1", 3);
	if (rounds != 1 || !IsServerProcessing() || FirstRoundTimerCountdown < 0)
	{
		FirstRoundTimerEnabled = false;
		return;
	}
	Log("coop_stats.inc / Action:TimedFirstRoundPoints() / 2", 3);
	FirstRoundTimerCountdown--;
	if (FirstRoundTimerCountdown > 0)
	{
		Log("coop_stats.inc / Action:TimedFirstRoundPoints() / A", 3);
		CreateTimer(1.0, TimedFirstRoundPoints, _);
		FirstRoundTimerEnabled = true;
//		PrintToServer("First-try bonus countdown: %d", FirstRoundTimerCountdown);
//		if (FirstRoundTimerCountdown == 60 || FirstRoundTimerCountdown == 30 || FirstRoundTimerCountdown < 6)
//		{
//			PrintHintTextToAll("First-try bonus countdown: %d", FirstRoundTimerCountdown);
//			PrintToServer("First-try bonus countdown: %d", FirstRoundTimerCountdown);
//		}
	}
	else
	{
		Log("coop_stats.inc / Action:TimedFirstRoundPoints() / First-try bonus", 2);
		for (new i = 1; i <= L4D_MAXPLAYERS; i++)
		{
			if (IsClientConnected(i) && IsClientInGame(i) && !IsFakeClient(i) && GetClientTeam(i) == TEAM_SURVIVORS)
			{
				AddCachedPoints(i, FirstRoundPoints);
				PrintHintText(i, "First-try bonus: %d points!", FirstRoundPoints);
				PrintToConsole(i, "First-try bonus: %d points!", FirstRoundPoints);
				PrintToServer("First-try bonus: %d points!", FirstRoundPoints);
			}
		}
	}
}

public Action:TimedClearCachedPoints(Handle:timer)
{
	ClearCachedPoints();
}

public Action:TimedGrantPlayerColor(Handle:timer, any:client)
{
	if (!IsClientInGame(client))
	{
		return;
	}
	if (Top15Used || GetClientHealth(client) < 1)
	{
		CreateTimer(1.0, TimedGrantPlayerColor, client);
	}
	else
	{
		GrantPlayerColor(client);		
	}
}

public Action:Command_Rank(client, args)
{
	if (!IsPlayerCPUAllowed(client)) return;
	PrintRank(client);
}

public Action:Command_Points(client, args)
{
	PrintPoints(client);
}

PrintTopList(client)
{
	if (Top15Used)
	{
		Log("coop_stats.inc / PrintTopList() / Top15Used", 2);
		return;
	}
	Log("coop_stats.inc / PrintTopList() / 1", 3);
	Top15Used = true;
	new Handle:Data = CreateKeyValues("points_data"); 
	new count;
	
	if (!FileToKeyValues(Data, datafilepath))
	{
		Log("coop_stats.inc / PrintTopList() / No data found.", 2);
		PrintToChat(client, "[SM] No data found.");
		Top15Used = false;
		return;
	}
	
//	KvGotoFirstSubKey(Data);
	KvJumpToKey(Data, "info", true);
	
	//get count
	count = KvGetNum(Data, "count", 0);
	if (count < 1)
	{
		Log("coop_stats.inc / PrintTopList() / count < 1", 2);
		Top15Used = false;
		return;	
	}
	
	Log("coop_stats.inc / PrintTopList() / 1", 3);
	
	new String:names[count][MAX_NAME_LENGTH];
	new String:countries[count][46];
	new points[count][2];
	new totalpoints = 0;
	
	KvGoBack(Data);
//	KvGotoFirstSubKey(Data);
	KvJumpToKey(Data, "data", true);
	KvGotoFirstSubKey(Data);
	
	Log("coop_stats.inc / PrintTopList() / 2", 3);
	
	//save name and points in 2 different arrays (association via i)
	for (new i = 0; i < count; i++)
	{
		KvGetString(Data, "name", names[i], MAX_NAME_LENGTH, "---");
		KvGetString(Data, "country", countries[i], 46, "");
		points[i][0] = i;
		points[i][1] = KvGetNum(Data, "points", 0);
//		PrintToChatAll("\x05points[%d][0] = %d", i, i);
//		PrintToChatAll("\x04points[%d][1] = %d", i, points[i][1]);
//		PrintToChatAll("\x04names[%d] = %s", i, names[i]);
		totalpoints += points[i][1];
		KvGotoNextKey(Data);
	}
	//sort points
	SortCustom2D(points, count, Sort_Function);
	
	//create panel
	new Handle:TopPointPanel = CreatePanel();
	SetPanelTitle(TopPointPanel, "Top 15:");
	DrawPanelText(TopPointPanel, " ");
	
	new String:text[64];
	
	Log("coop_stats.inc / PrintTopList() / 3", 3);
	
	//add 5 best to panel
	if (count > 15)
	{
		count = 15;
	}

//	new AdminId:ClientAdminId = GetUserAdmin(client);
//	new flags = GetAdminFlags(ClientAdminId, Access_Effective);

	for (new i = 0; i < count; i++)
	{
//		if (!StrEqual(countries[points[i][0]], "", false))
//		{
//			if (flags & ADMFLAG_ROOT || flags & ADMFLAG_BAN)
//			{
//				Format(text, sizeof(text), "%d (%d): %s (%s)", i + 1, points[i][1], names[points[i][0]], countries[points[i][0]]);
//			}
//			else
//			{
//				Format(text, sizeof(text), "%d: %s (%s)", i + 1, names[points[i][0]], countries[points[i][0]]);
//			}
//		}
//		else
//		{
		if (draw_points)
		{
			Format(text, sizeof(text), "%d (%d): %s ", i + 1, points[i][1], names[points[i][0]]); // , points[i][1]
		}
		else
		{
			Format(text, sizeof(text), "%d: %s ", i + 1, names[points[i][0]]); // , points[i][1]
		}
//		}
		DrawPanelText(TopPointPanel, text);
	}
	
	//add total points
//	DrawPanelText(TopPointPanel, " ");
//	Format(text, sizeof(text), "There have been a total of %d points on this server!", totalpoints);
//	DrawPanelText(TopPointPanel, text);
	
	//send panel
	SendPanelToClient(TopPointPanel, client, TopPointPanelHandler, 8);
	
	CloseHandle(TopPointPanel);
	CloseHandle(Data);
	Top15Used = false;
	Log("coop_stats.inc / PrintTopList() / 4", 3);
}

ClearAllPoints(client)
{
	if (Top15Used)
	{
		Log("coop_stats.inc / ClearAllPoints() / Top15Used", 2);
		return;
	}
	Log("coop_stats.inc / ClearAllPoints() / 1", 1);
	Top15Used = true;
	new Handle:Data = CreateKeyValues("points_data"); 
	new count;
	
	if (!FileToKeyValues(Data, datafilepath))
	{
		Log("coop_stats.inc / ClearAllPoints() / No data found.", 1);
		ReplyToCommand(client, "[SM] No data found.");
		return;
	}
	
	KvGotoFirstSubKey(Data);
	KvJumpToKey(Data, "info", true);
	
	count = KvGetNum(Data, "count", 0);
	
	KvGoBack(Data);
//	KvGotoFirstSubKey(Data);
	KvJumpToKey(Data, "data");
	KvGotoFirstSubKey(Data);
	
	new String:clientname[MAX_NAME_LENGTH];
	
	for (new i = 0; i < count; i++)
	{	
		KvGetString(Data, "name", clientname, MAX_NAME_LENGTH, "---");
		ReplyToCommand(client, "[HARDMOD] Clear Points: %s", clientname);
		KvSetNum(Data, "points", 0);
//		KvRewind(Data);
		KvGotoNextKey(Data);
	}
	KvRewind(Data);
	KeyValuesToFile(Data, datafilepath);
	CloseHandle(Data);
	Top15Used = false;
}

PrintRank(client)
{
	if (!IsRealClient(client) || Top15Used)
	{
		return;
	}
	
	new clientpoints = GetPoints(client);
	ClientPoints[client] = clientpoints;
	Top15Used = true;
	new Handle:Data = CreateKeyValues("points_data"); 
	
	if (!FileToKeyValues(Data, datafilepath))
	{
		PrintToChat(client, "[SM] No data found.");
		CloseHandle(Data);
		Top15Used = false;		
		return;
	}

	if (!KvJumpToKey(Data, "info"))
	{
		PrintToChat(client, "[SM] No data (info) found or data corrupted.");
		CloseHandle(Data);
		Top15Used = false;		
		return;
	}

	new count = KvGetNum(Data, "count", -1);

	if (count < 1)
	{
		CloseHandle(Data);
		Top15Used = false;		
		return;
	}
	
	Total_Players = count;
	new String:names[count][MAX_NAME_LENGTH];
	new String:authids[count][MAX_STEAM_LENGTH];
	new String:countries[count][46];
	new points[count][2];
	
	KvGoBack(Data);
//	KvGotoFirstSubKey(Data);
	KvJumpToKey(Data, "data");
	KvGotoFirstSubKey(Data);
	
	//save name and points in 2 different arrays (association via i)
	for (new i = 0; i < count; i++)
	{
		KvGetSectionName(Data, authids[i], MAX_STEAM_LENGTH);
		KvGetString(Data, "name", names[i], MAX_NAME_LENGTH, "---");
		KvGetString(Data, "country", countries[i], 46, "");
		points[i][0] = i;
		points[i][1] = KvGetNum(Data, "points", 0);
		KvGotoNextKey(Data);
	}
	
	//sort points
	SortCustom2D(points, count, Sort_Function);

	new j = -1;
	
	new String:clientname[MAX_NAME_LENGTH];
	GetClientName(client, clientname, sizeof(clientname));
	
//	PrintToChat(client, "\x05line \x03%d", 376);

	new String:clientauth[MAX_STEAM_LENGTH];
	GetClientAuthString(client, clientauth, sizeof(clientauth));
	
//	PrintToChatAll("rank :: count = %d", count);

	for (new i = 0; i < count; i++)
	{
//		PrintToChat(client, "\x05line \x03%d\x05, i = \x03%d\x05, count = \x03%d", 380, i, count);
		if (points[i][1] == clientpoints)
		{
//			PrintToChat(client, "\x04points[\x03%d\x04][1] (\x03%d\x04) == clientpoints (\x03%d\x04)", i, points[i][1], clientpoints);
			if (StrEqual(authids[points[i][0]], clientauth, true))
			{
//				PrintToChat(client, "\x05if (StrEqual(names[points[i][0]], clientname, true)) :: \x03 FOUND, i = %d", i);
				j = i;
			}
//			else
//			{
//				PrintToChat(client, "\x04%s \x05!=\x04 %s", names[i], clientname);
//			}
		}
//		else
//		{
//			PrintToChat(client, "\x05points[\x03%d\x05][1] (\x03%d\x05) != clientpoints (\x03%d\x05)", i, points[i][1], clientpoints);
//		}
	}
//	PrintToChat(client, "\x05line \x03%d", 394);
	if (j < 0)
	{
		PrintToChat(client, "\x05You have no rank yet");
		ClientRank[client] = 0;
		CloseHandle(Data);
		Top15Used = false;
		return;
	}
	ClientRank[client] = j + 1;
	CreateTimer(1.0, TimedGrantPlayerColor, client);
	GrantPlayerLevel(client, ClientRank[client]);
	
	PrintToChat(client, "\x05Players in stats: \x03%d\x05, Your rank: \x03%d", count, ClientRank[client]);
	
	CloseHandle(Data);
	Top15Used = false;
}

PrintMapTop(client)
{
	new points[32][2];
	new String:names[32][MAX_NAME_LENGTH];
	decl String:NameBuffer[MAX_NAME_LENGTH];
	decl String:WorseNameBuffer[MAX_NAME_LENGTH];
	new count, totalpoints, j, worse;
	NameBuffer = "";
	WorseNameBuffer = "";
	j = -100000;
	worse = 100000;
	count = 0;
	totalpoints = 0;
	new topplayerrank = 0;
	new worseplayerrank = 0;
	//save name and points in 2 different arrays (association via i)
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsRealClient(i))
		{
			new String:clientname[MAX_NAME_LENGTH];
			GetClientName(i, clientname, sizeof(clientname));
			names[i] = clientname;
			points[i][0] = i;
			points[i][1] = ChachedPoints[i];
			if (ChachedPoints[i] > j)
			{
				j = ChachedPoints[i];
				NameBuffer = clientname;
				topplayerrank = ClientRank[i];
			}
			if (ChachedPoints[i] < worse)
			{
				worse = ChachedPoints[i];
				WorseNameBuffer = clientname;
				worseplayerrank = ClientRank[i];
			}
			count++;
			totalpoints += ChachedPoints[i];
		}
	}
	if (GetConVarInt(hm_usemapmultipliers) > 0)
	{
		decl String:MapMultiplier[4];
		FloatToString(ReadMapMultiplier(current_map), MapMultiplier, sizeof(MapMultiplier));
		PrintToChat(client, "\x05Map points multiplier: \x04%s\x05, Alive Players: \x04%d", MapMultiplier, AlivePlayers());
	}
	CPrintToChat(client, "\x05Map total points: \x04%d (%d){blue}", totalpoints, CalculatePoints(totalpoints));
	CPrintToChat(client, "\x05Map best player: {blue}%s\x05 (rank: \x04%d\x05; points: \x04%d\x05, \x04%d\x05)", NameBuffer, topplayerrank, j, CalculatePoints(j));
	if (worse != 0)
	{
		CPrintToChat(client, "\x05Map worst player: {blue}%s\x05 (rank: \x04%d\x05; points: \x04%d\x05, \x04%d\x05)", WorseNameBuffer, worseplayerrank, worse, CalculatePoints(worse));
	}
	if (GetConVarInt(hm_countaliveplayers) > 0)
	{
		CPrintToChat(client, "\x05Your map points: \x04%d\x05 (\x04%d\x05){blue}", ChachedPoints[client], CalculatePoints(ChachedPoints[client]));
	}
	else
	{
		CPrintToChat(client, "\x05Your map points: \x04%d{blue}", ChachedPoints[client]);
	}
}

public Sort_Function(array1[], array2[], const completearray[][], Handle:hndl)
{
	//sort function for our crown array
	if (array1[1] > array2[1]) return -1;
	if (array1[1] == array2[1]) return 0;
	return 1;
}

public BackSort_Function(array1[], array2[], const completearray[][], Handle:hndl)
{
	//sort function for our crown array
	if (array1[1] > array2[1]) return 1;
	if (array1[1] == array2[1]) return 0;
	return 1;
}

public TopPointPanelHandler(Handle:menu, MenuAction:action, param1, param2)
{
	//nothing to do
}

public Action:StatsEvent_WitchKilled(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	if (!IsValidClient(client))
	{
		return Plugin_Continue;
	}
	new WitchPenalty = 0;
	if (GetClientTeam(client) == TEAM_SURVIVORS)
	{
		WitchPenalty = 1;
	}
	else if (GetClientTeam(client) == TEAM_INFECTED)
	{
		WitchPenalty = -1;
	}
	if (IsFakeClient(client))
	{
		return Plugin_Continue;
	}
	if (GetEventBool(event, "oneshot"))
	{
		AddCachedPoints(client, Witch_Points_Crown_Cost * WitchPenalty);
	}
	else
	{
		AddCachedPoints(client, Witch_Points_Cost * WitchPenalty);
	}
	return Plugin_Continue;
}

PrintFinalMapPoints()
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsRealClient(i))
		{
			PrintMapTop(i);
			//PrintPoints(i);
		}
	}
}
public Action:StatsEvent_InfectedDeath(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "attacker"));
	if (!IsRealClient(client))
	{
		return;
	}
	if (GetClientTeam(client) == TEAM_INFECTED)
	{
		return;
	}
	if (GetEventBool(event, "headshot"))
	{
		AddCachedPoints(client, Common_Points_HeadShot);
	}
	else
	{
		AddCachedPoints(client, Common_Points);
	}
}

public Action:StatsEvent_PlayerDeath(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "attacker"));
	new target = GetClientOfUserId(GetEventInt(event, "userid"));
	if (target > 0)
	{
		if (GetClientTeam(target) == TEAM_SURVIVORS)
		{
			if (client == target)
			{
				AddCachedPoints(client, Player_Suicide_Points);
			}
			else
			{
				AddCachedPoints(target, Player_Death_Points);
			}
		}
	}
	else
	{
		return;
	}
	if (!IsRealClient(client))
	{
		return;
	}
	if (GetClientTeam(client) != TEAM_SURVIVORS)
	{
		return;
	}
	if (GetClientTeam(target) != TEAM_INFECTED)
	{
		return;
	}
	if (GetClientZC(target) == ZC_TANK)
//	decl String:strBuffer[48];
//	GetEventString(event, "victimname", strBuffer, sizeof(strBuffer));
//	if (StrEqual("Tank", strBuffer, false))
	{
		return;
	}
	if (GetEventBool(event, "headshot"))
	{
		AddCachedPoints(client, Boss_Points_HeadShot);
	}
	else
	{
		AddCachedPoints(client, Boss_Points);
	}
}

public Action:StatsEvent_PlayerIncap(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "attacker"));
	new target = GetClientOfUserId(GetEventInt(event, "userid"));
	if (GetClientTeam(target) == TEAM_SURVIVORS)
	{
		if (client == target)
		{
			AddCachedPoints(client, Player_SelfIncap_Points);
//			PrintToChat(client, "\x05Incap: return # 1");
			return;
		}
		else
		{
			AddCachedPoints(target, Player_Incap_Points);
//			PrintToChat(target, "\x05Incap: return # 2");
			return;
		}
	}
}

#if HEAL_POINTS
	public Action:StatsEvent_MedkitUsed(Handle:event, const String:name[], bool:dontBroadcast)
	{
		new target = GetClientOfUserId(GetEventInt(event, "subject"));
		new m_isGoingToDie = GetEntProp(target, Prop_Send, "m_isGoingToDie");
		switch (m_isGoingToDie)
		{
			case 0:
			{
	//			PrintToChat(client, "\x05Heal: case 0");
				AddCachedPoints(target, Heal_Points_Cost_Alive);
			}
			case 1:
			{
	//			PrintToChat(client, "\x05Heal: case 1");
				AddCachedPoints(target, Heal_Points_Cost_Bad);
			}
			case 2:
			{
	//			PrintToChat(client, "\x05Heal: case 2");
				AddCachedPoints(target, Heal_Points_Cost_Really_Bad);
			}
		}
//		PrintToChat(client, "\x05Heal: m_isGoingToDie = \x04%d\x05", m_isGoingToDie);
	}
#endif

public Action:StatsEvent_PillsUsed(Handle:event, const String:name[], bool:dontBroadcast)
{
#if HEAL_POINTS
	new target = GetClientOfUserId(GetEventInt(event, "subject"));
	new m_isGoingToDie = GetEntProp(target, Prop_Send, "m_isGoingToDie");
	switch (m_isGoingToDie)
	{
		case 0:
		{
//			PrintToChat(client, "\x05Heal: case 0");
			AddCachedPoints(target, Pills_Points_Cost_Alive);
		}
		case 1:
		{
//			PrintToChat(client, "\x05Heal: case 1");
			AddCachedPoints(target, Pills_Points_Cost_Bad);
		}
		case 2:
		{
//			PrintToChat(client, "\x05Heal: case 2");
			AddCachedPoints(target, Pills_Points_Cost_Really_Bad);
		}
	}
//		PrintToChat(client, "\x05Heal: m_isGoingToDie = \x04%d\x05", m_isGoingToDie);
#endif
}

public Action:StatsEvent_PlayerDefibed(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	new target = GetClientOfUserId(GetEventInt(event, "subject"));
	if (client == target)
	{
		return Plugin_Handled;
	}
	AddCachedPoints(target, Defib_Points);
	return Plugin_Continue;
}

public Action:StatsEvent_FinalWin(Handle:event, const String:name[], bool:dontBroadcast)
{
	Alive_Players = AlivePlayers();
	save_points_flag = true;
	PrintFinalMapPoints();
}

SavePlayersCountry(client, const String:SPC_Country[])
{
	if (!IsRealClient(client))
	{
//		PrintToChatAll("\x05AddCachedPoints(any:client, points_add) \x04Fail");
		return;
	}
	if (GetConVarInt(hm_saveplayerscountry) < 1)
	{
		return;
	}
	
	new String:clientauth[32];
	GetClientAuthString(client, clientauth, sizeof(clientauth));
	new Handle:Data = CreateKeyValues("points_data");
	FileToKeyValues(Data, datafilepath);
	
	new count;
	
	//search data or create it
	KvJumpToKey(Data, "data", true);
	
	//user in list?
	if (!KvJumpToKey(Data, clientauth))
	{
		//add to count 1 or create the count
//		KvGoBack(Data);
		KvGotoFirstSubKey(Data);
		KvJumpToKey(Data, "info", true);

		count = KvGetNum(Data, "count", 0);
		count++;

		KvSetNum(Data, "count", count);
		KvGoBack(Data);

		//add auth key
		KvGotoFirstSubKey(Data);
		KvJumpToKey(Data, "data", true);
		KvJumpToKey(Data, clientauth, true);
	}

		//set points and name
	KvSetString(Data, "country", SPC_Country);

	new points = KvGetNum(Data, "points", 0);
	KvSetNum(Data, "points", points);

	new String:clientname[MAX_NAME_LENGTH];
	GetClientName(client, clientname, sizeof(clientname));
	KvSetString(Data, "name", clientname);

	//save it
	KvRewind(Data);
	KeyValuesToFile(Data, datafilepath);

	CloseHandle(Data);
}

stock GetPoints(client)
{
	if (!IsRealClient(client))
	{
		return 0;
	}
	new String:clientauth[32];
	GetClientAuthString(client, clientauth, sizeof(clientauth));
	if (Top15Used)
	{
		return 0;
	}
	Top15Used = true;
	new Handle:Data = CreateKeyValues("points_data");
	new count;
	FileToKeyValues(Data, datafilepath);

	//search data or create it
	KvJumpToKey(Data, "data", true);

	//user in list?
	if (!KvJumpToKey(Data, clientauth))
	{
		//add to count 1 or create the count
//		KvGoBack(Data);
	
		KvGotoFirstSubKey(Data);
		KvJumpToKey(Data, "info", true);
			
		count = KvGetNum(Data, "count", 0);
		count++;
			
		KvSetNum(Data, "count", count);
//		KvGoBack(Data);
			
		//add auth key
		KvGotoFirstSubKey(Data);
		KvJumpToKey(Data, "data", true);
		KvJumpToKey(Data, clientauth, true);
	}
	new points = KvGetNum(Data, "points", 0);
	CloseHandle(Data);
	Top15Used = false;
	return points;
}

CalculatePoints(points)
{
	if (GetConVarInt(hm_usemapmultipliers) > 0)
	{
		points = RoundToZero(points * 1.0 * ReadMapMultiplier(current_map));
	}
	if (GetConVarInt(hm_countaliveplayers) > 0)
	{
		
		points = RoundToZero(points * 1.0 / GetConVarInt(FindConVar("sv_maxplayers")) * AlivePlayers());
//		points = RoundToZero(points * 1.0 / 16.0 * AlivePlayers());
	}
	return points;
}

AddPoints(any:client, points_add)
{
	if (Top15Used)
	{
		return;
	}
	Top15Used = true;
	if (!IsRealClient(client))
	{
//		PrintToChatAll("\x05AddCachedPoints(any:client, points_add) \x04Fail");
		return;
	}
	if (GetConVarInt(hm_statsmode) > 0)
	{
		if (GetConVarInt(hm_statsmode) == 1)
		{
			if (rounds > 0)
			{
				return;
			}
		}
	}
	else return;
	new String:clientname[MAX_NAME_LENGTH];
	GetClientName(client, clientname, sizeof(clientname));
	new String:clientauth[32];
	GetClientAuthString(client, clientauth, sizeof(clientauth));
	new Handle:Data = CreateKeyValues("points_data");
	new count, points; //
	FileToKeyValues(Data, datafilepath);

	//search data or create it
	KvJumpToKey(Data, "data", true);

	//user in list?
	if (!KvJumpToKey(Data, clientauth))
	{
		//add to count 1 or create the count
		KvGoBack(Data);
		KvJumpToKey(Data, "info", true);
			
		count = KvGetNum(Data, "count", 0);
		count++;
			
		KvSetNum(Data, "count", count);
			
		//add auth key
		KvGoBack(Data);
		KvJumpToKey(Data, "data", true);
		KvJumpToKey(Data, clientauth, true);
	}

		//get point number
	new current_points = KvGetNum(Data, "points", 0);
//	PrintToChat(client, "\x05Points = \x03%d \x05(\x03%d\x05 -> \x03%d\x05) \x04::\x05 Result = \x03%d", current_points, points_add, CalculatePoints(points_add), current_points + CalculatePoints(points_add));
	PrintToServer("[%N] Points = %d (%d -> %d) :: Result = %d", client, current_points, points_add, CalculatePoints(points_add), current_points + CalculatePoints(points_add));
	points = current_points + CalculatePoints(points_add);

		//set points and name
	KvSetNum(Data, "points", points);	
	KvSetString(Data, "name", clientname);

	//save it
	KvRewind(Data);
	KeyValuesToFile(Data, datafilepath);

	CloseHandle(Data);
	Top15Used = false;
}

AddCachedPoints(any:client, points_add)
{
	if (!IsRealClient(client))
	{
		return;
	}
	if (GetConVarInt(hm_statsmode) > 0)
	{
		if (GetConVarInt(hm_statsmode) == 1)
		{
			if (rounds > 1)
			{
				return;
			}
		}
	}
	else return;
//	PrintToChat(client, "\x05AddCachedPoints(): %d + %d", ChachedPoints[client], points_add);
	ChachedPoints[client] += points_add;
	if (draw_chached_points)
	{
		if (LastPrintTime[client] != GetGameTime())
		{
			decl String:pointsBuffer[10];
			Format(pointsBuffer, sizeof(pointsBuffer), "%d", ChachedPoints[client]);
			PrintCenterText(client, pointsBuffer);
			LastPrintTime[client] = GetGameTime();
		}
		else if (!ShowPointsTimerEnabled[client])
		{
			ShowPointsTimerEnabled[client] = true;
			CreateTimer(0.1, TimedShowPoints, client);
		}
	}
}

public Action:TimedShowPoints(Handle:timer, any:client)
{
	if (!IsValidEntity(client) || !IsClientInGame(client)) return;
	ShowPointsTimerEnabled[client] = false;
	decl String:pointsBuffer[10];
	Format(pointsBuffer, sizeof(pointsBuffer), "%d", ChachedPoints[client]);
	PrintCenterText(client, pointsBuffer);
}

SaveCachedPoints(client)
{
	if (!IsRealClient(client))
	{
		return;
	}
//	CreateTimer(0.1, TimedAddPoints, client, ChachedPoints[client]);
	if (!Top15Used)
	{
		AddPoints(client, ChachedPoints[client]);
	}
	else
	{
		PrintToServer("WARNING! CRASH PROTECTION! %N loose his map points", client);
		PrintToChatAll("\x04WARNING! CRASH PROTECTION! {blue}%N \x04loose his map points", client);
	}
}

ClearCachedPoints()
{
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		ChachedPoints[i] = 0;
	}
}

Float:ReadMapMultiplier(const String:MapFileName[])
{
	new Handle:Data = CreateKeyValues("points_data");
	FileToKeyValues(Data, datafilepath);
	
	//search data or create it
	KvJumpToKey(Data, "mapsdata", true);
	
	//user in list?
	if (!KvJumpToKey(Data, MapFileName))
	{
		//add to count 1 or create the count
		KvGoBack(Data);
		KvJumpToKey(Data, "Multiplier", true);
		
//		KvSetFloat(Data, "Multiplier", Multiplier);
		KvGoBack(Data);
			
		//add auth key
		KvJumpToKey(Data, "mapsdata", true);
		KvJumpToKey(Data, MapFileName, true);
	}

//	KvSetFloat(Data, "Multiplier", Multiplier);
	new Float:Result = KvGetFloat(Data, "Multiplier", 0.0);

	KvRewind(Data);
	KeyValuesToFile(Data, datafilepath);
//	PrintToChat(client, "\x05Points = \x03%d (%d)", points, points_add);

	CloseHandle(Data);
	return Result;
}

SetupMapMultiplier(const String:MapFileName[], Float:Multiplier)
{
	new Handle:Data = CreateKeyValues("points_data");
	FileToKeyValues(Data, datafilepath);
	
	//search data or create it
	KvJumpToKey(Data, "mapsdata", true);
	
	//user in list?
	if (!KvJumpToKey(Data, MapFileName))
	{
		//add to count 1 or create the count
		KvGoBack(Data);
		KvJumpToKey(Data, "Multiplier", true);

//		KvSetFloat(Data, "Multiplier", Multiplier);
		KvGoBack(Data);

		//add auth key
		KvJumpToKey(Data, "mapsdata", true);
		KvJumpToKey(Data, MapFileName, true);
	}

	KvSetFloat(Data, "Multiplier", Multiplier);

	KvRewind(Data);
	KeyValuesToFile(Data, datafilepath);
//	PrintToChat(client, "\x05Points = \x03%d (%d)", points, points_add);

	CloseHandle(Data);
	decl String:MapMultiplier[4];
	FloatToString(Multiplier, MapMultiplier, sizeof(MapMultiplier));

	PrintToChatAll("\x05New Map Multiplier for \x04%s\x05 is \x04%s", MapFileName, MapMultiplier);
}

#if PLAYERS_ENABLED
GrantPlayerLevel(client, rank)
{
	new grant_level = 1;
	if (rank <= 1400) grant_level += 1;
	if (rank <= 1200) grant_level += 1;
	if (rank <= 1000) grant_level += 1;
	if (rank <= 800) grant_level += 1;
	if (rank <= 650) grant_level += 1;
	if (rank <= 500) grant_level += 1;
	if (rank <= 400) grant_level += 1;
	if (rank <= 300) grant_level += 1;
	if (rank <= 250) grant_level += 1;
	if (rank <= 200) grant_level += 1;
	if (rank <= 150) grant_level += 1;
	if (rank <= 125) grant_level += 1;
	if (rank <= 100) grant_level += 1;
	if (rank <= 75) grant_level += 1;
	if (rank <= 50) grant_level += 1;
	if (rank <= 30) grant_level += 1;
	if (rank <= 20) grant_level += 1;
	if (rank <= 15) grant_level += 1;
	if (rank <= 13) grant_level += 1;
	if (rank <= 10) grant_level += 1;
	if (rank <= 7) grant_level += 1;
	if (rank <= 5) grant_level += 1;
	if (rank <= 3) grant_level += 1;
	if (rank < 2) grant_level += 1;
	if (GetAdminLvl(client) < grant_level) SetAdminLvl(client, grant_level);
//	ServerCommand("sm_grantlevel #%d %d", GetClientUserId(client), grant_level);
}
#endif

GrantPlayerColor(client)
{
	if (GetConVarInt(hm_stats_colors) < 1)
	{
		return;
	}
	if (!IsValidClient(client))
	{
		return;
	}
	if (!IsClientConnected(client) || !IsClientInGame(client) || GetClientTeam(client) != TEAM_SURVIVORS)
	{
		return;
	}
	if (!IsPlayerAlive(client))
	{
		return;
	}
	if (IsFakeClient(client))
	{
		if (GetConVarInt(hm_stats_bot_colors) < 1) SetEntityRenderColor(client, 255, 255, 255, 255);
		else SetEntityRenderColor(client, 175, 175, 175, 255);
		return;
	}
	if (GetConVarInt(hm_stats_colors) == 1)
	{
		if (ClientRank[client] > 0 && ClientRank[client] < 31)
		{
			if (ClientRank[client] < 4) SetEntityRenderColor(client, 250, 0, 0, 255);
			else if (ClientRank[client] < 11) SetEntityRenderColor(client, 0, 255, 0, 255);
			else SetEntityRenderColor(client, 0, 0, 255, 255);
		}
		else SetEntityRenderColor(client, 255, 255, 255, 255);
		return;
	}
	if (GetConVarInt(hm_stats_colors) == 2)
	{
		if (PlayerVIP[client]) SetEntityRenderColor(client, 0, 255, 0, 255);
		else if (PlayerLevel[client] < 91)
		{
			if (PlayerLevel[client] > 59) SetEntityRenderColor(client, 255, 0, 0, 255);
			else if (PlayerLevel[client] > 49) SetEntityRenderColor(client, 255, 128, 0, 255);
			else if (PlayerLevel[client] > 29) SetEntityRenderColor(client, 255, 255, 0, 255);
			else if (PlayerLevel[client] > 22) SetEntityRenderColor(client, 0, 0, 255, 255);
			else if (PlayerLevel[client] > 18) SetEntityRenderColor(client, 0, 0, 175, 255);
			else if (PlayerLevel[client] > 14) SetEntityRenderColor(client, 0, 0, 75, 255);
		}
		else
		{
			new AdminId:AId = GetUserAdmin(client);
			new flags = GetAdminFlags(AId, Access_Effective);		
			if (flags & ADMFLAG_CUSTOM1)
			{
				SetEntityRenderColor(client, 0, 175, 0, 255);
			}
			else SetEntityRenderColor(client, 255, 255, 255, 255);
		}
		return;
	}
	SetEntityRenderColor(client, 255, 255, 255, 255);
}

AlivePlayers()
{
	if (save_points_flag)
	{
		return Alive_Players;
	}
	new j = 0;
	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		if (IsClientConnected(i) && IsClientInGame(i))
		{
			if (GetClientTeam(i) == TEAM_SURVIVORS && IsPlayerAlive(i))
			{
				j++;
			}
		}
	}
	return j;
}

CStatsOnClientDisconnect(client)
{
	if (!GetConVarInt(hm_statsmode)) return;
	if (IsRealClient(client))
	{
		if (save_points_flag || ChachedPoints[client] < 0)
		{
			SaveCachedPoints(client);
		}
	}
	ChachedPoints[client] = 0;
}

CStatsOnClientConnected(client)
{
	ChachedPoints[client] = 0;
}

CStatsOnClientPostAdminCheck(client)
{
	CreateTimer(5.0, TimedRank, client);
}

CStatsMapTransitionEvent()
{
	Alive_Players = AlivePlayers();
	save_points_flag = true;
	PrintFinalMapPoints();
}

StatsEvent_PlayerNowIt(Handle:event)
{
	new target = GetClientOfUserId(GetEventInt(event, "userid"));
	if (!IsValidClient(target))
	{
		return;
	}
	new attacker = GetClientOfUserId(GetEventInt(event, "attacker"));
	if (attacker == 0) return;
	if (!IsClientConnected(attacker) || !IsClientInGame(attacker))
	{
		return;
	}
	else
	{
		if (IsFakeClient(attacker))
		{
			return;
		}
		if (GetClientTeam(attacker) != TEAM_SURVIVORS)
		{
			return;
		}
	}
	if (target == attacker)
	{
		AddCachedPoints(target, Vomit_Self_Points);
	}
	else if (GetClientTeam(target) == TEAM_SURVIVORS)
	{
		AddCachedPoints(attacker, Vomit_Teammate_Points);
	}
	else if (GetClientTeam(target) == TEAM_INFECTED)
	{
		if (GetClientZC(target) == ZC_TANK)
		{
			AddCachedPoints(attacker, Vomit_Tank_Points);
		}
	}
}

CSPlayerFirstSpawn(Handle:event)
{
	if (GetEventInt(event, "isbot") && GetConVarInt(hm_stats_bot_colors) < 1)
	{
		return;
	}
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	GrantPlayerColor(client);
}

CSPlayerSpawn(Handle:event)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	GrantPlayerColor(client);
}

CSOnClientDisconnectPost(client)
{
	ClientRank[client] = 0;
}

CSRoundStartEvent()
{
	Log("coop_stats.inc / CSRoundStartEvent() / 1", 2);
	new real_clients_count = 0;
	FirstRoundTimerCountdown = -1;
	FirstRoundTimerEnabled = false;

	for (new i = 1; i <= L4D_MAXPLAYERS; i++)
	{
		ShowPointsTimerEnabled[i] = false;
		if (IsValidEntity(i) && IsClientConnected(i))
		{
			if (IsClientInGame(i) && !IsFakeClient(i))
			{
				real_clients_count++;
			}
		}
	}
	if (rounds == 1 && IsFirstMapFlag) // && IsServerProcessing())
	{
		if (real_clients_count > 0)
		{
			Log("coop_stats.inc / CSRoundStartEvent() / 2 - if (real_clients_count > 0)", 2);
			StartFirstRoundTimer(70);
		}
	}
	if (GetConVarInt(hm_statsmode) != 1)
	{
		ClearCachedPoints();
		CreateTimer(0.1, TimedClearCachedPoints, _);
	}
	if (IsFirstRoundAfterLoad)
	{
		if (GetConVarInt(hm_stats_checkonfirstround) > 0)
		{
			Command_StatsCheckLimit(0, 0);
		}
	}
	else
	{
		for (new i = 1; i <= L4D_MAXPLAYERS; i++)
		{
			if (IsClientConnected(i) && IsClientInGame(i) && !IsFakeClient(i) && GetClientTeam(i) == TEAM_SURVIVORS)
			{
				CreateTimer(1.0, TimedGrantPlayerColor, i);
			}
		}
	}
}

public PrizeCircle(client, Float:position[3])
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
				PrizePlayer(i);
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
					PrizePlayer(i);
				}
			}
		}
	}
}

PrizePlayer(entity)
{
	if (IsIncapacitated(entity)) return;
	new revive_count = GetEntProp(entity, Prop_Send, "m_currentReviveCount");
	if (revive_count > 0)
	{
		PrintToConsole(entity, "[PrizeBox]: %d points!", PrizeBox_Penalty * revive_count);
		AddCachedPoints(entity, PrizeBox_Penalty * revive_count);
		return;
	}
	new bonus = GetHealth(entity) * 2 + GetClientTempHealth(entity);
	if (GetPlayerWeaponSlot(entity, 4) != -1)
	{
		decl String:SlotItemName[MAX_LINE_WIDTH];
		GetWeaponNameAtSlot(entity, 4, SlotItemName, MAX_LINE_WIDTH);
		if (StrEqual(SlotItemName, "weapon_pain_pills", false))
		{
			bonus += Weapon_Pain_Pills_Points;
		}
		else
		{
			bonus += Weapon_Adrenaline_Points;
		}
	}
	if (GetPlayerWeaponSlot(entity, 3) != -1)
	{
		decl String:SlotItemName[MAX_LINE_WIDTH];
		GetWeaponNameAtSlot(entity, 3, SlotItemName, MAX_LINE_WIDTH);
		if (StrEqual(SlotItemName, "weapon_first_aid_kit", false))
		{
			bonus += Weapon_MedKit_Points;
		}
		else if (StrEqual(SlotItemName, "weapon_defibrillator", false))
		{
			bonus += Weapon_Defibrillator_Points;
		}
	}
	PrintToConsole(entity, "[PrizeBox]: %d points!", bonus);
	AddCachedPoints(entity, bonus);
}

StartFirstRoundTimer(CountDown)
{
	Log("coop_stats.inc / StartFirstRoundTimer() / 1", 2);
	if (FirstRoundTimerEnabled) return;
	Log("coop_stats.inc / StartFirstRoundTimer() / 2 - NOT FirstRoundTimerEnabled", 2);
	FirstRoundTimerCountdown = CountDown;
	FirstRoundTimerEnabled = true;
	CreateTimer(1.0, TimedFirstRoundPoints);
}

CSGameInstructorNodraw()
{
	Log("coop_stats.inc / CSGameInstructorNodraw() / 1", 2);
	if (rounds == 1 && IsFirstMapFlag) // && IsServerProcessing())
	{
		Log("coop_stats.inc / CSGameInstructorNodraw() / 2 - rounds == 1 && IsFirstMapFlag", 2);
		StartFirstRoundTimer(70);
		return;
	}
	PrintToServer("CSGameInstructorNodraw(): rounds = %d, IsFirstMap = %b, IsSP = %b", rounds, IsFirstMapFlag, IsServerProcessing());
}