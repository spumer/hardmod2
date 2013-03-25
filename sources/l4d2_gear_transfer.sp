/*=======================================================================================
	Plugin Info:

*	Name	:	[L4D2] Gear Transfer
*	Version	:	1.2.0
*	Author	:	SilverShot
*	Link	:	http://forums.alliedmods.net/showthread.php?t=137616

========================================================================================
	Change Log:

*	1.2.0
	- Added defibrillators, first aid kits, explosive and incendiary rounds to transfers.
	- Added cvars to allow/disallow the transfer of certain items.
	- Renamed more appropriately to "Gear Transfer".

*	1.1.2
	- Removed HookSingleEntityOutput, which was causing crashes.

*	1.1.1
	- Some fixes.

*	1.1
	- Added AtomicStryker's Vocalize with scenes.

*	1.03
	- Removed Vocalize stuff.

*	1.02
	- Changed things AtomicStryker suggested.

*	1.0.1
	- Fixed UnhookEvent error.
	- Added check in case of over 64 grenades.

*	1.0
	- Initial release.

========================================================================================

	This plugin was made using source code from the following plugins.
	If I have used your code and not credited you, please let me know.

*	Thanks to "DJ_WEST" for "[L4D/L4D2] Grenade Transfer"
	http://forums.alliedmods.net/showthread.php?t=122293

*	Thanks to "AtomicStryker" for "[L4D & L4D2] Boomer Splash Damage"
	http://forums.alliedmods.net/showthread.php?t=98794

*	Thanks to "Crimson_Fox" for "[L4D2] Weapon Unlock"
	http://forums.alliedmods.net/showthread.php?t=114296

*	Thanks to "AtomicStryker" for "L4D2 Vocalize ANYTHING"
	http://forums.alliedmods.net/showthread.php?t=122270

======================================================================================*/

#include <sourcemod>
#include <sdktools>

#pragma semicolon 1
#define SOUND_BIGREWARD "UI/BigReward.wav"			// Give
#define SOUND_LITTLEREWARD "UI/LittleReward.wav"	// Receive
#define ITEMZ					64					// How many items we store
#define CVAR_FLAGS				FCVAR_PLUGIN|FCVAR_SPONLY|FCVAR_NOTIFY
#define PLUGIN_VERSION			"1.2.0"


new g_Frame;
new bool:b_Versus;
static const Float:TRACE_TOLERANCE = 35.0;
new i_HasTransferred[64];
new i_ItemCount;
new i_ItemSpawnID[ITEMZ];
new Float:f_ItemSpawn_XYZ[ITEMZ][3];
new Float:f_GetItemTimer;

new bool:b_AllowMol;
new bool:b_AllowPip;
new bool:b_AllowVom;
new bool:b_AllowExp;
new bool:b_AllowInc;
new bool:b_AllowDef;
new bool:b_AllowFir;
new bool:b_Enable;
new bool:b_AutoGive;
new bool:b_AutoGrab;
new Float:f_DistGive;
new Float:f_DistGrab;
new i_Method;
new bool:b_Sounds;
new Float:f_Timer;
new Handle:h_AutoTimer = INVALID_HANDLE;

// Cvars
new Handle:h_AllowMol = INVALID_HANDLE;
new Handle:h_AllowPip = INVALID_HANDLE;
new Handle:h_AllowVom = INVALID_HANDLE;
new Handle:h_AllowExp = INVALID_HANDLE;
new Handle:h_AllowInc = INVALID_HANDLE;
new Handle:h_AllowDef = INVALID_HANDLE;
new Handle:h_AllowFir = INVALID_HANDLE;
new Handle:h_Enable = INVALID_HANDLE;
new Handle:h_AutoGrab = INVALID_HANDLE;
new Handle:h_AutoGive = INVALID_HANDLE;
new Handle:h_DistGrab = INVALID_HANDLE;
new Handle:h_DistGive = INVALID_HANDLE;
new Handle:h_Sounds = INVALID_HANDLE;
new Handle:h_Timer = INVALID_HANDLE;
new Handle:h_Method = INVALID_HANDLE;


// Items to transfer
new const String:g_Pickups[7][] =
{
	"weapon_molotov",
	"weapon_pipe_bomb",
	"weapon_vomitjar",
	"weapon_upgradepack_explosive",
	"weapon_upgradepack_incendiary",
	"weapon_defibrillator",
	"weapon_first_aid_kit"
};


