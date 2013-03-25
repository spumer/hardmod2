#define PLUGIN_VERSION 		"1.2"

/*=======================================================================================
	Plugin Info:

*	Name	:	[L4D2] Healing Gnome
*	Author	:	SilverShot
*	Descrp	:	Heals players with temporary or main health when they hold the Gnome.
*	Link	:	http://forums.alliedmods.net/showthread.php?t=179267

========================================================================================
	Change Log:

1.2 (10-May-2012)
	- Added cvar "l4d2_gnome_modes_off" to control which game modes the plugin works in.
	- Added cvar "l4d2_gnome_modes_tog" same as above.
	- Renamed command "sm_gnomewipe" to "sm_gnomekill", due to command name conflict.
	- Changed cvar "l4d2_gnome_safe", 1=Spawn in saferoom, 2=Equip to random player, 3=Only on first chapter.

1.1 (01-Mar-2012)
	- Added command sm_gnomeglow to display the gnome positions.
	- Added command sm_gnometele to teleport to gnome positions.

1.0 (28-Feb-2012)
	- Initial release.

========================================================================================
	Thanks:

	This plugin was made using source code from the following plugins.
	If I have used your code and not credited you, please let me know.

*	"Zuko & McFlurry" for "[L4D2] Weapon/Zombie Spawner" - Modified SetTeleportEndPoint function.
	http://forums.alliedmods.net/showthread.php?t=109659

======================================================================================*/

#pragma semicolon 			1

#include <sourcemod>
#include <sdktools>

#define CVAR_FLAGS			FCVAR_PLUGIN|FCVAR_NOTIFY
#define CHAT_TAG			"\x04[\x05Gnome\x04] \x01"
#define CONFIG_SPAWNS		"data/l4d2_gnome.cfg"
#define MAX_GNOMES			32

#define MODEL_GNOME			"models/props_junk/gnome.mdl"


static	Handle:g_hCvarAllow, Handle:g_hCvarHeal, Handle:g_hCvarRandom, Handle:g_hCvarSafe, Handle:g_hCvarTemp, Handle:g_hCvarModes,
		Handle:g_hCvarModesOff, Handle:g_hCvarModesTog, bool:g_bCvarAllow, g_iCvarHeal, g_iCvarRandom, g_iCvarSafe, g_iCvarTemp,
		Handle:g_hCvarDecayRate, Handle:g_hCvarGnomeRate, Handle:g_hCvarMPGameMode, Float:g_fCvarDecayRate, Float:g_fCvarGnomeRate,
		Handle:g_hTimerHeal, Handle:g_hMenuAng, Handle:g_hMenuPos, bool:g_bLoaded, g_iPlayerSpawn, g_iRoundStart,
		g_iGnomeCount, g_iGnomes[MAX_GNOMES][2], g_iGnome[MAXPLAYERS+1], Float:g_fHealTime[MAXPLAYERS+1];



// ====================================================================================================
//					PLUGIN INFO / START / END
// ====================================================================================================
public Plugin:myinfo =
{
	name = "[L4D2] Healing Gnome",
	author = "SilverShot",
	description = "Heals players with temporary or main health when they hold the Gnome.",
	version = PLUGIN_VERSION,
	url = "http://forums.alliedmods.net/showthread.php?t=179267"
}

public APLRes:AskPluginLoad2(Handle:myself, bool:late, String:error[], err_max)
{
	decl String:sGameName[12];
	GetGameFolderName(sGameName, sizeof(sGameName));
	if( strcmp(sGameName, "left4dead2", false) )
	{
		strcopy(error, err_max, "Plugin only supports Left 4 Dead 2.");
		return APLRes_SilentFailure;
	}
	return APLRes_Success;
}

