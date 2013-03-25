#define PLUGIN_VERSION "1.0"
/*====================================================================================================================
======================================================================================================================

																		______818¶811118¶¶¶81_8111111__11¶¶1¶8____111
																		8111_181¶1____8¶¶¶¶¶¶¶¶¶¶¶¶¶811_____1888_____
																		1¶¶18181____1¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶1111_____8¶_____
																		1¶¶1181___1¶¶¶¶¶¶¶¶¶¶¶¶8818¶¶¶88111____188¶88
																		111111___18¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶1__18881111____1¶8¶
																		18¶8111_118118¶¶¶¶¶¶8¶¶¶8888___1111111____1¶8
 .d8888b.                                       8888888888 d8b          888111__8811881118811_11811181__11_11___1____
d88P  Y88b                                      888        Y8P          ___81__¶¶1181111_11188___81__88___1111___11__
Y88b.                                           888                     ___81_1¶1181______111881__11__181____11____11
 "Y888b.    8888b.  88888b.  88888b.  888  888  8888888    888 888  888 __88__¶¶1111_________1181__11___81__1_11____1
    "Y88b.     "88b 888 "88b 888 "88b 888  888  888        888 `Y8bd8P' _181_1¶¶811___1_________881_1____11____11_1__
      "888 .d888888 888  888 888  888 888  888  888        888   X88K   _¶11_¶¶¶81_1______________11_1____1_____11_1_
Y88b  d88P 888  888 888 d88P 888 d88P Y88b 888  888        888 .d8""8b. 1¶811¶¶¶¶1__1_________________1_________1_111
 "Y8888P"  "Y888888 88888P"  88888P"   "Y88888  888        888 888  888 88¶18¶¶¶8111_1_________________1____1___1__11
                    888      888           888                          ¶¶¶_¶¶¶¶11______________________11___8__11_11
                    888      888      Y8b d88P                          8¶8_¶¶¶1_________________________8¶8__1__8_1_
                    888      888       "Y88P"	  						¶¶11¶¶¶¶¶¶¶¶¶¶¶811______1118¶¶¶81_1¶1_1__11_1
																		¶¶1¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶81__1888¶¶¶111_1¶__1_1811
																		¶¶1¶¶¶¶¶¶¶¶¶¶¶¶1¶¶¶¶¶_____18_1¶¶81¶¶¶¶__1_881
																		¶¶1¶¶¶¶¶¶¶¶1881__18¶1______1__11___1¶¶__111¶8
																		¶88¶¶¶118¶8111____111_______________1¶1_11188
																		¶18¶¶¶1__111_____11¶1________________11118118
																		8_¶¶¶¶81_________18¶1_________________8118818
																		81¶¶¶¶¶811_______8¶1__________________11_8811
																		81¶¶¶¶¶¶11111___18¶¶1_________________811_¶_1
																		818¶¶¶¶¶¶11111__1¶¶¶¶8118¶____________811_81_
888                                             d8b 8888888888 d8b 888  ¶18¶¶¶¶¶¶8811___1¶¶¶¶¶¶__1____________818_81_
888                                             Y8P 888        Y8P 888  ¶88¶¶¶¶¶¶¶¶81_1118¶¶1181______________188181_
888                                                 888            888  ¶88¶¶¶¶¶¶¶¶¶8111118___1_______________88118__
88888b.  888  888     888d888  8888b.  88888888 888 8888888    888 888  ¶¶8¶¶¶¶¶¶¶¶¶8¶¶¶¶¶¶¶¶11111_1181_______8¶1881_
888 "88b 888  888     888P"       "88b    d88P  888 888        888 888  ¶¶¶8¶¶¶¶¶¶¶¶8118¶¶¶1111___1___1_______¶¶1881_
888  888 888  888     888     .d888888   d88P   888 888        888 888  88¶¶¶¶¶¶¶¶¶¶81_118888¶8811_____1_____8¶81¶11_
888 d88P Y88b 888     888     888  888  d88P    888 888        888 888  ¶¶8¶¶¶¶¶¶¶¶¶¶¶81111118811_____1111111¶¶888111
88888P"   "Y88888     888     "Y888888 88888888 888 8888888888 8888888888888¶¶¶¶¶¶¶¶¶¶¶11111_________1118¶11181881__1
              888    													¶¶¶1¶88¶¶¶¶¶¶¶¶88111_181__11118¶¶8111_181___1
         Y8b d88P                                						¶¶¶1¶¶8¶¶¶¶¶¶¶¶¶¶811_11111_118¶81__1___811__8
          "Y88P"														11818¶8_8¶¶¶¶¶¶¶¶¶¶¶¶8¶¶8¶¶¶811____11__11_111
																		_____811_¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶811_______1__8__11__
																		11___181__8¶¶¶¶¶¶¶¶¶¶¶811_____________¶8111__
					+-+-+-+-+-+-+-+-+ +-+-+-+-+							_____111___1¶¶8¶¶¶88881111____________¶¶818__
					|R|e|m|e|m|b|e|r| |K|u|r|t|							_____1¶_1____181111111111_____________¶¶118__
					+-+-+-+-+-+-+-+-+ +-+-+-+-+							__1__1¶1_1_____11111_________________1¶¶_1118
					
=====================================================================================================================
====================================================================================================================*/
#include <sourcemod>
#include <sdktools>
#pragma semicolon 1