// Vocalize Coach
new const String:g_Coach[5][] =
{
	"takepipebomb01",
	"takepipebomb02",
	"takepipebomb03",
	"takemolotov01",
	"takemolotov02"
};
// Vocalize Nick
new const String:g_Nick[4][] =
{
	"takepipebomb01",
	"takepipebomb02",
	"takemolotov01",
	"takemolotov02"
};
// Vocalize Ellis
new const String:g_Ellis[11][] =
{
	"takepipebomb01",
	"takepipebomb02",
	"takepipebomb03",
	"takemolotov01",
	"takemolotov02",
	"takemolotov03",
	"takemolotov04",
	"takemolotov05",
	"takemolotov06",
	"takemolotov07",
	"takemolotov08"
};
// Vocalize Rochelle
new const String:g_Rochelle[6][] =
{
	"takepipebomb01",
	"takepipebomb02",
	"takemolotov01",
	"takemolotov02",
	"takemolotov03",
	"takemolotov04"
};


public Plugin:myinfo =
{
	name = "[L4D2] Gear Transfer",
	author = "SilverShot",
	description = "Survivor bots can automaticlaly pickup and give items. Players can switch, grab or give items.",
	version = PLUGIN_VERSION,
	url = "http://forums.alliedmods.net/showthread.php?t=137616"
}



/*======================================================================================
#####################			P L U G I N   S T A R T				####################
======================================================================================*/
public OnPluginStart()
{
	// Game check. Not strictly for L4D2 to allow L4D testing.
	decl String:s_GameName[128];
	GetGameFolderName(s_GameName, sizeof(s_GameName));
	if (StrContains(s_GameName, "left4dead") < 0) SetFailState("l4d2_gear_transfer plugin only supports L4D2");

	// Cvars
	h_AllowMol = CreateConVar("gt_allow_mol", "1", "0=Disables, 1=Enables the transfer of molotovs.", CVAR_FLAGS);
	h_AllowPip = CreateConVar("gt_allow_pip", "1", "0=Disables, 1=Enables the transfer of pipe bombs.", CVAR_FLAGS);
	h_AllowVom = CreateConVar("gt_allow_vom", "1", "0=Disables, 1=Enables the transfer of vomit jars.", CVAR_FLAGS);
	h_AllowExp = CreateConVar("gt_allow_exp", "1", "0=Disables, 1=Enables the transfer of explosive rounds.", CVAR_FLAGS);
	h_AllowInc = CreateConVar("gt_allow_inc", "1", "0=Disables, 1=Enables the transfer of incendiary ammo.", CVAR_FLAGS);
	h_AllowDef = CreateConVar("gt_allow_def", "1", "0=Disables, 1=Enables the transfer of defibrillators.", CVAR_FLAGS);
	h_AllowFir = CreateConVar("gt_allow_fir", "1", "0=Disables, 1=Enables the transfer of first aid kits.", CVAR_FLAGS);
	h_AutoGive = CreateConVar("gt_auto_give", "0", "0=Disables, 1=Enables. Make bots give their items to players with none.", CVAR_FLAGS);
	h_AutoGrab = CreateConVar("gt_auto_grab", "1", "0=Disables, 1=Enables. Make bots automatically pick up nearby items.", CVAR_FLAGS);
	h_Enable = CreateConVar("gt_enabled", "1", "0=Disables, 1=Enables. Turn the plugin on.", CVAR_FLAGS);
	h_DistGive = CreateConVar("gt_dist_give", "100.0", "How close you have to be to transfer an item.", CVAR_FLAGS);
	h_DistGrab = CreateConVar("gt_dist_grab", "150.0", "How close the bots need to be for them to pick up an item.", CVAR_FLAGS);
	h_Method = CreateConVar("gt_method", "2", "0=Shove only, 1=Reload key only, 2=Shove and Reload key to transfer items.", CVAR_FLAGS);
	h_Sounds = CreateConVar("gt_sounds", "1", "0=Disables, 1=Enables. Play a sound to the person giving/receiving an item.", CVAR_FLAGS);
	h_Timer = CreateConVar("gt_timer", "1.0", "How often to check the bot positions to items/survivors for auto grab/give.", CVAR_FLAGS, true, 0.5, true, 5.0);
	CreateConVar("gr_version", PLUGIN_VERSION, "Gear Transfer version", FCVAR_NOTIFY|FCVAR_REPLICATED);
	AutoExecConfig(true, "l4d2_gear_transfer");

	// Cvars hooks
	HookConVarChange(h_AllowMol, ConVarChanged_Cvar);
	HookConVarChange(h_AllowPip, ConVarChanged_Cvar);
	HookConVarChange(h_AllowVom, ConVarChanged_Cvar);
	HookConVarChange(h_AllowExp, ConVarChanged_Cvar);
	HookConVarChange(h_AllowInc, ConVarChanged_Cvar);
	HookConVarChange(h_AllowDef, ConVarChanged_Cvar);
	HookConVarChange(h_AllowFir, ConVarChanged_Cvar);
	HookConVarChange(h_Enable, ConVarChanged_Enable);
	HookConVarChange(h_AutoGive, ConVarChanged_Cvar);
	HookConVarChange(h_AutoGrab, ConVarChanged_Cvar);
	HookConVarChange(h_DistGive, ConVarChanged_Cvar);
	HookConVarChange(h_DistGrab, ConVarChanged_Cvar);
	HookConVarChange(h_Method, ConVarChanged_Cvar);
	HookConVarChange(h_Sounds, ConVarChanged_Cvar);
	HookConVarChange(h_Timer, ConVarChanged_Cvar);

	// Current cvars
	GetCvars();
	if (b_Enable) HookEvents();
}