public OnPluginStart()
{
	g_hCvarAllow =		CreateConVar(	"l4d2_gnome_allow",		"1",			"0=Plugin off, 1=Plugin on.", CVAR_FLAGS );
	g_hCvarHeal =		CreateConVar(	"l4d2_gnome_heal",		"1",			"0=Off, 1=Heal players holding the gnome.", CVAR_FLAGS );
	g_hCvarModes =		CreateConVar(	"l4d2_gnome_modes",		"",				"Turn on the plugin in these game modes, separate by commas (no spaces). (Empty = all).", CVAR_FLAGS );
	g_hCvarModesOff =	CreateConVar(	"l4d2_gnome_modes_off",	"",				"Turn off the plugin in these game modes, separate by commas (no spaces). (Empty = none).", CVAR_FLAGS );
	g_hCvarModesTog =	CreateConVar(	"l4d2_gnome_modes_tog",	"0",			"Turn on the plugin in these game modes. 0=All, 1=Coop, 2=Survival, 4=Versus, 8=Scavenge. Add numbers together.", CVAR_FLAGS );
	g_hCvarRandom =		CreateConVar(	"l4d2_gnome_random",	"-1",			"-1=All, 0=None. Otherwise randomly select this many gnomes to spawn from the maps confg.", CVAR_FLAGS );
	g_hCvarSafe =		CreateConVar(	"l4d2_gnome_safe",		"2",			"On round start spawn the gnome: 0=Off, 1=In the saferoom, 2=Equip to random player.", CVAR_FLAGS );
	g_hCvarTemp =		CreateConVar(	"l4d2_gnome_temp",		"-1",			"-1=Add temporary health, 0=Add to main health. Values between 1 and 100 creates a chance to give main health, else temp health.", CVAR_FLAGS );
	CreateConVar(						"l4d2_gnome_version",	PLUGIN_VERSION, "Healing Gnome plugin version.", CVAR_FLAGS|FCVAR_REPLICATED|FCVAR_DONTRECORD);
	AutoExecConfig(true,				"l4d2_gnome");

	RegAdminCmd("sm_gnome",			CmdGnomeTemp,		ADMFLAG_ROOT, 	"Spawns a temporary gnome at your crosshair.");
	RegAdminCmd("sm_gnomesave",		CmdGnomeSave,		ADMFLAG_ROOT, 	"Spawns a gnome at your crosshair and saves to config.");
	RegAdminCmd("sm_gnomedel",		CmdGnomeDelete,		ADMFLAG_ROOT, 	"Removes the gnome you are pointing at and deletes from the config if saved.");
	RegAdminCmd("sm_gnomekill",		CmdGnomeWipe,		ADMFLAG_ROOT, 	"Removes all gnomes from the current map and deletes them from the config.");
	RegAdminCmd("sm_gnomeglow",		CmdGnomeGlow,		ADMFLAG_ROOT, 	"Toggle to enable glow on all gnomes to see where they are placed.");
	RegAdminCmd("sm_gnomelist",		CmdGnomeList,		ADMFLAG_ROOT, 	"Display a list gnome positions and the total number of.");
	RegAdminCmd("sm_gnometele",		CmdGnomeTele,		ADMFLAG_ROOT, 	"Teleport to a gnome (Usage: sm_gnometele <index: 1 to MAX_GNOMES>).");
	RegAdminCmd("sm_gnomeang",		CmdGnomeAng,		ADMFLAG_ROOT, 	"Displays a menu to adjust the gnome angles your crosshair is over.");
	RegAdminCmd("sm_gnomepos",		CmdGnomePos,		ADMFLAG_ROOT, 	"Displays a menu to adjust the gnome origin your crosshair is over.");

	g_hCvarDecayRate = FindConVar("pain_pills_decay_rate");
	g_hCvarGnomeRate = FindConVar("sv_healing_gnome_replenish_rate");
	g_hCvarMPGameMode = FindConVar("mp_gamemode");
	HookConVarChange(g_hCvarMPGameMode,		ConVarChanged_Allow);
	HookConVarChange(g_hCvarAllow,			ConVarChanged_Allow);
	HookConVarChange(g_hCvarModes,			ConVarChanged_Allow);
	HookConVarChange(g_hCvarModesOff,		ConVarChanged_Allow);
	HookConVarChange(g_hCvarModesTog,		ConVarChanged_Allow);
	HookConVarChange(g_hCvarHeal,			ConVarChanged_Cvars);
	HookConVarChange(g_hCvarRandom,			ConVarChanged_Cvars);
	HookConVarChange(g_hCvarSafe,			ConVarChanged_Cvars);
	HookConVarChange(g_hCvarTemp,			ConVarChanged_Cvars);
	HookConVarChange(g_hCvarDecayRate,		ConVarChanged_Cvars);
	HookConVarChange(g_hCvarGnomeRate,		ConVarChanged_Cvars);
}

public OnPluginEnd()
{
	ResetPlugin();
}

public OnMapStart()
{
	PrecacheModel(MODEL_GNOME, true);
}

public OnMapEnd()
{
	ResetPlugin(false);
}



// ====================================================================================================
//					CVARS
// ====================================================================================================
public OnConfigsExecuted()
	IsAllowed();

public ConVarChanged_Cvars(Handle:convar, const String:oldValue[], const String:newValue[])
	GetCvars();

public ConVarChanged_Allow(Handle:convar, const String:oldValue[], const String:newValue[])
	IsAllowed();

GetCvars()
{
	g_iCvarHeal = GetConVarInt(g_hCvarHeal);
	g_iCvarRandom = GetConVarInt(g_hCvarRandom);
	g_iCvarSafe = GetConVarInt(g_hCvarSafe);
	g_iCvarTemp = GetConVarInt(g_hCvarTemp);
	g_fCvarDecayRate = GetConVarFloat(g_hCvarDecayRate);
	g_fCvarGnomeRate = GetConVarFloat(g_hCvarGnomeRate);
}

IsAllowed()
{
	new bool:bCvarAllow = GetConVarBool(g_hCvarAllow);
	new bool:bAllowMode = IsAllowedGameMode();
	GetCvars();

	if( g_bCvarAllow == false && bCvarAllow == true && bAllowMode == true )
	{
		LoadGnomes();
		g_bCvarAllow = true;
		HookEvent("player_spawn",		Event_PlayerSpawn,	EventHookMode_PostNoCopy);
		HookEvent("round_start",		Event_RoundStart,	EventHookMode_PostNoCopy);
		HookEvent("round_end",			Event_RoundEnd,		EventHookMode_PostNoCopy);
		HookEvent("item_pickup",		Event_ItemPickup);
	}

	else if( g_bCvarAllow == true && (bCvarAllow == false || bAllowMode == false) )
	{
		ResetPlugin();
		g_bCvarAllow = false;
		UnhookEvent("player_spawn",		Event_PlayerSpawn,	EventHookMode_PostNoCopy);
		UnhookEvent("round_start",		Event_RoundStart,	EventHookMode_PostNoCopy);
		UnhookEvent("round_end",		Event_RoundEnd,		EventHookMode_PostNoCopy);
		UnhookEvent("item_pickup",		Event_ItemPickup);
	}
}

static g_iCurrentMode;