/*=====< debug >=====*/
#define debug 0 // on,off
#if debug
#endif

/*=====================
     * Tag & Log *
=======================*/
#define FS		"[Sappy Fix]"
#define LOG		"logs\\fix_sappy.log"

#if debug
new	String:DEBUG[256];
#endif

/*=====================
      * ConVar *
=======================*/
new		Handle:g_Sf, Handle:g_Bebop, Handle:g_Survivor, Handle:g_Timer, Handle:g_Items, Handle:g_GameMode, 
		Handle:freeze;

new 	bool:block, bool:bev, bool:bev2, bool:bev3;

new		g_CvarSf, g_CvarMin, g_CvarMax, g_CvarItems, afk[MAXPLAYERS+1], loading[MAXPLAYERS+1];

new		Float:g_CvarTimer;

public Plugin:myinfo =
{
	name = "[L4D & L4D2] Sappy Fix",
	author = "raziEiL [disawar1]",
	description = "Smells Like Bug Spirit ^^",
	version = PLUGIN_VERSION,
	url = "http://l4d.darkmental.ru"
}

/*=====================
	* PLUGIN START! *
=======================*/
public OnPluginStart()
{
	#if debug
	BuildPath(Path_SM, DEBUG, sizeof(DEBUG), LOG);
	#endif

	CreateConVar("sappy_fix_version", PLUGIN_VERSION, "Sappy Bug Fix plugin version", FCVAR_PLUGIN|FCVAR_SPONLY|FCVAR_NOTIFY|FCVAR_DONTRECORD);
	
	g_Sf		=	CreateConVar("sappy_fix_sacrifice", "1", "1: Enable Sacrifice Bug Fix for survival mode, 0: Disable fix");
	g_Bebop		=	CreateConVar("sappy_fix_behop", "4", "Min amount of Survivors when round starts, 0: Disable fix if u not use bebop, MultiSlots, bot_l4d plugins. Note: sometimes bebop cant kick bot when player leave.");
	g_Survivor	=	CreateConVar("sappy_fix_extrabots", "0", "Max amount of Survivors on your server, 0: Disable fix if u not use SuperVersus, L4D Players. Note: sometimes you can see more bots than in the your cfg");
	g_Timer		=	CreateConVar("sappy_fix_timer", "10", "Check Survivors limit after x.x sec when round started, 0: Disable checking");
	g_Items		=	CreateConVar("sappy_fix_dropitems", "1", "1: Delete all player or bot items when they disconnected, 0: Disable fix. Note: This options blocks item spam for bebop, multislot.. plugin");
	AutoExecConfig(true, "fix_Sappy");
	
	g_GameMode	=	FindConVar("mp_gamemode");
	freeze		=	FindConVar("sb_stop");
	
	HookConVarChange(g_GameMode, OnGameModeChange);
	HookConVarChange(g_Bebop, OnCVarChange);
	HookConVarChange(g_Survivor, OnCVarChange);
	HookConVarChange(g_Timer, OnCVarChange);
	HookConVarChange(g_Items, OnCVarChange);
	
	RegAdminCmd("fx", CmdFix, ADMFLAG_KICK);
}