/*======================================================================================
####################				M A P   S T A R T				####################
======================================================================================*/
public OnMapStart()
{
	// Reset array
	for (new i = 0; i < ITEMZ; i++) {
		ResetItemArrays(i);
	}

	// Cache sounds
	if (b_Sounds) {
		if (!IsSoundPrecached(SOUND_LITTLEREWARD)) PrecacheSound(SOUND_LITTLEREWARD, true);
		if (!IsSoundPrecached(SOUND_BIGREWARD)) PrecacheSound(SOUND_BIGREWARD, true);
	}


	// Do not allow auto give/grab in versus mode
	new String:s_GameMode[25];
	GetConVarString(FindConVar("mp_gamemode"), s_GameMode, sizeof(s_GameMode));
	if (StrEqual(s_GameMode,"versus",false)) b_Versus = true; else b_Versus = false;

	// Start auto Give/grab
	if (b_AutoGive || b_AutoGrab) MakeAutoTimer();
}


PlaySound(client, const String:s_Sound[32])
	EmitSoundToClient(client, s_Sound, SOUND_FROM_PLAYER, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL, SNDPITCH_NORMAL, -1, NULL_VECTOR, NULL_VECTOR, true, 0.0);



/*======================================================================================
############		C V A R   C H A N G E   -   H O O K   E V E N T S		############
======================================================================================*/
public ConVarChanged_Enable(Handle:convar, const String:oldValue[], const String:newValue[])
{
	b_Enable = GetConVarBool(h_Enable);
	new i = StringToInt(newValue);

	if (i > 0) {
		HookEvents();
		MakeAutoTimer();
	}else{
		UnhookEvents();
		KillAutoTimer();
	}
}


public ConVarChanged_Cvar(Handle:convar, const String:oldValue[], const String:newValue[])
{
	GetCvars();
	if (!b_AutoGive && !b_AutoGrab) KillAutoTimer(); else MakeAutoTimer();
}


HookEvents()
{
	HookEvent("player_shoved", Event_PlayerShoved);
}
UnhookEvents()
{
	UnhookEvent("player_shoved", Event_PlayerShoved);
}



/*======================================================================================
################					G E T   C V A R S					################
======================================================================================*/
GetCvars()
{
	b_AllowMol = GetConVarBool(h_AllowMol);
	b_AllowPip = GetConVarBool(h_AllowPip);
	b_AllowVom = GetConVarBool(h_AllowVom);
	b_AllowExp = GetConVarBool(h_AllowExp);
	b_AllowInc = GetConVarBool(h_AllowInc);
	b_AllowDef = GetConVarBool(h_AllowDef);
	b_AllowFir = GetConVarBool(h_AllowFir);
	b_AutoGive = GetConVarBool(h_AutoGive);
	b_AutoGrab = GetConVarBool(h_AutoGrab);
	b_Enable = GetConVarBool(h_Enable);
	f_DistGive = GetConVarFloat(h_DistGive);
	f_DistGrab = GetConVarFloat(h_DistGrab);
	i_Method = GetConVarInt(h_Method);
	b_Sounds = GetConVarBool(h_Sounds);
	f_Timer = GetConVarFloat(h_Timer);
}



/*======================================================================================
################		P L A Y E R   C M D   -   R E L O A D			################
======================================================================================*/
public Action:OnPlayerRunCmd(client, &buttons, &impulse, Float:vel[3], Float:angles[3], &weapon)
{
	if (!b_Enable || i_Method == 0) return Plugin_Continue;

	g_Frame++;
	if (g_Frame < 3) {return Plugin_Continue;} else {g_Frame = 0;}

	if (IsFakeClient(client)) return Plugin_Continue;

	if (buttons & IN_RELOAD)
	{
		if (i_HasTransferred[client] == 1) return Plugin_Continue;		// They just transferred, return
		new target = GetClientAimTarget(client, true);					// Who are they aiming at
		if (target < 0) return Plugin_Continue;							// Not valid, return

		if (GetClientTeam(target) == 2) {				// Aiming at a survivor
			decl Float:f_Client[3], Float:f_Target[3];	// Floats
			decl Float:f_Distance;

			GetClientAbsOrigin(client, f_Client);		// Get attacker position
			GetClientAbsOrigin(target, f_Target);		// Get target position
			f_Distance = GetVectorDistance(f_Client, f_Target);

			if (f_Distance < f_DistGive) {				// They are within range
				TransferItem(client, target);
			}
		}
	}
	return Plugin_Continue;
}