bool:IsAllowedGameMode()
{
	if( g_hCvarMPGameMode == INVALID_HANDLE )
		return false;

	new iCvarModesTog = GetConVarInt(g_hCvarModesTog);
	if( iCvarModesTog != 0 )
	{
		g_iCurrentMode = 0;

		new entity = CreateEntityByName("info_gamemode");
		DispatchSpawn(entity);
		HookSingleEntityOutput(entity, "OnCoop", OnGamemode, true);
		HookSingleEntityOutput(entity, "OnSurvival", OnGamemode, true);
		HookSingleEntityOutput(entity, "OnVersus", OnGamemode, true);
		HookSingleEntityOutput(entity, "OnScavenge", OnGamemode, true);
		AcceptEntityInput(entity, "PostSpawnActivate");
		AcceptEntityInput(entity, "Kill");

		if( g_iCurrentMode == 0 )
			return false;

		if( !(iCvarModesTog & g_iCurrentMode) )
			return false;
	}

	decl String:sGameModes[64], String:sGameMode[64];
	GetConVarString(g_hCvarMPGameMode, sGameMode, sizeof(sGameMode));
	Format(sGameMode, sizeof(sGameMode), ",%s,", sGameMode);

	GetConVarString(g_hCvarModes, sGameModes, sizeof(sGameModes));
	if( strcmp(sGameModes, "") )
	{
		Format(sGameModes, sizeof(sGameModes), ",%s,", sGameModes);
		if( StrContains(sGameModes, sGameMode, false) == -1 )
			return false;
	}

	GetConVarString(g_hCvarModesOff, sGameModes, sizeof(sGameModes));
	if( strcmp(sGameModes, "") )
	{
		Format(sGameModes, sizeof(sGameModes), ",%s,", sGameModes);
		if( StrContains(sGameModes, sGameMode, false) != -1 )
			return false;
	}

	return true;
}

public OnGamemode(const String:output[], caller, activator, Float:delay)
{
	if( strcmp(output, "OnCoop") == 0 )
		g_iCurrentMode = 1;
	else if( strcmp(output, "OnSurvival") == 0 )
		g_iCurrentMode = 2;
	else if( strcmp(output, "OnVersus") == 0 )
		g_iCurrentMode = 4;
	else if( strcmp(output, "OnScavenge") == 0 )
		g_iCurrentMode = 8;
}


// ====================================================================================================
//					EVENTS
// ====================================================================================================
public Event_RoundEnd(Handle:event, const String:name[], bool:dontBroadcast)
{
	ResetPlugin(false);
}

public Event_RoundStart(Handle:event, const String:name[], bool:dontBroadcast)
{
	if( g_iPlayerSpawn == 1 && g_iRoundStart == 0 )
		CreateTimer(1.0, tmrStart, _, TIMER_FLAG_NO_MAPCHANGE);
	g_iRoundStart = 1;
}

public Event_PlayerSpawn(Handle:event, const String:name[], bool:dontBroadcast)
{
	if( g_iPlayerSpawn == 0 && g_iRoundStart == 1 )
		CreateTimer(1.0, tmrStart, _, TIMER_FLAG_NO_MAPCHANGE);
	g_iPlayerSpawn = 1;
}

public Action:tmrStart(Handle:timer)
{
	ResetPlugin();
	LoadGnomes();

	if( g_iCvarSafe == 1 )
	{
		new client;
		for( new i = 1; i <= MaxClients; i++ )
		{
			if( IsClientInGame(i) && GetClientTeam(i) == 2 && IsPlayerAlive(i) )
			{
				client = i;
				break;
			}
		}

		if( client )
		{
			decl Float:vPos[3], Float:vAng[3];
			GetClientAbsOrigin(client, vPos);
			GetClientAbsAngles(client, vAng);
			vPos[2] += 25.0;
			CreateGnome(vPos, vAng);
		}
	}
	else if( g_iCvarSafe == 2 )
	{
		new iClients[MAXPLAYERS+1], count;

		for( new i = 1; i <= MaxClients; i++ )
			if( IsClientInGame(i) && GetClientTeam(i) == 2 && IsPlayerAlive(i) )
				iClients[count++] = i;

		new client = GetRandomInt(0, count-1);
		client = iClients[client];

		if( client )
		{
			new entity = GivePlayerItem(client, "weapon_gnome");
			if( entity != -1 )
				EquipPlayerWeapon(client, entity);
		}
	}
}

public Event_ItemPickup(Handle:event, const String:name[], bool:dontBroadcast)
{
	if( g_iCvarHeal )
	{
		decl String:sTemp[8];
		GetEventString(event, "item", sTemp, sizeof(sTemp));
		if( strcmp(sTemp, "gnome") == 0 )
		{
			new client = GetClientOfUserId(GetEventInt(event, "userid"));
			g_iGnome[client] = GetEntPropEnt(client, Prop_Send, "m_hActiveWeapon");

			if( g_hTimerHeal == INVALID_HANDLE )
				CreateTimer(0.1, tmrHeal, _, TIMER_REPEAT);
		}
	}
}

public Action:tmrHeal(Handle:timer)
{
	new entity, bool:healed;

	if( g_iCvarHeal )
	{
		for( new i = 1; i <= MaxClients; i++ )
		{
			entity = g_iGnome[i];
			if( entity )
			{
				if( IsClientInGame(i) && IsPlayerAlive(i) && entity == GetEntPropEnt(i, Prop_Send, "m_hActiveWeapon") )
				{
					HealClient(i);
					healed = true;
				}
				else
					g_iGnome[i] = 0;
			}
		}
	}

	if( healed == false )
	{
		g_hTimerHeal = INVALID_HANDLE;
		return Plugin_Stop;
	}

	return Plugin_Continue;
}