/* * * Test Command * * */
public Action:CmdFix(client, args)
{
	//CreateTimer(0.01, DoIt2);
	ValidTime();
	return Plugin_Handled;
}

public OnMapStart()
{
	block=false;
	ValidMode();
}

/*										+==========================================+
										|	 	 Sacrifice BOT BUG FIX  		   |
										|								 (survival)|
										+==========================================+	
*/
public ValidMode()
{
	decl String:mode[32];
	GetConVarString(g_GameMode, mode, sizeof(mode));
	
	if (strcmp(mode, "survival") == 0 && g_CvarSf == 1){
	
		ValidMap();
	}
}

ValidMap()
{
	decl String:map[64];
	GetCurrentMap(map, sizeof(map));
	
	if (strcmp(map, "c7m1_docks") == 0 ||
		strcmp(map, "c7m3_port") == 0){
		new Handle:g_Rest = FindConVar("mp_restartgame");
		SetConVarInt(g_Rest, 1);
		
		#if debug
		LogToFile(DEBUG, "%s Valid map \"%s\"", FS, map);
		LogToFile(DEBUG, "%s Sacrifice Bug is fixed!", FS);
		#endif
	}
}

/*										+==========================================+
										|	 	 	Bebop BUG FIX  				   |
										|							 (all gamemode)|
										+==========================================+	
*/
public Action:PlayerIdle(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "player"));
	new fake = GetClientOfUserId(GetEventInt(event, "bot"));
	
	if (client > 0){
		afk[client]=client;
		afk[fake]=afk[client];
		
		#if debug
		LogToFile(DEBUG, "%s %N %d, %N %d [IDLE]", FS, client, client, fake, fake);
		#endif
	}
}

public Action:PlayerBack(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	
	if (client > 0 && !IsFakeClient(client)){
/*										+==========================================+
										|	 		Checking PART I   			   |
										+==========================================+
*/
		if (!block && g_CvarTimer > 0){
			#if debug
			LogToFile(DEBUG, "%s Client activated Cheking!", FS);
			#endif
			
			block=true;
			ValidTime();
		}
/*										+==========================================+
										|	 				END 				   |
										+==========================================+
*/		
		if (g_CvarMin > 0){
			if (loading[client] != 0){
			
				loading[client]=0;
				
				#if debug
				LogToFile(DEBUG, "%s %N %d [CONNECTED!]", FS, client, loading[client]);
				#endif
			}
			if (GetClientTeam(client) != 2) 
				IdleStatus(client);
		}
	}
}

public bool:OnClientConnect(client, String:rejectmsg[], maxlen)
{
	if (client > 0 && !IsFakeClient(client)){
		loading[client]=1;
		
		#if debug
		LogToFile(DEBUG, "%s %N %d [CONNECT...]", FS, client, loading[client]);
		#endif
		
		return true;
	}
	return false;
}

public OnClientDisconnect(client)
{
	if (client > 0){
	
		if (IsClientConnected(client) && loading[client] != 0){
			loading[client]=0;
			
			#if debug
			LogToFile(DEBUG, "%s %N %d [DISCONNECT]", FS, client, loading[client]);
			#endif
		}
		if (IsClientInGame(client)){
		
			if (!IsFakeClient(client) && g_CvarMin != 0){
				IdleStatus(client);
				CreateTimer(1.0, DoIt2);
			}
			if (IsFakeClient(client) && !IsBehopClient(client) && GetClientTeam(client) == 2){
				Items(client);
			}
		}
	}
}