/*======================================================================================
####################			P L A Y E R   S H O V E D			####################
======================================================================================*/
public Action:Event_PlayerShoved(Handle:h_Event, const String:s_Name[], bool:b_DontBroadcast)
{
	if (i_Method == 1) return;							// Reload key only

	new i_UserID, i_Victim, i_Attacker;
	i_UserID = GetEventInt(h_Event, "attacker");
	i_Attacker = GetClientOfUserId(i_UserID);

	if (IsFakeClient(i_Attacker)) return;
	if (i_HasTransferred[i_Attacker] == 1) return;		// They just transferred, return

	i_UserID = GetEventInt(h_Event, "userid");
	i_Victim = GetClientOfUserId(i_UserID);
	TransferItem(i_Attacker, i_Victim, true);
}



/*======================================================================================
###############				T R A N S F E R   G R E N A D E				###############
======================================================================================*/
public Action:tmrResetTransfer(Handle:timer, any:client)
	i_HasTransferred[client] = 0;


TransferItem(i_Attacker, i_Victim, FromShove=false)
{
	// Survivors only
	if (GetClientTeam(i_Attacker) != 2 || GetClientTeam(i_Victim) != 2) return;
	
	// Stop transfer more than once a second
	if (i_HasTransferred[i_Attacker] == 1) return;	

	// Declare variables
	new i;
	new i_Slot;
	new bool:HoldingGrenade = false;	// Attacker is holding a grenade, thats what they want to switch...
	new bool:AttackerHolding = false;	// Attacker has item in hand, for switch
	new bool:AttackerGrenade = false;	// Attacker has a grenade
	new bool:AttackerSpecial = false;	// Attacker has first aid, defib or upgrade ammo
	new bool:VictimGrenade = false;		// Bot has a grenade
	new bool:VictimSpecial = false;		// Bot has first aid, defib or upgrade ammo
	new bool:VictimIsFake = false;
	decl String:s_SteamID[64];
	decl String:s_Weapon[32];			// Player weapon
	decl String:s_BotsItem[32];			// Temp string
	decl String:s_BotGrenade[32];		// Bot molotov, pipebomb, vomitjar
	decl String:s_BotSpecial[32];		// Bot firstaidkit, defib, incendiary, explosive


	// Get attackers weapon in hand, must be grenade/special ammo etc
	GetClientWeapon(i_Attacker, s_Weapon, sizeof(s_Weapon));
	for (i = 0; i < 7; i++) {
		if (StrEqual(s_Weapon, g_Pickups[i])) {
			AttackerHolding = true;		// Switch, give
			if (i < 3) HoldingGrenade = true;
			if (FromShove && i == 6) return;	// Don't allow medkits to be transferred from shoves, so they can heal!
		}
	}

	// Attacker has grenade
	i = GetPlayerWeaponSlot(i_Attacker, 2);
	if (i != -1) AttackerGrenade = true;

	// Attacker has special
	i = GetPlayerWeaponSlot(i_Attacker, 3);
	if (i != -1) AttackerSpecial = true;

	// Victim grenade
	i = GetPlayerWeaponSlot(i_Victim, 2);
	if (i != -1) {
		GetEdictClassname(i, s_BotGrenade, sizeof(s_BotGrenade));
		VictimGrenade = true;
	}

	// Victim special
	i = GetPlayerWeaponSlot(i_Victim, 3);
	if (i != -1) {
		GetEdictClassname(i, s_BotSpecial, sizeof(s_BotSpecial));
		VictimSpecial = true;
	}

	// Fake client, IsFakeClient returns true when someone is idle :/
	GetClientAuthString(i_Victim, s_SteamID, 64);
	if (StrEqual(s_SteamID,"BOT") == true) VictimIsFake = true;


	// ########## SWITCH ##########  -  If player with an item has shoved a bot also with an item, switch!
	if (VictimIsFake && AttackerHolding) {
		if (HoldingGrenade && AttackerGrenade && VictimGrenade || !HoldingGrenade && AttackerSpecial && VictimSpecial) {
			if (HoldingGrenade) {
				s_BotsItem = s_BotGrenade;
				i_Slot = 2;
			}else{
				s_BotsItem = s_BotSpecial;
				i_Slot = 3;
			}

			i_HasTransferred[i_Attacker] = 1;
			CreateTimer(1.0, tmrResetTransfer, i_Attacker);
			if (!AllowedToTransfer(s_BotsItem)) return;
			if (!AllowedToTransfer(s_Weapon)) return;

			if (b_Sounds) {
				PlaySound(i_Victim, SOUND_LITTLEREWARD);
				PlaySound(i_Attacker, SOUND_BIGREWARD);
			}

			StripWeapon(i_Attacker, i_Slot);
			StripWeapon(i_Victim, i_Slot);
			GiveItem(i_Attacker, s_BotsItem);
			GiveItem(i_Victim, s_Weapon);
			Vocalize(i_Attacker, s_BotsItem);

			// Switch to previous weapon to stop the bug where Molotovs appear with Pipe particles and vice versa.
			ClientCommand(i_Attacker, "lastinv");
			return;
		}
	}

	// ########## GIVE ##########  -  If player with a grenade has shoved a survivor without a grenade, transfer
	if (AttackerHolding) {
		if (HoldingGrenade && AttackerGrenade && !VictimGrenade || !HoldingGrenade && AttackerSpecial && !VictimSpecial)
		{
			i_HasTransferred[i_Attacker] = 1;
			CreateTimer(1.0, tmrResetTransfer, i_Attacker);
			if (!AllowedToTransfer(s_Weapon)) return;
			if (HoldingGrenade) {i_Slot = 2;} else {i_Slot = 3;}

			if (b_Sounds) PlaySound(i_Victim, SOUND_LITTLEREWARD);
			if (b_Sounds) PlaySound(i_Attacker, SOUND_BIGREWARD);

			StripWeapon(i_Attacker, i_Slot);
			GiveItem(i_Victim, s_Weapon);
			Vocalize(i_Victim, s_Weapon);
			return;
		}
	}

	// ########## GRAB ##########  -  If player with no grenade has shoved a bot with a grenade, transfer
	if (VictimIsFake) {
		if (!AttackerGrenade && VictimGrenade || !AttackerSpecial && VictimSpecial) {
			if (!AttackerGrenade && VictimGrenade) {
				s_BotsItem = s_BotGrenade;
				i_Slot = 2;
			}else{
				s_BotsItem = s_BotSpecial;
				i_Slot = 3;
			}

			i_HasTransferred[i_Attacker] = 1;
			CreateTimer(1.0, tmrResetTransfer, i_Attacker);
			if (!AllowedToTransfer(s_BotsItem)) return;

			if (b_Sounds) PlaySound(i_Attacker, SOUND_LITTLEREWARD); // Received item

			StripWeapon(i_Victim, i_Slot);
			GiveItem(i_Attacker, s_BotsItem);	// null not allowed
			Vocalize(i_Attacker, s_BotsItem);
			return;
		}
	}
}