HealClient(client)
{
	new iHealth = GetClientHealth(client);
	new Float:fGameTime = GetGameTime();

	if( g_iCvarTemp == -1 || (g_iCvarTemp != 0 && GetRandomInt(1, 100) >= g_iCvarTemp) )
	{
		new Float:fHealthTime = GetEntPropFloat(client, Prop_Send, "m_healthBufferTime");
		new Float:fHealth = GetEntPropFloat(client, Prop_Send, "m_healthBuffer");
		fHealth -= (fGameTime - fHealthTime) * g_fCvarDecayRate;
		if( fHealth < 0.0 )
			fHealth = 0.0;

		new Float:fBuff = (0.1 * g_fCvarGnomeRate);

		if( fHealth + iHealth + fBuff > 100 )
			SetEntPropFloat(client, Prop_Send, "m_healthBuffer", 100.1 - float(iHealth));
		else
			SetEntPropFloat(client, Prop_Send, "m_healthBuffer", fHealth + fBuff);
		SetEntPropFloat(client, Prop_Send, "m_healthBufferTime", GetGameTime());
	}
	else
	{
		if( fGameTime - g_fHealTime[client] > 1.0 )
		{
			g_fHealTime[client] = fGameTime;

			if( iHealth < 100 )
			{
				new iBuff = RoundToFloor(g_fCvarGnomeRate);
				if( iBuff == 0 )
					iBuff++;

				if( iHealth > 100 )
					iHealth = 100;

				iHealth += iBuff;
				SetEntityHealth(client, iHealth);
			}
		}
	}
}



// ====================================================================================================
//					LOAD GNOMES
// ====================================================================================================
LoadGnomes()
{
	if( g_bLoaded || g_iCvarRandom == 0 ) return;
	g_bLoaded = true;

	decl String:sPath[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, sPath, sizeof(sPath), "%s", CONFIG_SPAWNS);
	if( !FileExists(sPath) )
		return;

	// Load config
	new Handle:hFile = CreateKeyValues("gnomes");
	if( !FileToKeyValues(hFile, sPath) )
	{
		CloseHandle(hFile);
		return;
	}

	// Check for current map in the config
	decl String:sMap[64];
	GetCurrentMap(sMap, 64);

	if( !KvJumpToKey(hFile, sMap) )
	{
		CloseHandle(hFile);
		return;
	}

	// Retrieve how many gnomes to display
	new iCount = KvGetNum(hFile, "num", 0);
	if( iCount == 0 )
	{
		CloseHandle(hFile);
		return;
	}

	// Spawn only a select few gnomes?
	new iIndexes[MAX_GNOMES+1];
	if( iCount > MAX_GNOMES )
		iCount = MAX_GNOMES;


	// Spawn saved gnomes or create random
	new iRandom = g_iCvarRandom;
	if( iRandom == -1 || iRandom > iCount)
		iRandom = iCount;
	if( iRandom != -1 )
	{
		for( new i = 1; i <= iCount; i++ )
			iIndexes[i] = i;

		SortIntegers(iIndexes, iCount+1, Sort_Random);
		iCount = iRandom;
	}

	// Get the gnome origins and spawn
	decl String:sTemp[10], Float:vPos[3], Float:vAng[3];
	new index;
	for( new i = 1; i <= iCount; i++ )
	{
		if( iRandom != -1 ) index = iIndexes[i];
		else index = i;

		IntToString(index, sTemp, sizeof(sTemp));

		if( KvJumpToKey(hFile, sTemp) )
		{
			KvGetVector(hFile, "angle", vAng);
			KvGetVector(hFile, "origin", vPos);

			if( vPos[0] == 0.0 && vPos[0] == 0.0 && vPos[0] == 0.0 ) // Should never happen.
				LogError("Error: 0,0,0 origin. Iteration=%d. Index=%d. Random=%d. Count=%d.", i, index, iRandom, iCount);
			else
				CreateGnome(vPos, vAng, index);
			KvGoBack(hFile);
		}
	}

	CloseHandle(hFile);
}



// ====================================================================================================
//					CREATE GNOME
// ====================================================================================================
CreateGnome(const Float:vOrigin[3], const Float:vAngles[3], index = 0)
{
	if( g_iGnomeCount >= MAX_GNOMES )
		return;

	new iGnomeIndex = -1;
	for( new i = 0; i < MAX_GNOMES; i++ )
	{
		if( g_iGnomes[i][0] == 0 )
		{
			iGnomeIndex = i;
			break;
		}
	}

	if( iGnomeIndex == -1 )
		return;

	new entity = CreateEntityByName("prop_physics");
	if( entity == -1 )
		ThrowError("Failed to create gnome model.");

	g_iGnomes[iGnomeIndex][0] = EntIndexToEntRef(entity);
	g_iGnomes[iGnomeIndex][1] = index;
	DispatchKeyValue(entity, "model", MODEL_GNOME);
	DispatchSpawn(entity);
	TeleportEntity(entity, vOrigin, vAngles, NULL_VECTOR);

	g_iGnomeCount++;
}



