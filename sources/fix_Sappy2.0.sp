#define PLUGIN_VERSION "2.0beta"
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

/*=====================
  * Tag & Log & Sound *
=======================*/
#define FS		"[Sappy Fix]"
#define FD		"[Defib Fix]"
#define FM		"[Model Fix]"

#define LOG		"logs\\fix_sappy.log"

#define SOUND	"ui/beep_error01.wav"

#if debug
static	String:DEBUG[256];
#endif

/*=====================
      * ConVar *
=======================*/

static	Handle:g_Sf, Handle:g_Bebop, Handle:g_Survivor, Handle:g_Timer, Handle:g_Items, Handle:g_GameMode, 
		Handle:freeze, Handle:g_Def, Handle:g_Model, Handle:g_xTimer,
		
		bool:block[5], 
		
		bool:g_CvarSf, g_CvarMin, g_CvarMax, g_CvarItems, bool:g_CvarDef, afk[MAXPLAYERS+1], loading[MAXPLAYERS+1],

		Float:g_CvarTimer, Float:g_CvarXTimer, ID[MAXPLAYERS+1], Handle:zTimer[MAXPLAYERS+1], 
		String:sModel[64][MAXPLAYERS+1], sData[MAXPLAYERS+1];

	
public Plugin:myinfo =
{
	name = "[L4D & L4D2] Sappy Fix",
	author = "raziEiL [disawar1]",
	description = "Smells Like Bug Spirit ^^",
	version = PLUGIN_VERSION,
	url = "http://steamcommunity.com/id/raziEiL"
}

/*=====================
	* PLUGIN START! *
=======================*/
public OnPluginStart()
{
	#if debug
		BuildPath(Path_SM, DEBUG, sizeof(DEBUG), LOG);
	#endif

	CreateConVar("sappy_fix_version", PLUGIN_VERSION, "Sappy Bug Fix plugin version", FCVAR_REPLICATED|FCVAR_NOTIFY);
	
	g_Sf			=	CreateConVar("sappy_fix_sacrifice", "1", "1: Enable Sacrifice Bug Fix for survival mode, 0: Disable fix", FCVAR_PLUGIN, true, 0.0, true, 1.0);
	g_Bebop		=	CreateConVar("sappy_fix_bebop", "4", "Min amount of Survivors when round starts, 0: Disable fix if u not use bebop, MultiSlots, bot_l4d plugins. Note: sometimes bebop cant kick bot when player leave.", FCVAR_PLUGIN);
	g_Survivor	=	CreateConVar("sappy_fix_extrabots", "0", "Max amount of Survivors on your server, 0: Disable fix if u not use SuperVersus, L4D Players. Note: sometimes you can see more bots than in the your cfg", FCVAR_PLUGIN);
	g_Timer		=	CreateConVar("sappy_fix_timer", "10", "Check Survivors limit after x.x sec when round started, 0: Disable checking", FCVAR_PLUGIN);
	g_Items		=	CreateConVar("sappy_fix_dropitems", "1", "1: Delete all player or bot items when they disconnected, 0: Disable fix. Note: This options blocks item spam for bebop, multislot.. plugin", FCVAR_PLUGIN, true, 0.0, true, 1.0);
	g_Def			=	CreateConVar("sappy_fix_defibrillator", "1", "1: Ghost revive bug fix (Bebop), 0: Disable for left 4 dead.", FCVAR_PLUGIN, true, 0.0, true, 1.0);
	g_Model		=	CreateConVar("sappy_fix_models", "1", "Save survivors model every x.x sec and keep character when going idle, 0: Disable fix.", FCVAR_PLUGIN, true, 0.0, true, 5.0);
	AutoExecConfig(true, "fix_Sappy");
	
	g_GameMode	=	FindConVar("mp_gamemode");
	freeze		=	FindConVar("sb_stop");
	
	HookConVarChange(g_GameMode, OnGameModeChange);
	HookConVarChange(g_Bebop, OnCVarChange);
	HookConVarChange(g_Survivor, OnCVarChange);
	HookConVarChange(g_Timer, OnCVarChange);
	HookConVarChange(g_Items, OnCVarChange);
	HookConVarChange(g_Def, OnCVarChange);
	HookConVarChange(g_Model, OnCVarChange);
	
	#if debug
	RegAdminCmd("fx", CmdFix, ADMFLAG_KICK);
	#endif
}
#if debug
/* * * Test Command * * */
public Action:CmdFix(client, args)
{	
	for (new i = 1; i <= MaxClients; i++)
		if (IsClientInGame(i) && GetClientTeam(i) == 2)
			PrintToChatAll("%N (%d), ghost <%d>", i, i, ID[i]);

	new entity;
	while ((entity = FindEntityByClassname(entity , "survivor_death_model")) != -1){
		PrintToChatAll("ghost ent <%d>", entity);	
	}

	CreateTimer(0.01, SappyFix);
	return Plugin_Handled;
}
#endif