/*======================================================================================
###############			A U T O   G I V E / G R A B   T I M E R			################
======================================================================================*/
// Timer to check the bot positions and their distance to the items
public Action:tmrAutoGiveGrab(Handle:timer)
{
	if (!b_Enable || b_Versus) return;
	if (!b_AutoGive && !b_AutoGrab) return;

	// Allows the timer to have a dynamic time.
	h_AutoTimer = CreateTimer(f_Timer, tmrAutoGiveGrab);


	// Update item locations min every 5 seconds if AutoGrab is enabled.
	if (b_AutoGrab) {
		f_GetItemTimer += f_Timer;
		if (f_GetItemTimer >= 5.0) {
			GetItemSpawns();
			f_GetItemTimer = 0.0;
		}
	}


	// Declare variables
	new i;
	decl Float:f_TargetPos[3]; // = item (auto grab)
	decl Float:f_ClientPos[3]; // = bot
	decl Float:f_PlayerPos[3]; // = human player
	decl String:s_EdictClassName[32];
	decl String:s_SteamID[64];
	decl String:s_Temp[32];
	new i_Slot;
	new client;
	new count;
	new bool:ClientIsFake;
	new bool:ClientGrenade;		// Attacker has a grenade
	new bool:ClientSpecial;		// Attacker has first aid, defib or upgrade ammo
	new bool:BotGrenade;
	new bool:BotSpecial;


	// Loop through the clients
	for (client = 1; client < MaxClients; client++) {

		// Client in game, team survivor, is alive
		if (IsClientInGame(client) && GetClientTeam(client) == 2 && GetClientHealth(client) > 0) {
		
			// Are they a bot, IsFakeClient returns true when someone is idle :/
			GetClientAuthString(client, s_SteamID, 64);
			if (StrEqual(s_SteamID,"BOT") == true) ClientIsFake = true; else ClientIsFake = false;

			if (GetPlayerWeaponSlot(client, 2) != -1) ClientGrenade = true; else ClientGrenade = false;
			if (GetPlayerWeaponSlot(client, 3) != -1) ClientSpecial = true; else ClientSpecial = false;


			// ########## AUTO GIVE ########## - Loop through bot positions and check if they can give grenades to player
			if (b_AutoGive) {

				if (!ClientIsFake) {							// If human player
					if (!ClientGrenade || !ClientSpecial) {		// Client must have none

						// Loop through bots
						for (new bot_client = 1; bot_client < MaxClients; bot_client++) {

							// Make sure bot_client survivor team and alive
							if (IsClientInGame(bot_client) && GetClientTeam(bot_client) == 2 && GetClientHealth(bot_client) > 0) {

								// Make sure they are a bot
								GetClientAuthString(bot_client, s_SteamID, 64);
								if (StrEqual(s_SteamID,"BOT") == true) {

									// bot_client should have a grenade or special ammo, to give client
									if (GetPlayerWeaponSlot(bot_client, 2) != -1) BotGrenade = true; else BotGrenade = false;
									if (GetPlayerWeaponSlot(bot_client, 3) != -1) BotSpecial = true; else BotSpecial = false;
									
									// Client has no grenade and bot does, -OR- client has no special and bot does
									i_Slot = -1;
									if (!ClientGrenade && BotGrenade) {i_Slot = 2;} else if (!ClientSpecial && BotSpecial) {i_Slot = 3;}
									if (i_Slot != -1) {

										GetClientEyePosition(client, f_PlayerPos);							// Position of player
										GetClientEyePosition(bot_client, f_ClientPos);						// Get bot position
										new Float:f_Distance = GetVectorDistance(f_ClientPos, f_PlayerPos);	// Distance between client and bot

										// We're close enough, and players are visible, transfer item!
										if (f_Distance <= f_DistGive && IsVisibleTo(f_ClientPos, f_PlayerPos)) {

											i = GetPlayerWeaponSlot(bot_client, i_Slot);	// Get bots item name
											if (i > -1) {									// Definately holding a weapon?
												GetEdictClassname(i, s_EdictClassName, sizeof(s_EdictClassName));
												if (AllowedToTransfer(s_EdictClassName)) {
													if (b_Sounds) PlaySound(client, SOUND_LITTLEREWARD);

													StripWeapon(bot_client, i_Slot);
													GiveItem(client, s_EdictClassName);
													Vocalize(client, s_EdictClassName);
													return;
												}
											}
										}
									}
								}
							}

						}
					}
				}
			}


			// ########## AUTO GRAB ##########
			if (b_AutoGrab) {
				// Make sure client is a Survivor Bot, with no item
				if (ClientIsFake) {

					// Make sure the bot has no grenade/special item
					if (!ClientGrenade || !ClientSpecial) {

						// Loop through the known item entities
						for (count = 0; count < i_ItemCount; count++) {
							if (i_ItemSpawnID[count] > -1 && IsValidEntity(i_ItemSpawnID[count])) {
								GetEdictClassname(i_ItemSpawnID[count], s_EdictClassName, sizeof(s_EdictClassName));

								// Loop through items to pick up
								for (i = 0; i < 6; i++) {												// Don't include first aid kit, obviously
									if (StrContains(s_EdictClassName, g_Pickups[i], false) > -1) {		// Item must be in the pick up list
										if (!ClientGrenade && i < 3 || !ClientSpecial && i > 2) {		// Only pick up item if slot is empty
											if (AllowedToTransfer(s_EdictClassName)) {				// Cvar allowed
												f_TargetPos = f_ItemSpawn_XYZ[count];					// Item spawn locations.
												GetClientEyePosition(client, f_ClientPos);				// Check Distance between item and bot
												new Float:f_Distance = GetVectorDistance(f_ClientPos, f_TargetPos);

												// Can they see it?
												if (f_Distance < f_DistGrab && IsVisibleTo(f_ClientPos, f_TargetPos)) {
													RemoveEdict(i_ItemSpawnID[count]);
													ResetItemArrays(count);
													Format(s_Temp, sizeof(s_Temp), g_Pickups[i]);

													GiveItem(client, s_Temp);
													Vocalize(client, s_Temp);
													return;
												}
											}
										}
									}
								}

							}else{
								ResetItemArrays(count);
							}
						}
					}
				}
			}

		}
	}
}