// ====================================================================================================
//					COMMANDS
// ====================================================================================================
//					sm_gnome
// ====================================================================================================
public Action:CmdGnomeTemp(client, args)
{
	if( !client )
	{
		ReplyToCommand(client, "[Gnome] Commands may only be used in-game on a dedicated server..");
		return Plugin_Handled;
	}
	else if( g_iGnomeCount >= MAX_GNOMES )
	{
		PrintToChat(client, "%sError: Cannot add anymore gnomes. Used: (\x05%d/%d\x01).", CHAT_TAG, g_iGnomeCount, MAX_GNOMES);
		return Plugin_Handled;
	}

	new Float:vPos[3], Float:vAng[3];
	if( !SetTeleportEndPoint(client, vPos, vAng) )
	{
		PrintToChat(client, "%sCannot place gnome, please try again.", CHAT_TAG);
		return Plugin_Handled;
	}

	CreateGnome(vPos, vAng);
	return Plugin_Handled;
}

// ====================================================================================================
//					sm_gnomesave
// ====================================================================================================
public Action:CmdGnomeSave(client, args)
{
	if( !client )
	{
		ReplyToCommand(client, "[Gnome] Commands may only be used in-game on a dedicated server..");
		return Plugin_Handled;
	}
	else if( g_iGnomeCount >= MAX_GNOMES )
	{
		PrintToChat(client, "%sError: Cannot add anymore gnomes. Used: (\x05%d/%d\x01).", CHAT_TAG, g_iGnomeCount, MAX_GNOMES);
		return Plugin_Handled;
	}

	decl String:sPath[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, sPath, sizeof(sPath), "%s", CONFIG_SPAWNS);
	if( !FileExists(sPath) )
	{
		new Handle:hCfg = OpenFile(sPath, "w");
		WriteFileLine(hCfg, "");
		CloseHandle(hCfg);
	}

	// Load config
	new Handle:hFile = CreateKeyValues("gnomes");
	if( !FileToKeyValues(hFile, sPath) )
	{
		PrintToChat(client, "%sError: Cannot read the gnome config, assuming empty file. (\x05%s\x01).", CHAT_TAG, sPath);
	}

	// Check for current map in the config
	decl String:sMap[64];
	GetCurrentMap(sMap, 64);
	if( !KvJumpToKey(hFile, sMap, true) )
	{
		PrintToChat(client, "%sError: Failed to add map to gnome spawn config.", CHAT_TAG);
		CloseHandle(hFile);
		return Plugin_Handled;
	}

	// Retrieve how many gnomes are saved
	new iCount = KvGetNum(hFile, "num", 0);
	if( iCount >= MAX_GNOMES )
	{
		PrintToChat(client, "%sError: Cannot add anymore gnomes. Used: (\x05%d/%d\x01).", CHAT_TAG, iCount, MAX_GNOMES);
		CloseHandle(hFile);
		return Plugin_Handled;
	}

	// Save count
	iCount++;
	KvSetNum(hFile, "num", iCount);

	decl String:sTemp[10];

	IntToString(iCount, sTemp, sizeof(sTemp));

	if( KvJumpToKey(hFile, sTemp, true) )
	{
		new Float:vPos[3], Float:vAng[3];
		// Set player position as gnome spawn location
		if( !SetTeleportEndPoint(client, vPos, vAng) )
		{
			PrintToChat(client, "%sCannot place gnome, please try again.", CHAT_TAG);
			CloseHandle(hFile);
			return Plugin_Handled;
		}

		// Save angle / origin
		KvSetVector(hFile, "angle", vAng);
		KvSetVector(hFile, "origin", vPos);

		CreateGnome(vPos, vAng, iCount);

		// Save cfg
		KvRewind(hFile);
		KeyValuesToFile(hFile, sPath);

		PrintToChat(client, "%s(\x05%d/%d\x01) - Saved at pos:[\x05%f %f %f\x01] ang:[\x05%f %f %f\x01]", CHAT_TAG, iCount, MAX_GNOMES, vPos[0], vPos[1], vPos[2], vAng[0], vAng[1], vAng[2]);
	}
	else
		PrintToChat(client, "%s(\x05%d/%d\x01) - Failed to save Gnome.", CHAT_TAG, iCount, MAX_GNOMES);

	CloseHandle(hFile);
	return Plugin_Handled;
}

