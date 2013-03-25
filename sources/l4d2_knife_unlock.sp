#define PLUGIN_VERSION 		"1.0"

/*======================================================================================
	Plugin Info:

*	Name	:	[L4D2] Knife Unlock
*	Author	:	SilverShot, Dr!fter
*	Descrp	:	Unlocks the Knife melee weapon. No addons, no anim glitches and functional give knife command.
*	Link	:	http://forums.alliedmods.net/showthread.php?t=185258

========================================================================================
	Change Log:

1.0 (15-May-2012)
	- Initial release.

========================================================================================

	This plugin was made using source code from the following plugins.
	If I have used your code and not credited you, please let me know.

*	Thanks to "Dr!fter" for "[EXTENSION] MemPatch" and converting this plugin to use sourcemod based functions.
	http://forums.alliedmods.net/showthread.php?t=172187

======================================================================================*/

#pragma semicolon 1

#include <sourcemod>

public Plugin:myinfo =
{
	name = "[L4D2] Knife Unlock",
	author = "SilverShot, Dr!fter",
	description = "Unlocks the Knife melee weapon. No addons, no anim glitches and functional give knife command.",
	version = PLUGIN_VERSION,
	url = "http://forums.alliedmods.net/showthread.php?t=185258"
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
	CreateConVar("l4d2_knife_unlock_version", PLUGIN_VERSION, "Knife Unlock version.", FCVAR_PLUGIN|FCVAR_NOTIFY|FCVAR_REPLICATED|FCVAR_DONTRECORD);

	new Handle:hGameConfg = LoadGameConfigFile("l4d2_knife_unlock");
	new Address:patchAddr;

	if( hGameConfg )
	{
		patchAddr = GameConfGetAddress(hGameConfg, "KnifePatch");
	}

	if( patchAddr )
	{
		if( LoadFromAddress(patchAddr, NumberType_Int8) == 0x6B && LoadFromAddress(patchAddr + Address:4, NumberType_Int8) == 0x65 )
		{
			StoreToAddress(patchAddr, 0x4B, NumberType_Int8); // K
			StoreToAddress(patchAddr + Address:4, 0x61, NumberType_Int8); // a
		}
	}
	CloseHandle(hGameConfg);
}