/*======================================================================================
############			A L L O W E D   T O   T R A N S F E R			################
======================================================================================*/
AllowedToTransfer(String:s_Item[32])
{
	new i_Item = -1;
	if (strlen(s_Item) < 1) return true;

	for (new i = 0; i < 7; i++) {
		if (StrContains(s_Item,g_Pickups[i]) > -1) i_Item = i;
	}

	switch (i_Item) {
		case -1: return true;
		case 0: if (!b_AllowMol) return false;
		case 1: if (!b_AllowPip) return false;
		case 2: if (!b_AllowVom) return false;
		case 3: if (!b_AllowExp) return false;
		case 4: if (!b_AllowInc) return false;
		case 5: if (!b_AllowDef) return false;
		case 6: if (!b_AllowFir) return false;
	}

	return true;
}



/*======================================================================================
################		M A K E  /  K I L L   A U T O   T I M E R		################
======================================================================================*/
MakeAutoTimer()
{
	if (h_AutoTimer != INVALID_HANDLE) {
		KillTimer(h_AutoTimer);
		h_AutoTimer = INVALID_HANDLE;
	}
	h_AutoTimer = CreateTimer(1.0, tmrAutoGiveGrab);
}

KillAutoTimer()
{
	if (h_AutoTimer != INVALID_HANDLE) {
		KillTimer(h_AutoTimer);
		h_AutoTimer = INVALID_HANDLE;
	}
}