public Action:DoIt2(Handle:timer)
{
	new fake=0, fafk=0, spectator=0, total=0, connected=0, log=0;

	for (new i = 1; i <= MaxClients; i++){
	
		if (loading[i] != 0)
			connected++;
			
		if (IsClientInGame(i) && GetClientTeam(i) != 3)
		{
			if (GetClientTeam(i) == 2)
				total++;
			if (GetClientTeam(i) == 2 && IsFakeClient(i))
			{
				if (afk[i] == 0) fake++;
				else fafk++;
			}
			if (afk[i] != 0 && !IsFakeClient(i))
				spectator++;
			#if debug
			if (IsBehopClient(i))
				LogToFile(DEBUG, "%s hmm %N", FS, i);
			#endif
		}
	}
	for (new i = 1; i <= MaxClients; i++){
	
		if (IsClientInGame(i) && GetClientTeam(i) == 2)
		{
			if (total > g_CvarMin && fake > connected && IsFakeClient(i) && !IsBehopClient(i) && afk[i] == 0)
			{
				total--;
				fake--;
				log++;
				KickClient(i);
				
				#if debug
//				PrintToChatAll("%s kick %N", FS, i);
				LogToFile(DEBUG, "%s kick %N", FS, i);
				#endif
			}
		}
	}
	#if debug
	new t=total-(fake+fafk);
	new f=fake+fafk;
//	PrintToChatAll("Client: %d, Fake: %d, Spec: %d, Connect: %d", t, f, spectator, connected);
	LogToFile(DEBUG, "%s Client: %d, Fake: %d, Spec: %d, Connect: %d", FS, t, f, spectator, connected);
	#endif	
	new z = GetConVarInt(freeze);
	if (z != 0){
		if (g_CvarItems == 0){
			g_CvarItems = 1;
			CreateTimer(2.0, DisableItems);
		}
		SetConVarInt(freeze, 0);
		if (log != 0){
			#if debug
			LogToFile(DEBUG, "%s Detected \"%d\" extra bot, they have been removed.", FS, log);
			LogToFile(DEBUG, "%s Extra Survivors Bug is fixed! [Bebop]", FS);
			#endif
		}
		#if debug
		else LogToFile(DEBUG, "%s All is okay.", FS);
		#endif
	}
}

IdleStatus(client)
{
/*
 - Function:
 - When Idle player leave game find his bot and change afk status to 0.
 - When Idle player back change bot with client afk status to 0. (work with !spectate etc cmd.)
 */
	if (afk[client] != 0){
		for (new i = 1; i <= MaxClients; i++){
			if (IsClientInGame(i) && afk[i] == client && i != client){
				afk[i]=0;
				
				#if debug
				LogToFile(DEBUG, "%s %N [leave][BACK], %N afk=0", FS, client, i);
				#endif
			}
		}
		afk[client]=0;
	}
}

bool:IsBehopClient(client)
{
/* 
 - Note: fake client created for player, 
			different plugins use the same names.

	+==========================================+
		fake name   		|	plugin		
	|==========================================|
	 "I am not real."		|	bot_l4d	
	 "bebop_bot_fakeclient"	|	bebop
	 "FakeClient"			|	MultiSlots
	 "Not in Ghost."		|	L4D Players
	 "SurvivorBot"			|	SuperVersus

 - Function:
 - dont delete iteam when fake bot created for player.
 - dont kick fake bot when some player leave.
*/ 
	decl String:name[32];
	GetClientName(client, name, sizeof(name));
	if (strcmp(name, "bebop_bot_fakeclient") == 0 ||
		strcmp(name, "I am not real.") == 0 || 
		strcmp(name, "FakeClient") == 0 ||
		strcmp(name, "Not in Ghost.") == 0 ||
		strcmp(name, "SurvivorBot") == 0)
		return true;
	return false;
}
/*										+==========================================+
										|	 	 	Item Spam FIX  				   |
										|							 (all gamemode)|
										+==========================================+	
*/
public Action:DisableItems(Handle:timer)
{
	g_CvarItems = 0;
}

Items(client)
{
	if (g_CvarItems == 1){
		//PrintToChatAll("%s %N del items", FS, client);
		new weapons = GetPlayerWeaponSlot(client, 0);
		if (weapons != -1)
			RemovePlayerItem(client, weapons);

		new pistol = GetPlayerWeaponSlot(client, 1);
		if (pistol != -1)	
			RemovePlayerItem(client, pistol);
			
		new bomb = GetPlayerWeaponSlot(client, 2);
		if (bomb != -1)	
			RemovePlayerItem(client, bomb);

		new medkit = GetPlayerWeaponSlot(client, 3);
		if (medkit != -1)	
			RemovePlayerItem(client, medkit);
			
		new pills = GetPlayerWeaponSlot(client, 4);
		if (pills != -1)	
			RemovePlayerItem(client, pills);
	}
}
/*										+==========================================+
										|	 		Checking PART II   			   |
										|									(Timer)|
										+==========================================+	
*/
public Action:RoundStart(Handle:event, String:event_name[], bool:dontBroadcast)
{
	if (block)
		ValidTime();
}

