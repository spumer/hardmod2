#define PLUGIN_VERSION		"1.0-ta"

/*=======================================================================================
	Plugin Info:

*	Name	:	[L4D2] F-18 Airstrike
*	Author	:	SilverShot
*	Descrp	:	Creates F-18 fly bys which shoot missiles to where they were triggered from.
*	Link	:	http://forums.alliedmods.net/showthread.php?t=181517

========================================================================================
	Change Log:

1.0-ta (15-Jun-2012)
	- Initial release.

======================================================================================*/

#pragma semicolon 			1

#include <l4d2_airstrike>
#include <sdktools>

static	bool:g_bLoaded;



// ====================================================================================================
//					PLUGIN INFO / START / END
// ====================================================================================================
public Plugin:myinfo =
{
	name = "[L4D2] F-18 Airstrike - Tank",
	author = "SilverShot",
	description = "Creates F-18 fly bys which shoot missiles to where they were triggered from.",
	version = PLUGIN_VERSION,
	url = "http://forums.alliedmods.net/showthread.php?t=181517"
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

public OnAllPluginsLoaded()
{
	if( LibraryExists("l4d2_airstrike") == false )
	{
		SetFailState("F-19 Airstrike 'l4d2_airstrike.core.smx' plugin not loaded.");
	}
}

public OnPluginStart()
{
	CreateConVar("l4d2_strike_tank",	PLUGIN_VERSION,			"F-18 Airstrike Tank plugin version",	FCVAR_PLUGIN|FCVAR_NOTIFY|FCVAR_DONTRECORD);
}

public F18_OnPluginState(pluginstate)
{
	static mystate;

	if( pluginstate == 1 && mystate == 0 )
	{
		mystate = 1;
		g_bLoaded = true;
		HookEvent("tank_spawn", Event_TankSpawn);
	}
	else if( pluginstate == 0 && mystate == 1 )
	{
		mystate = 0;
		g_bLoaded = false;
		UnhookEvent("tank_spawn", Event_TankSpawn);
	}
}

public F18_OnRoundState(roundstate)
{
	static mystate;

	if( roundstate == 1 && mystate == 0 )
	{
		mystate = 1;
		g_bLoaded = true;
	}
	else if( roundstate == 0 && mystate == 1 )
	{
		mystate = 0;
		g_bLoaded = false;
	}
}



// ====================================================================================================
//					CREATE AIRSTRIKE
// ====================================================================================================
public Event_TankSpawn(Handle:event, const String:name[], bool:dontBroadcast)
{
	if( g_bLoaded == true )
	{
		new client = GetClientOfUserId(GetEventInt(event, "userid"));
		if( client )
		{
			decl Float:vPos[3], Float:vAng[3];
			GetClientAbsOrigin(client, vPos);
			GetClientEyeAngles(client, vAng);
			F18_ShowAirstrike(vPos[0], vPos[1], vPos[2], vAng[1]);
		}
	}
}