/*======================================================================================
############			G E T   G R E N A D E   S P A W N S				################
======================================================================================*/
GetItemSpawns()
{
	// Search for dynamic weapon spawns,
	decl String:s_EdictClassName[32];
	new count = 0;

	// Loop through all game entities
	new i_EntCount = GetEntityCount();
	for (new i = 0; i < i_EntCount; i++) {

		if (IsValidEntity(i) && IsValidEdict(i)) {
			GetEdictClassname(i, s_EdictClassName, sizeof(s_EdictClassName));

			// Check if the entity is a valid pickup item
			for (new p = 0; p < sizeof(g_Pickups); p++) {
				if (StrContains(s_EdictClassName, g_Pickups[p], false) > -1) {
					// record their position
					new Float:f_Location[3];
					GetEntPropVector(i, Prop_Send, "m_vecOrigin", f_Location);
					i_ItemSpawnID[count] = i;
					f_ItemSpawn_XYZ[count] = f_Location;
					count++;
					if (count == ITEMZ) {
						i_ItemCount = count;
						return; // More than ITEMZ are spawned, we only account for the first ITEMZ
					}
				}
			}
		}

	}
	i_ItemCount = count;
}



/*======================================================================================
###############			R E S E T   G R E N A D E   S P A W N S			################
======================================================================================*/
ResetItemArrays(i)
{
	i_ItemSpawnID[i] = -1;
	f_ItemSpawn_XYZ[i] = Float:{0.0,0.0,0.0};
}



/*======================================================================================
###############				G I V E   G R E N A D E 					################
======================================================================================*/
GiveItem(client, String:s_Class[32])
{
	new i_Ent;
	i_Ent = CreateEntityByName(s_Class);
	DispatchSpawn(i_Ent);
	EquipPlayerWeapon(client, i_Ent);
}



/*======================================================================================
###############				R E M O V E   G R E N A D E					################
======================================================================================*/
// Helper method to strip a given player "client"'s slot
// (can't remember where this is from)
StripWeapon(client, i_Slot)
{
	new ent = GetPlayerWeaponSlot(client, i_Slot);

	if (ent != -1) {
		RemovePlayerItem(client, ent);
		RemoveEdict(ent);
	}
}