// ====================================================================================================
//					sm_gnomedel
// ====================================================================================================
public Action:CmdGnomeDelete(client, args)
{
	if( !g_bCvarAllow )
	{
		ReplyToCommand(client, "[SM] Plugin turned off.");
		return Plugin_Handled;
	}

	if( !client )
	{
		ReplyToCommand(client, "[Gnome] Commands may only be used in-game on a dedicated server..");
		return Plugin_Handled;
	}

	new entity = GetClientAimTarget(client, false);
	if( entity == -1 ) return Plugin_Handled;
	entity = EntIndexToEntRef(entity);

	new cfgindex, index = -1;
	for( new i = 0; i < MAX_GNOMES; i++ )
	{
		if( g_iGnomes[i][0] == entity )
		{
			index = i;
			break;
		}
	}

	if( index == -1 )
		return Plugin_Handled;

	cfgindex = g_iGnomes[index][1];
	if( cfgindex == 0 )
	{
		RemoveGnome(index);
		return Plugin_Handled;
	}

	for( new i = 0; i < MAX_GNOMES; i++ )
	{
		if( g_iGnomes[i][1] > cfgindex )
			g_iGnomes[i][1]--;
	}

	g_iGnomeCount--;

	// Load config
	decl String:sPath[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, sPath, sizeof(sPath), "%s", CONFIG_SPAWNS);
	if( !FileExists(sPath) )
	{
		PrintToChat(client, "%sError: Cannot find the gnome config (\x05%s\x01).", CHAT_TAG, CONFIG_SPAWNS);
		return Plugin_Handled;
	}

	new Handle:hFile = CreateKeyValues("gnomes");
	if( !FileToKeyValues(hFile, sPath) )
	{
		PrintToChat(client, "%sError: Cannot load the gnome config (\x05%s\x01).", CHAT_TAG, sPath);
		CloseHandle(hFile);
		return Plugin_Handled;
	}

	// Check for current map in the config
	decl String:sMap[64];
	GetCurrentMap(sMap, 64);

	if( !KvJumpToKey(hFile, sMap) )
	{
		PrintToChat(client, "%sError: Current map not in the gnome config.", CHAT_TAG);
		CloseHandle(hFile);
		return Plugin_Handled;
	}

	// Retrieve how many gnomes
	new iCount = KvGetNum(hFile, "num", 0);
	if( iCount == 0 )
	{
		CloseHandle(hFile);
		return Plugin_Handled;
	}

	new bool:bMove;
	decl String:sTemp[16];

	// Move the other entries down
	for( new i = cfgindex; i <= iCount; i++ )
	{
		IntToString(i, sTemp, sizeof(sTemp));

		if( KvJumpToKey(hFile, sTemp) )
		{
			if( !bMove )
			{
				bMove = true;
				KvDeleteThis(hFile);
				RemoveGnome(index);
			}
			else
			{
				IntToString(i-1, sTemp, sizeof(sTemp));
				KvSetSectionName(hFile, sTemp);
			}
		}

		KvRewind(hFile);
		KvJumpToKey(hFile, sMap);
	}

	if( bMove )
	{
		iCount--;
		KvSetNum(hFile, "num", iCount);

		// Save to file
		KvRewind(hFile);
		KeyValuesToFile(hFile, sPath);

		PrintToChat(client, "%s(\x05%d/%d\x01) - Gnome removed from config.", CHAT_TAG, iCount, MAX_GNOMES);
	}
	else
		PrintToChat(client, "%s(\x05%d/%d\x01) - Failed to remove Gnome from config.", CHAT_TAG, iCount, MAX_GNOMES);

	CloseHandle(hFile);
	return Plugin_Handled;
}

// ====================================================================================================
//					sm_gnomewipe
// ====================================================================================================
public Action:CmdGnomeWipe(client, args)
{
	if( !client )
	{
		ReplyToCommand(client, "[Gnome] Commands may only be used in-game on a dedicated server..");
		return Plugin_Handled;
	}

	decl String:sPath[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, sPath, sizeof(sPath), "%s", CONFIG_SPAWNS);
	if( !FileExists(sPath) )
	{
		PrintToChat(client, "%sError: Cannot find the gnome config (\x05%s\x01).", CHAT_TAG, sPath);
		return Plugin_Handled;
	}

	// Load config
	new Handle:hFile = CreateKeyValues("gnomes");
	if( !FileToKeyValues(hFile, sPath) )
	{
		PrintToChat(client, "%sError: Cannot load the gnome config (\x05%s\x01).", CHAT_TAG, sPath);
		CloseHandle(hFile);
		return Plugin_Handled;
	}

	// Check for current map in the config
	decl String:sMap[64];
	GetCurrentMap(sMap, 64);

	if( !KvJumpToKey(hFile, sMap, false) )
	{
		PrintToChat(client, "%sError: Current map not in the gnome config.", CHAT_TAG);
		CloseHandle(hFile);
		return Plugin_Handled;
	}

	KvDeleteThis(hFile);
	ResetPlugin();

	// Save to file
	KvRewind(hFile);
	KeyValuesToFile(hFile, sPath);
	CloseHandle(hFile);

	PrintToChat(client, "%s(0/%d) - All gnomes removed from config, add with \x05sm_gnomesave\x01.", CHAT_TAG, MAX_GNOMES);
	return Plugin_Handled;
}

// ====================================================================================================
//					sm_gnomeglow
// ====================================================================================================
public Action:CmdGnomeGlow(client, args)
{
	static bool:glow;
	glow = !glow;
	PrintToChat(client, "%sGlow has been turned %s", CHAT_TAG, glow ? "on" : "off");

	VendorGlow(glow);
	return Plugin_Handled;
}

VendorGlow(glow)
{
	new ent;

	for( new i = 0; i < MAX_GNOMES; i++ )
	{
		ent = g_iGnomes[i][0];
		if( IsValidEntRef(ent) )
		{
			SetEntProp(ent, Prop_Send, "m_iGlowType", 3);
			SetEntProp(ent, Prop_Send, "m_glowColorOverride", 65535);
			SetEntProp(ent, Prop_Send, "m_nGlowRange", glow ? 0 : 50);
			ChangeEdictState(ent, FindSendPropOffs("prop_dynamic", "m_nGlowRange"));
		}
	}
}

// ====================================================================================================
//					sm_gnomelist
// ====================================================================================================
public Action:CmdGnomeList(client, args)
{
	decl Float:vPos[3];
	new count;
	for( new i = 0; i < MAX_GNOMES; i++ )
	{
		if( IsValidEntRef(g_iGnomes[i][0]) )
		{
			count++;
			GetEntPropVector(g_iGnomes[i][0], Prop_Data, "m_vecOrigin", vPos);
			PrintToChat(client, "%s%d) %f %f %f", CHAT_TAG, i+1, vPos[0], vPos[1], vPos[2]);
		}
	}
	PrintToChat(client, "%sTotal: %d.", CHAT_TAG, count);
	return Plugin_Handled;
}