public OnMapStart()
{
	#if debug
		LogToFile(DEBUG, "%s MAP CHANGE TO ....", FS);
	#endif
	PrecacheSound(SOUND, true);
	
	ValidMode();
	
	if (g_CvarDef)
		GhostStatusToFalse();
}
/*										+==========================================+
										| AFK Models 'BUG' - this is not a bug real|
										|					(Saving Client Models) |
										+==========================================+	
*/
public Action:SaveEntityModel(Handle:timer)
{
	for (new i = 1; i <= MaxClients; i++){
	
		if (IsClientInGame(i) && GetClientTeam(i) == 2 && !IsFakeClient(i)){
		
			sData[i] = GetEntProp(i, Prop_Send, "m_survivorCharacter");	
			GetClientModel(i, sModel[i], sizeof(sModel));
		}
	}
}

public Action:Triger(Handle:timer, any:client)
{
	ChangeEntityModel(client, client);
	if (IsClientInGame(client) && !StrEqual(sModel[client], ""))
		LogMessage("%s Model Bug is Fixed!", FM);
}

ChangeEntityModel(client, fake)
{	
	if (IsClientInGame(client) && IsClientInGame(fake) && !StrEqual(sModel[client], "")){
	
		SetEntProp(fake, Prop_Send, "m_survivorCharacter", sData[client]);
		SetEntityModel(fake, sModel[client]);
		
		#if debug
			//PrintToChatAll("Change %N model to %N model <%s>", fake, client, sModel[client]);
			LogToFile(DEBUG, "%s Change %N to %N model <%s>", FM, fake, client, sModel[client]);
		#endif
	}
	#if debug
	if (StrEqual(sModel[client], ""))
		LogToFile(DEBUG, "%s uhm INDEX (%d) model <%s> [MISSING!]", FM, client, sModel[client]);
	#endif	
}
/*										+==========================================+
										|	 	 Sacrifice BUG FIX  'dead bots'	   |
										|								 (survival)|
										+==========================================+	
*/
ValidMode()
{
	decl String:mode[24];
	GetConVarString(g_GameMode, mode, sizeof(mode));
	
	if (strcmp(mode, "survival") == 0 && g_CvarSf)
		ValidMap();
}