/*======================================================================================
###############					V O C A L I Z E 						################
======================================================================================*/
Vocalize(i_Client, String:s_Class[32])
{
	// We don't need to vocalize vomitjars, defibs, explosive ammo or incendiary ammo.
	if (StrContains(s_Class,"vomit", false) > 0) {
		return;
	}else if (StrContains(s_Class,"defib", false) > 0) {
		return;
	}else if (StrContains(s_Class,"explosive", false) > 0) {
		return;
	}else if (StrContains(s_Class,"incendiary", false) > 0) {
		return;
	}

	// Declare variables
	decl String:s_Model[128];
	decl String:s_Scene[64];
	new i_Type, i_Rand, i_Min, i_Max;

	// Get survivor model
	GetEntPropString(i_Client, Prop_Data, "m_ModelName", s_Model, 128);
	if (StrContains(s_Model,"coach", false) > 0) {Format(s_Model,32,"coach");i_Type = 1;}else
	if (StrContains(s_Model,"gambler", false) > 0) {Format(s_Model,32,"gambler");i_Type = 2;}else
	if (StrContains(s_Model,"mechanic", false) > 0) {Format(s_Model,32,"mechanic");i_Type = 3;}else
	if (StrContains(s_Model,"producer", false) > 0) {Format(s_Model,32,"producer");i_Type = 4;}else{return;}

	// Molotov
	if (StrContains(s_Class,"pipe", false) > 0) {
		switch (i_Type) {
			case 1: i_Max = 2;	// Coach
			case 2: i_Max = 1;	// Nick
			case 3: i_Max = 2;	// Ellis
			case 4: i_Max = 1;	// Rochelle
		}
	// Pipe Bomb
	}else if (StrContains(s_Class,"molotov", false) > 0) {
		switch (i_Type) {
			case 1: {i_Min = 3; i_Max = 4;}
			case 2: {i_Min = 2; i_Max = 3;}
			case 3: {i_Min = 3; i_Max = 10;}
			case 4: {i_Min = 2; i_Max = 5;}
		}
	}else{
		return;
	}

	// Random number
	i_Rand = GetRandomInt(i_Min, i_Max);

	// Select random vocalize
	switch (i_Type)
	{
		case 1: Format(s_Class, sizeof(s_Class),"%s", g_Coach[i_Rand]);
		case 2: Format(s_Class, sizeof(s_Class),"%s", g_Nick[i_Rand]);
		case 3: Format(s_Class, sizeof(s_Class),"%s", g_Ellis[i_Rand]);
		case 4: Format(s_Class, sizeof(s_Class),"%s", g_Rochelle[i_Rand]);
	}

	// Create scene location and call
	Format(s_Scene,64,"scenes/%s/%s.vcd", s_Model, s_Class);
	VocalizeScene(i_Client, s_Scene);
}



// Taken from:
// [Tech Demo] L4D2 Vocalize ANYTHING
// http://forums.alliedmods.net/showthread.php?t=122270
// author = "AtomicStryker"
/*======================================================================================
###############				V O C A L I Z E   S C E N E					################
======================================================================================*/
VocalizeScene(client, String:scenefile[64])
{
	if (!FileExists(scenefile))
	{
		LogError("Specified Scenefile: %s does not exist, aborting", scenefile);
		return;
	}

	new tempent = CreateEntityByName("instanced_scripted_scene");
	DispatchKeyValue(tempent, "SceneFile", scenefile);
	DispatchSpawn(tempent);
	SetEntPropEnt(tempent, Prop_Data, "m_hOwner", client);
	ActivateEntity(tempent);
	AcceptEntityInput(tempent, "Start", client, client);

	// Was crashing on Sourcemod 1.4.0
	//HookSingleEntityOutput(tempent, "OnCompletion", EntityOutput:OnSceneCompletion, true);
}

//public OnSceneCompletion(const String:s_Output[], i_Caller, i_Activator, Float:f_Delay)
//{
//	RemoveEdict(i_Caller);
//}



/*======================================================================================
###############					T R A C E   R A Y						################
======================================================================================*/
// Taken from:
// plugin = "L4D_Splash_Damage"
// author = "AtomicStryker"
static bool:IsVisibleTo(Float:position[3], Float:targetposition[3])
{
	decl Float:vAngles[3], Float:vLookAt[3];
	
	MakeVectorFromPoints(position, targetposition, vLookAt); // compute vector from start to target
	GetVectorAngles(vLookAt, vAngles); // get angles from vector for trace
	
	// execute Trace
	new Handle:trace = TR_TraceRayFilterEx(position, vAngles, MASK_SHOT, RayType_Infinite, _TraceFilter);
	
	new bool:isVisible = false;
	if (TR_DidHit(trace))
	{
		decl Float:vStart[3];
		TR_GetEndPosition(vStart, trace); // retrieve our trace endpoint
		
		if ((GetVectorDistance(position, vStart, false) + TRACE_TOLERANCE) >= GetVectorDistance(position, targetposition))
		{
			isVisible = true; // if trace ray lenght plus tolerance equal or bigger absolute distance, you hit the target
		}
	}
	else
	{
		LogError("Tracer Bug: Player-Zombie Trace did not hit anything, WTF");
		isVisible = true;
	}
	CloseHandle(trace);

	return isVisible;
}

public bool:_TraceFilter(entity, contentsMask)
{
	if (!entity || !IsValidEntity(entity)) // dont let WORLD, or invalid entities be hit
		return false;

	/*
	decl String:class[128];
	GetEdictClassname(entity, class, sizeof(class)); // also not survivors
	if (StrContains(class, "survivor", .caseSensitive = false) > -1)
	{
		return false;
	}
	*/

	return true;
}