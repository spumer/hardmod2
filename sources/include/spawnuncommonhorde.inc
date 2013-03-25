/*
 * ============================================================================
 *
 *  Buy System
 *
 *  File:          spawnuncommonhorde.inc
 *  Type:          Module
 *  Description:   Handles horde spawning of uncommons, based of Atomic 
 *                  Strykers spawn uncommon infected
 *					(http://forums.alliedmods.net/showthread.php?t=109623).
 *
 *  Copyright (C) 2010  Mr. Zero
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ============================================================================
 */

#pragma semicolon 1
#include <sourcemod>
#include <sdktools>
#include <sdkhooks>

static HordeType;
static RemainingZombiesToSpawn;

#define COMMONTYPE_RANDOM 0
#define COMMONTYPE_ROIT 1
#define COMMONTYPE_CEDA 2
#define COMMONTYPE_CLOWN 3
#define COMMONTYPE_MUD 4
#define COMMONTYPE_ROADCREW 5
#define COMMONTYPE_JIMMY 6
#define COMMONTYPE_FALLEN 7

//static const COMMONTYPE_RANDOM 		= 0;
//static const COMMONTYPE_RIOT 		= 1;
//static const COMMONTYPE_CEDA 		= 2;
//static const COMMONTYPE_CLOWN 		= 3;
//static const COMMONTYPE_MUD 		= 4;
//static const COMMONTYPE_ROADCREW 	= 5;
//static const COMMONTYPE_JIMMY 		= 6;
//static const COMMONTYPE_FALLEN 		= 7;

static const String:COMMONMODEL_RIOT[] 		= "models/infected/common_male_riot.mdl";
static const String:COMMONMODEL_CEDA[] 		= "models/infected/common_male_ceda.mdl";
static const String:COMMONMODEL_CLOWN[] 	= "models/infected/common_male_clown.mdl";
static const String:COMMONMODEL_MUD[] 		= "models/infected/common_male_mud.mdl";
static const String:COMMONMODEL_ROADCREW[] 	= "models/infected/common_male_roadcrew.mdl";
static const String:COMMONMODEL_JIMMY[] 	= "models/infected/common_male_jimmy.mdl";
static const String:COMMONMODEL_FALLEN[] 	= "models/infected/common_male_fallen_survivor.mdl";

public SUH_OnPluginStart()
{
	PrecacheModel(COMMONMODEL_RIOT, true);
	PrecacheModel(COMMONMODEL_CEDA, true);
	PrecacheModel(COMMONMODEL_CLOWN, true);
	PrecacheModel(COMMONMODEL_MUD, true);
	PrecacheModel(COMMONMODEL_ROADCREW, true);
	PrecacheModel(COMMONMODEL_JIMMY, true);
	PrecacheModel(COMMONMODEL_FALLEN, true);
	PrecacheModel("models/infected/common_male_riot.mdl", true);
	PrecacheModel("models/infected/common_male_ceda.mdl", true);
	PrecacheModel("models/infected/common_male_clown.mdl", true);
	PrecacheModel("models/infected/common_male_mud.mdl", true);
	PrecacheModel("models/infected/common_male_roadcrew.mdl", true);
	PrecacheModel("models/infected/common_male_jimmy.mdl", true);
	PrecacheModel("models/infected/common_male_fallen_survivor.mdl", true);
}

public SUH_OnMapStart()
{
	PrecacheModel(COMMONMODEL_RIOT, true);
	PrecacheModel(COMMONMODEL_CEDA, true);
	PrecacheModel(COMMONMODEL_CLOWN, true);
	PrecacheModel(COMMONMODEL_MUD, true);
	PrecacheModel(COMMONMODEL_ROADCREW, true);
	PrecacheModel(COMMONMODEL_JIMMY, true);
	PrecacheModel(COMMONMODEL_FALLEN, true);
	PrecacheModel("models/infected/common_male_riot.mdl", true);
	PrecacheModel("models/infected/common_male_ceda.mdl", true);
	PrecacheModel("models/infected/common_male_clown.mdl", true);
	PrecacheModel("models/infected/common_male_mud.mdl", true);
	PrecacheModel("models/infected/common_male_roadcrew.mdl", true);
	PrecacheModel("models/infected/common_male_jimmy.mdl", true);
	PrecacheModel("models/infected/common_male_fallen_survivor.mdl", true);
}

public SUH_OnMapEnd()
{
	RemainingZombiesToSpawn = 0;
}

//stock SpawnUncommonHorde(const CommonType = COMMONTYPE_RANDOM, const HordeAmount = 25);
stock SpawnUncommonHorde(const CommonType = 0, const HordeAmount = 25)
{
	HordeType = CommonType;
	RemainingZombiesToSpawn = HordeAmount;
	CheatCommand(GetAnyClient(), "z_spawn", "mob");
}

public OnEntityCreated(entity, const String:classname[])
{
	if (RemainingZombiesToSpawn <= 0 || !HordeType) return;
	
	if (!StrEqual(classname, "infected", false)) return;

	decl String:model[256];
	new number = HordeType;
	
	if (number == COMMONTYPE_RANDOM) number = GetRandomInt(1,6);
	
	switch (number)
	{
		case 1:
		{
			Format(model, sizeof(model), COMMONMODEL_RIOT);
		}
		case 2:
		{
			Format(model, sizeof(model), COMMONMODEL_CEDA);
		}
		case 3:
		{
			Format(model, sizeof(model), COMMONMODEL_CLOWN);
		}
		case 4:
		{
			Format(model, sizeof(model), COMMONMODEL_MUD);
		}
		case 5:
		{
			Format(model, sizeof(model), COMMONMODEL_ROADCREW);
		}
		case 6:
		{
			Format(model, sizeof(model), COMMONMODEL_JIMMY);
		}
		case 7:
		{
			Format(model, sizeof(model), COMMONMODEL_FALLEN);
		}
	}
	SetEntityModel(entity, model);
	RemainingZombiesToSpawn--;
}

GetAnyClient()
{
	for (new i = 1; i <= MaxClients; i++)
	{
		if (IsClientInGame(i))
		{
			return i;
		}
	}
	return 0;
}

CheatCommand(client, const String:command[], const String:arguments[]="")
{
	if (!client) return;
	new admindata = GetUserFlagBits(client);
	SetUserFlagBits(client, ADMFLAG_ROOT);
	new flags = GetCommandFlags(command);
	SetCommandFlags(command, flags & ~FCVAR_CHEAT);
	FakeClientCommand(client, "%s %s", command, arguments);
	SetCommandFlags(command, flags);
	SetUserFlagBits(client, admindata);
}