// ====================================================================================================
//					sm_gnometele
// ====================================================================================================
public Action:CmdGnomeTele(client, args)
{
	if( args == 1 )
	{
		decl String:arg[16];
		GetCmdArg(1, arg, 16);
		new index = StringToInt(arg) - 1;
		if( index > -1 && index < MAX_GNOMES && IsValidEntRef(g_iGnomes[index][0]) )
		{
			decl Float:vPos[3];
			GetEntPropVector(g_iGnomes[index][0], Prop_Data, "m_vecOrigin", vPos);
			vPos[2] += 20.0;
			TeleportEntity(client, vPos, NULL_VECTOR, NULL_VECTOR);
			PrintToChat(client, "%sTeleported to %d.", CHAT_TAG, index + 1);
			return Plugin_Handled;
		}

		PrintToChat(client, "%sCould not find index for teleportation.", CHAT_TAG);
	}
	else
		PrintToChat(client, "%sUsage: sm_gnometele <index 1-%d>.", CHAT_TAG, MAX_GNOMES);
	return Plugin_Handled;
}

// ====================================================================================================
//					MENU ANGLE
// ====================================================================================================
public Action:CmdGnomeAng(client, args)
{
	ShowMenuAng(client);
	return Plugin_Handled;
}

ShowMenuAng(client)
{
	CreateMenus();
	DisplayMenu(g_hMenuAng, client, MENU_TIME_FOREVER);
}

public AngMenuHandler(Handle:menu, MenuAction:action, client, index)
{
	if( action == MenuAction_Select )
	{
		if( index == 6 )
			SaveData(client);
		else
			SetAngle(client, index);
		ShowMenuAng(client);
	}
}

SetAngle(client, index)
{
	new aim = GetClientAimTarget(client, false);
	if( aim != -1 )
	{
		new Float:vAng[3], entity;
		aim = EntIndexToEntRef(aim);

		for( new i = 0; i < MAX_GNOMES; i++ )
		{
			entity = g_iGnomes[i][0];

			if( entity == aim  )
			{
				GetEntPropVector(entity, Prop_Send, "m_angRotation", vAng);

				if( index == 0 ) vAng[0] += 5.0;
				else if( index == 1 ) vAng[1] += 5.0;
				else if( index == 2 ) vAng[2] += 5.0;
				else if( index == 3 ) vAng[0] -= 5.0;
				else if( index == 4 ) vAng[1] -= 5.0;
				else if( index == 5 ) vAng[2] -= 5.0;

				TeleportEntity(entity, NULL_VECTOR, vAng, NULL_VECTOR);

				PrintToChat(client, "%sNew angles: %f %f %f", CHAT_TAG, vAng[0], vAng[1], vAng[2]);
				break;
			}
		}
	}
}

// ====================================================================================================
//					MENU ORIGIN
// ====================================================================================================
public Action:CmdGnomePos(client, args)
{
	ShowMenuPos(client);
	return Plugin_Handled;
}

ShowMenuPos(client)
{
	CreateMenus();
	DisplayMenu(g_hMenuPos, client, MENU_TIME_FOREVER);
}

public PosMenuHandler(Handle:menu, MenuAction:action, client, index)
{
	if( action == MenuAction_Select )
	{
		if( index == 6 )
			SaveData(client);
		else
			SetOrigin(client, index);
		ShowMenuPos(client);
	}
}

SetOrigin(client, index)
{
	new aim = GetClientAimTarget(client, false);
	if( aim != -1 )
	{
		new Float:vPos[3], entity;
		aim = EntIndexToEntRef(aim);

		for( new i = 0; i < MAX_GNOMES; i++ )
		{
			entity = g_iGnomes[i][0];

			if( entity == aim  )
			{
				GetEntPropVector(entity, Prop_Send, "m_vecOrigin", vPos);

				if( index == 0 ) vPos[0] += 0.5;
				else if( index == 1 ) vPos[1] += 0.5;
				else if( index == 2 ) vPos[2] += 0.5;
				else if( index == 3 ) vPos[0] -= 0.5;
				else if( index == 4 ) vPos[1] -= 0.5;
				else if( index == 5 ) vPos[2] -= 0.5;

				TeleportEntity(entity, vPos, NULL_VECTOR, NULL_VECTOR);

				PrintToChat(client, "%sNew origin: %f %f %f", CHAT_TAG, vPos[0], vPos[1], vPos[2]);
				break;
			}
		}
	}
}

SaveData(client)
{
	new entity, index;
	new aim = GetClientAimTarget(client, false);
	if( aim == -1 )
		return;

	aim = EntIndexToEntRef(aim);

	for( new i = 0; i < MAX_GNOMES; i++ )
	{
		entity = g_iGnomes[i][0];

		if( entity == aim  )
		{
			index = g_iGnomes[i][1];
			break;
		}
	}

	if( index == 0 )
		return;

	// Load config
	decl String:sPath[PLATFORM_MAX_PATH];
	BuildPath(Path_SM, sPath, sizeof(sPath), "%s", CONFIG_SPAWNS);
	if( !FileExists(sPath) )
	{
		PrintToChat(client, "%sError: Cannot find the gnome config (\x05%s\x01).", CHAT_TAG, CONFIG_SPAWNS);
		return;
	}

	new Handle:hFile = CreateKeyValues("gnomes");
	if( !FileToKeyValues(hFile, sPath) )
	{
		PrintToChat(client, "%sError: Cannot load the gnome config (\x05%s\x01).", CHAT_TAG, sPath);
		CloseHandle(hFile);
		return;
	}

	// Check for current map in the config
	decl String:sMap[64];
	GetCurrentMap(sMap, 64);

	if( !KvJumpToKey(hFile, sMap) )
	{
		PrintToChat(client, "%sError: Current map not in the gnome config.", CHAT_TAG);
		CloseHandle(hFile);
		return;
	}

	decl Float:vAng[3], Float:vPos[3], String:sTemp[32];
	GetEntPropVector(entity, Prop_Send, "m_vecOrigin", vPos);
	GetEntPropVector(entity, Prop_Send, "m_angRotation", vAng);

	IntToString(index, sTemp, sizeof(sTemp));
	if( KvJumpToKey(hFile, sTemp) )
	{
		KvSetVector(hFile, "angle", vAng);
		KvSetVector(hFile, "origin", vPos);

		// Save cfg
		KvRewind(hFile);
		KeyValuesToFile(hFile, sPath);

		PrintToChat(client, "%sSaved origin and angles to the data config", CHAT_TAG);
	}
}