ValidMap()
{
	decl String:map[5];
	GetCurrentMap(map, sizeof(map));

	if (strcmp(map, "c7m1") == 0 ||
		strcmp(map, "c7m3") == 0){
		new Handle:g_Rest = FindConVar("mp_restartgame");
		SetConVarInt(g_Rest, 1);
		
		#if debug
			LogToFile(DEBUG, "%s Valid map \"%s\"", FS, map);
			LogToFile(DEBUG, "%s Sacrifice Bug is fixed!", FS);
		#endif
		LogMessage("%s Sacrifice Bug is Fixed!", FS);
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
	
	if (client){
/*										+------------------------------------------+
										|			 Model Fix PART I 	 		   |
										+------------------------------------------+
*/
		if (g_CvarXTimer > 0 && GetClientTeam(client) != 3)
			ChangeEntityModel(client, fake);
/*										+------------------------------------------+
										|	 				END 				   |
										+------------------------------------------+
*/			
		if (g_CvarMin > 0){
		
			afk[client]=client;
			afk[fake]=afk[client];

			#if debug
				//PrintToChatAll("%s %N %d, %N %d [IDLE]", FS, client, client, fake, fake);
				LogToFile(DEBUG, "%s %N %d, %N %d [IDLE]", FS, client, client, fake, fake);
			#endif
		}
	}
}

public Action:PlayerBack(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	
	if (client && !IsFakeClient(client)){	
/*										+------------------------------------------+
										|			 Model Fix PART II 	 		   |
										+------------------------------------------+
*/	
		if (g_CvarXTimer > 0 && GetClientTeam(client) != 2)
			CreateTimer(0.01, Triger, client);
/*										+------------------------------------------+
										|	 				END 				   |
										+------------------------------------------+
*/		
		if (g_CvarMin > 0){
		
			if (loading[client] != 0 && GetClientTeam(client) != 1)
				CreateTimer(1.0, TakeOverBot, client); // client connected and takeover the control of alive survivor bot
				
			if (GetClientTeam(client) != 2)
				IdleStatus(client);
		}
	}
}

public Action:TakeOverBot(Handle:timer, any:client)
{
	loading[client] = 0;
	CreateTimer(0.1, SappyFix);

	#if debug
		LogToFile(DEBUG, "%s %N %d [CONNECTED!]", FS, client, loading[client]);
	#endif
}

public bool:OnClientConnect(client, String:rejectmsg[], maxlen)
{
	if (client && !IsFakeClient(client)){
		loading[client] = 1;

		#if debug
			LogToFile(DEBUG, "%s %N (%d) %d [CONNECT...]", FS, client, client, loading[client]);
		#endif
		
		return true;
	}
	return false;
}

public OnClientDisconnect(client)
{
	if (client){
	
		if (!IsFakeClient(client)){
		
				#if debug
					LogToFile(DEBUG, "%s INDEX (%d) [DISCONNECT]", FS, client);
				#endif
		
			if (loading[client] != 0) 
				loading[client] = 0;
				
			if (g_CvarMin != 0){
				IdleStatus(client);
				CreateTimer(1.0, SappyFix, client); // we wait when a bot join in the game
			}
			if (g_CvarXTimer > 0)
				Format(sModel[client], sizeof(sModel), ""); // clear player model
		}
		if (IsClientInGame(client) && IsFakeClient(client) && !IsBehopClient(client) && GetClientTeam(client) == 2){
			Items(client);
/*									+------------------------------------------+
									|	 	 Ghost PART III (Avtivate Event)   |
									+------------------------------------------+
*/
			if (g_CvarDef && !IsPlayerAlive(client)){
				#if debug
					LogToFile(DEBUG, "%s dead bot leave %N (%d)", FD, client, client);
				#endif
				
				DateBase(client);
			}
		}
		if (g_CvarDef)
			CreateTimer(1.5, ClearDB, client);	
	}
}
/*									+------------------------------------------+
									|	 				END 				   |
									+------------------------------------------+
*/	
public Action:ClearDB(Handle:timer, any:client)
{
	if (ID[client] != 0){
		#if debug
			LogToFile(DEBUG, "%s Clear ghost INDEX (%d) <%d> [NOT IN GAME]", FD, client, ID[client]);
		#endif
		ID[client] = 0;
	}
	TimeToKill(client);
}

public Action:SappyFix(Handle:timer, any:id)
{
	new fake, fafk, spectator, total, connected, log;

	for (new i = 1; i <= MaxClients; i++){
	
		if (loading[i] != 0)
			connected++;
			
		if (IsClientInGame(i) && GetClientTeam(i) != 3){
		
			if (GetClientTeam(i) == 2)
				total++;
				
			if (GetClientTeam(i) == 2 && IsFakeClient(i)){
				if (afk[i] == 0) fake++;
				else fafk++;
			}
			if (afk[i] != 0 && !IsFakeClient(i))
				spectator++;
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
					PrintToChatAll("%s kick %N", FS, i);
					LogToFile(DEBUG, "%s kick %N", FS, i);
				#endif

				DateBase(id);
			}
		}
	}
	#if debug
		new t=total-(fake+fafk);
		new f=fake+fafk;
		PrintToChatAll("Client: %d, Fake: %d, Spec: %d, Connect: %d", t, f, spectator, connected);
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
			LogMessage("%s Extra Survivors Bug is Fixed! [Bebop]", FS);
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
 - Defib fix when player was afk and his bot dies change bot ghost data to client data.
 */
	if (afk[client] != 0){

		for (new i = 1; i <= MaxClients; i++){
			if (i != client && IsClientInGame(i) && afk[i] == client){
				afk[i]=0;
				
				if (g_CvarDef && ID[i] != 0 && !IsPlayerAlive(i)){
					ID[client] = ID[i];
					ID[i] = 0;
					
					#if debug
						//PrintToChatAll("%s %N Ghost <%d> change to %N", FS, i, ID[client], client);
						LogToFile(DEBUG, "%s %%N Ghost <%d> change to %N", FS, i, ID[client], client);
					#endif
				}

				#if debug
					//PrintToChatAll("%s %N [leave][BACK], %N afk=0", FS, client, i);
					LogToFile(DEBUG, "%s %N [leave][BACK], %N afk=0", FS, client, i);
				#endif
				break;
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
	if (StrContains(name, "bebop_bot_fakeclient", false) != -1 ||
		StrContains(name, "I am not real.", false) != -1 || 
		StrContains(name, "FakeClient", false) != -1 ||
		StrContains(name, "Not in Ghost.", false) != -1 ||
		StrContains(name, "SurvivorBot", false) != -1){
		#if debug
			LogToFile(DEBUG, "%s DETECTED FAKE BOT '%s'", FS, name);
		#endif
		return true;
	}
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
	
		for (new x = 0; x <= 4; x++){
			new slot = GetPlayerWeaponSlot(client, x);
			if (slot != -1)
				RemovePlayerItem(client, slot);
		}
	}
}
/*										+------------------------------------------+
										|	 		Checking PART II   		(Timer)|
										+------------------------------------------+	
*/
public Action:RoundStart(Handle:event, String:event_name[], bool:dontBroadcast)
{
	if (g_CvarDef)
		GhostStatusToFalse();

	if (g_CvarTimer > 0)
		ValidTime();
}

ValidTime()
{
	#if debug
		LogToFile(DEBUG, "%s Cheking please wait...", FS);
	#endif

	SetConVarInt(freeze, 1);
	if (g_CvarMin > 0)
		CreateTimer(g_CvarTimer, SappyFix);	
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
		LogMessage("%s Extra Survivors Bug is Fixed! [SUPER VERSUS]", FS);
	}
	#if debug
		else LogToFile(DEBUG, "%s All is okay.", FS);
	#endif
	
	SetConVarInt(freeze, 0);
}
/*										+==========================================+
										|	 Defibrillator (Ghost, alive) BUG FIX  |
										|							 (all gamemode)|
										+==========================================+	
*/
public Action:Event_PlayerDead(Handle:event, String:event_name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	
	if (client && GetClientTeam(client) == 2 && ID[client] == 0){
		new entity = -1;
		
		while ((entity = FindEntityByClassname(entity , "survivor_death_model")) != -1){
			
			if (NotSavedEntity(entity)){
			
				ID[client] = entity;
				zTimer[client] = CreateTimer(0.5, IsClientRevived, client, TIMER_REPEAT);
				
				#if debug
					//PrintToChatAll("Save dead %N (%d) <%d>", client, client, entity);
					LogToFile(DEBUG, "%s Save dead %N (%d) <%d>", FD, client, client, entity);
				#endif
				break;
			}
		}	
	}
}

public Action:IsClientRevived(Handle:timer, any:client)
{
	if (IsClientInGame(client) && IsPlayerAlive(client)){

		#if debug
			//PrintToChatAll("Clear db %N (%d) <%d>", client, client, ID[client]);
			LogToFile(DEBUG, "%s Clear ghost %N (%d) <%d> [ALIVE]", FD, client, client, ID[client]);
		#endif
		
		ID[client] = 0;
		TimeToKill(client);
	}
}

TimeToKill(client)
{
	if (zTimer[client] != INVALID_HANDLE){
		KillTimer(zTimer[client]);
		zTimer[client] = INVALID_HANDLE;
	}
}

bool:NotSavedEntity(entity)
{
	for (new i = 1; i <= MaxClients; i++)
		if (ID[i] == entity)
			return false;
	return true;
}
/*										+------------------------------------------+
										| 	   Ghost ENDING  (Action fixing)	   |
										+------------------------------------------+
*/	
DateBase(const id)
{
	if (ID[id] != 0 && IsValidEntity(ID[id]) && IsValidEdict(ID[id])){
		#if debug
			//PrintToChatAll("%s AcceptEntityInput %N (%d) <%d> [GHOST DETECTED!]", FD, id, id, ID[id]);
			LogToFile(DEBUG, "%s AcceptEntityInput INDEX (%d) <%d> [GHOST DETECTED!]", FD, id, ID[id]);
		#endif

		AcceptEntityInput(ID[id], "Kill");
		
		/* Clear */
		ID[id] = 0;
	}
}
/*										+------------------------------------------+
										| 	   Ghost ENDING  (Action fixing)	   |
										+------------------------------------------+
static sound[MAXPLAYERS+1];
- Note: Equip slot 3 (defib) because the weapons can not be ammo.
stock SwitchWeapon(client)
{

	if (client && IsClientInGame(client)){
		
		new defib = GetPlayerWeaponSlot(client, 3);
		if (defib != -1){
			EquipPlayerWeapon(client, defib);
			
			if (!sound[client]){
				EmitSoundToClient(client, SOUND, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_CONVO, SND_NOFLAGS, SNDVOL_NORMAL, SNDPITCH_NORMAL, -1, NULL_VECTOR, NULL_VECTOR, true, 0.0);
				sound[client] = true;
				CreateTimer(2.0, EmitMe, client);
			}
		}
	}
}

public Action:EmitMe(Handle:timer, any:client) sound[client] = false;
*/
GhostStatusToFalse()
{
	for (new i = 1; i < MaxClients; i++){
		ID[i] = 0;
		TimeToKill(i);
	}
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

GetCVars()
{
	g_CvarSf = GetConVarBool(g_Sf);
	g_CvarMax = GetConVarInt(g_Survivor);
	g_CvarItems = GetConVarInt(g_Items);
}

Plugin()
{
	g_CvarMin = GetConVarInt(g_Bebop);
	g_CvarTimer = GetConVarFloat(g_Timer);
	g_CvarDef = GetConVarBool(g_Def);
	g_CvarXTimer = GetConVarFloat(g_Model);
	
	if (g_xTimer != INVALID_HANDLE){
		KillTimer(g_xTimer);
		g_xTimer = INVALID_HANDLE;
	}
	if (g_CvarXTimer != 0)
		g_xTimer = CreateTimer(g_CvarXTimer, SaveEntityModel, _, TIMER_REPEAT);
	
	if ((g_CvarTimer > 0 || g_CvarDef) && !block[1]){
	
		HookEvent("round_start", RoundStart, EventHookMode_PostNoCopy);
		block[1] = true;
		
		#if debug
		LogToFile(DEBUG, "%s HookEvent", FS);
		#endif
	}
	else if (g_CvarTimer == 0 && !g_CvarDef && block[1]){
		
		UnhookEvent("round_start", RoundStart, EventHookMode_PostNoCopy);
		block[1] = false;
		
		#if debug
		LogToFile(DEBUG, "%s UnhookEvent", FS);
		#endif
	}
	if ((g_CvarMin > 0 || g_CvarXTimer > 0) && !block[2]){
	
		HookEvent("player_bot_replace", PlayerIdle);
		block[2] = true;
		
		#if debug
		LogToFile(DEBUG, "%s HookEvent II", FS);
		#endif
	}
	else if (g_CvarMin == 0 && g_CvarXTimer == 0 && block[2]){
	
		UnhookEvent("player_bot_replace", PlayerIdle);
		block[2] = false;
		
		#if debug
		LogToFile(DEBUG, "%s UnhookEvent  II", FS);
		#endif
	}
	if ((g_CvarMin > 0 || g_CvarTimer > 0 || g_CvarXTimer > 0) && !block[3]){
		
		HookEvent("player_team", PlayerBack);
		block[3] = true;
		
		#if debug
		LogToFile(DEBUG, "%s HookEvent III", FS);
		#endif
	}
	else if (g_CvarMin == 0 && g_CvarTimer == 0 && g_CvarXTimer == 0 && block[3]){
	
		UnhookEvent("player_team", PlayerBack);
		block[3] = false;
		
		#if debug
		LogToFile(DEBUG, "%s UnhookEvent  III", FS);
		#endif
	}
	if (g_CvarDef && !block[4]){
	
		HookEvent("player_death", Event_PlayerDead);
		block[4] = true;
		
		#if debug
		LogToFile(DEBUG, "%s HookEvent Ghost I", FD);
		#endif
	}
	else if (!g_CvarDef && block[4]){
		
		UnhookEvent("player_dead", Event_PlayerDead);
		block[4] = false;
		
		#if debug
		LogToFile(DEBUG, "%s UnhookEvent Ghost I", FD);
		#endif
		
		GhostStatusToFalse();
	}
}

public APLRes:AskPluginLoad2(Handle:myself, bool:late, String:error[], err_max)
{
	if (!IsDedicatedServer()) 
		return APLRes_Failure;

	decl String:buffer[24];
	GetGameFolderName(buffer, sizeof(buffer));

	if (strcmp(buffer, "left4dead") == 0 || 
		strcmp(buffer, "left4dead2") == 0)
		return APLRes_Success;
	
	Format(buffer, sizeof(buffer), "Plugin not support \"%s\" game", buffer);
	strcopy(error, err_max, buffer);
	return APLRes_Failure;
}