public ValidTime()
{
	#if debug
	LogToFile(DEBUG, "%s Cheking please wait...", FS);
	#endif

	SetConVarInt(freeze, 1);
	if (g_CvarMin > 0)
		CreateTimer(g_CvarTimer, DoIt2);	
	else if (g_CvarMax > 0)
		CreateTimer(g_CvarTimer, DoIt);	
}
/*										+==========================================+
										|	 	Extra SURVIVORS Bots BUG FIX       |
										|							 (all gamemode)|
										+==========================================+	
*/
public Action:DoIt(Handle:timer)
{
	ValidLimit();
}

public ValidLimit()
{
	new x=0, k=0;

	for (new i = 1; i <= MaxClients; i++)
	{
		if (IsClientInGame(i) && GetClientTeam(i) == 2)
			x++;
	}
	for (new i = 1; i <= MaxClients; i++)
	{
		if (IsClientInGame(i) && GetClientTeam(i) == 2)
		{
			if (x > g_CvarMax && IsFakeClient(i)){
				k++;
				x--;
				KickClient(i);	
			}
		}
	}
	if (k != 0){
		#if debug
		LogToFile(DEBUG, "%s Detected \"%d\" extra bot, they have been removed.", FS, k);
		LogToFile(DEBUG, "%s Extra Survivors Bug is fixed! [SUPER VERSUS]", FS);
		#endif
	}
	#if debug
	else LogToFile(DEBUG, "%s All is okay.", FS);
	#endif
	
	SetConVarInt(freeze, 0);
}

/*=====================
	* GetConVar *
=======================*/
public OnGameModeChange(Handle:convar_hndl, const String:oldValue[], const String:newValue[])
{
	ValidMode();
}

public OnCVarChange(Handle:convar_hndl, const String:oldValue[], const String:newValue[])
{
	GetCVars();
	Plugin();
}

public OnConfigsExecuted()
{
	GetCVars();
	Plugin();
}

public GetCVars()
{
	g_CvarSf = GetConVarInt(g_Sf);
	g_CvarMax = GetConVarInt(g_Survivor);
	g_CvarItems = GetConVarInt(g_Items);
}

public Plugin()
{
	g_CvarMin = GetConVarInt(g_Bebop);
	g_CvarTimer = GetConVarFloat(g_Timer);
	
	if (g_CvarTimer > 0 && !bev){
		HookEvent("round_start", RoundStart, EventHookMode_PostNoCopy);
		bev = true;
		#if debug
		LogToFile(DEBUG, "%s HookEvent", FS);
		#endif
	}
	else if (g_CvarTimer == 0 && bev){
		UnhookEvent("round_start", RoundStart);
		bev = false;
		#if debug
		LogToFile(DEBUG, "%s UnhookEvent", FS);
		#endif
	}
	if (g_CvarMin > 0 && !bev2){
		HookEvent("player_bot_replace", PlayerIdle);
		bev2 = true;
		#if debug
		LogToFile(DEBUG, "%s HookEvent II", FS);
		#endif
	}
	else if (g_CvarMin == 0 && bev2){
		UnhookEvent("player_bot_replace", PlayerIdle);
		bev2 = false;
		#if debug
		LogToFile(DEBUG, "%s UnhookEvent  II", FS);
		#endif
	}
	if (!bev3){
		if (g_CvarMin > 0 || g_CvarTimer > 0){
			HookEvent("player_team", PlayerBack);
			bev3 = true;
			#if debug
			LogToFile(DEBUG, "%s HookEvent III", FS);
			#endif
		}
	}
	if (bev3){
		if (g_CvarMin == 0 && g_CvarTimer == 0){
			UnhookEvent("player_team", PlayerBack);
			bev3 = false;
			#if debug
			LogToFile(DEBUG, "%s UnhookEvent  III", FS);
			#endif
		}
	}
}

public APLRes:AskPluginLoad2(Handle:myself, bool:late, String:error[], err_max)
{
	decl String:game[32];
	GetGameFolderName(game, sizeof(game));

	if (!StrEqual(game, "left4dead", false) &&
		!StrEqual(game, "left4dead2", false) ||
		!IsDedicatedServer())
		return APLRes_Failure;
	return APLRes_Success;
}