CreateMenus()
{
	if( g_hMenuAng == INVALID_HANDLE )
	{
		g_hMenuAng = CreateMenu(AngMenuHandler);
		AddMenuItem(g_hMenuAng, "", "X + 5.0");
		AddMenuItem(g_hMenuAng, "", "Y + 5.0");
		AddMenuItem(g_hMenuAng, "", "Z + 5.0");
		AddMenuItem(g_hMenuAng, "", "X - 5.0");
		AddMenuItem(g_hMenuAng, "", "Y - 5.0");
		AddMenuItem(g_hMenuAng, "", "Z - 5.0");
		AddMenuItem(g_hMenuAng, "", "SAVE");
		SetMenuTitle(g_hMenuAng, "Set Angle");
		SetMenuExitButton(g_hMenuAng, true);
	}

	if( g_hMenuPos == INVALID_HANDLE )
	{
		g_hMenuPos = CreateMenu(PosMenuHandler);
		AddMenuItem(g_hMenuPos, "", "X + 0.5");
		AddMenuItem(g_hMenuPos, "", "Y + 0.5");
		AddMenuItem(g_hMenuPos, "", "Z + 0.5");
		AddMenuItem(g_hMenuPos, "", "X - 0.5");
		AddMenuItem(g_hMenuPos, "", "Y - 0.5");
		AddMenuItem(g_hMenuPos, "", "Z - 0.5");
		AddMenuItem(g_hMenuPos, "", "SAVE");
		SetMenuTitle(g_hMenuPos, "Set Position");
		SetMenuExitButton(g_hMenuPos, true);
	}
}



// ====================================================================================================
//					STUFF
// ====================================================================================================
bool:IsValidEntRef(entity)
{
	if( entity && EntRefToEntIndex(entity) != INVALID_ENT_REFERENCE )
		return true;
	return false;
}

ResetPlugin(bool:all = true)
{
	g_bLoaded = false;
	g_iGnomeCount = 0;
	g_iRoundStart = 0;
	g_iPlayerSpawn = 0;

	for( new i = 1; i <= MAXPLAYERS; i++ )
		g_fHealTime[i] = 0.0;

	if( all )
		for( new i = 0; i < MAX_GNOMES; i++ )
			RemoveGnome(i);
}

RemoveGnome(index)
{
	new entity = g_iGnomes[index][0];
	g_iGnomes[index][0] = 0;

	if( IsValidEntRef(entity) )
		AcceptEntityInput(entity, "kill");
}



// ====================================================================================================
//					POSITION
// ====================================================================================================
MoveForward(const Float:vPos[3], const Float:vAng[3], Float:vReturn[3], Float:fDistance)
{
	fDistance *= -1.0;
	decl Float:vDir[3];
	GetAngleVectors(vAng, vDir, NULL_VECTOR, NULL_VECTOR);
	vReturn = vPos;
	vReturn[0] += vDir[0] * fDistance;
	vReturn[1] += vDir[1] * fDistance;
}

MoveSideway(const Float:vPos[3], const Float:vAng[3], Float:vReturn[3], Float:fDistance)
{
	fDistance *= -1.0;
	decl Float:vDir[3];
	GetAngleVectors(vAng, NULL_VECTOR, vDir, NULL_VECTOR);
	vReturn = vPos;
	vReturn[0] += vDir[0] * fDistance;
	vReturn[1] += vDir[1] * fDistance;
}

SetTeleportEndPoint(client, Float:vPos[3], Float:vAng[3])
{
	GetClientEyePosition(client, vPos);
	GetClientEyeAngles(client, vAng);

	new Handle:trace = TR_TraceRayFilterEx(vPos, vAng, MASK_SHOT, RayType_Infinite, _TraceFilter);

	if(TR_DidHit(trace))
	{
		decl Float:vNorm[3];
		TR_GetEndPosition(vPos, trace);
		TR_GetPlaneNormal(trace, vNorm);
		new Float:angle = vAng[1];
		GetVectorAngles(vNorm, vAng);

		vPos[2] += 25.0;

		if( vNorm[2] == 1.0 )
		{
			vAng[0] = 0.0;
			vAng[1] += angle;
			MoveSideway(vPos, vAng, vPos, -8.0);
		}
		else
		{
			vAng[0] = 0.0;
			vAng[1] += angle - 90.0;
			MoveForward(vPos, vAng, vPos, -10.0);
		}
	}
	else
	{
		CloseHandle(trace);
		return false;
	}
	CloseHandle(trace);
	return true;
}

public bool:_TraceFilter(entity, contentsMask)
{
	return entity > MaxClients || !entity;
}