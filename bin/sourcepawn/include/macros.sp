/**
 * =============================================================================
 * L4D Health Glow (C)2011 Buster "Mr. Zero" Nielsen
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License, version 3.0, as 
 * published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along 
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2,"
 * the "Source Engine," the "SourcePawn JIT," and any Game MODs that run on 
 * software by the Valve Corporation.  You must obey the GNU General Public
 * License in all respects for all other code used.  Additionally, 
 * AlliedModders LLC grants this exception to all derivative works.  
 * AlliedModders LLC defines further exceptions, found in LICENSE.txt 
 * (as of this writing, version JULY-31-2007), or 
 * <http://www.sourcemod.net/license.php>.
 */

/*
 * ==================================================
 *                    Preprocessor
 * ==================================================
 */

#define FOR_EACH_CLIENT(%1)													\
    for (new %1 = 1; %1 <= MaxClients; %1++)

#define FOR_EACH_CLIENT_COND(%1,%2)											\
    for (new %1 = 1; %1 <= MaxClients; %1++)								\
        if (%2)

#define FOR_EACH_CLIENT_CONNECTED(%1)										\
    FOR_EACH_CLIENT_COND(%1, IsClientConnected(%1))

#define FOR_EACH_CLIENT_IN_GAME(%1)											\
    FOR_EACH_CLIENT_COND(%1, IsClientInGame(%1))

#define FOR_EACH_BOT(%1)													\
    FOR_EACH_CLIENT_COND(%1, (IsClientInGame(%1) && IsFakeClient(%1)))

#define FOR_EACH_HUMAN(%1)													\
    FOR_EACH_CLIENT_COND(%1, (IsClientInGame(%1) && !IsFakeClient(%1)))

#define FOR_EACH_ALIVE_BOT(%1)												\
    FOR_EACH_CLIENT_COND(%1, (IsClientInGame(%1) && IsFakeClient(%1) && GetClientHealth(%1) > 0 && IsPlayerAlive(%1))) 

#define FOR_EACH_ALIVE_HUMAN(%1)											\
    FOR_EACH_CLIENT_COND(%1, (IsClientInGame(%1) && !IsFakeClient(%1) && GetClientHealth(%1) > 0 && IsPlayerAlive(%1)))

#define FOR_EACH_ALIVE_CLIENT(%1)											\
    FOR_EACH_CLIENT_COND(%1, (IsClientInGame(%1) && GetClientHealth(%1) > 0 && IsPlayerAlive(%1)))

#define FOR_EACH_DEAD_BOT(%1)												\
    FOR_EACH_CLIENT_COND(%1, (IsClientInGame(%1) && IsFakeClient(%1) && !IsPlayerAlive(%1))) 

#define FOR_EACH_DEAD_HUMAN(%1)												\
    FOR_EACH_CLIENT_COND(%1, (IsClientInGame(%1) && !IsFakeClient(%1) && !IsPlayerAlive(%1)))

#define FOR_EACH_DEAD_CLIENT(%1)											\
    FOR_EACH_CLIENT_COND(%1, (IsClientInGame(%1) && !IsPlayerAlive(%1)))

#define FOR_EACH_BOT_ON_TEAM(%1,%2)											\
    FOR_EACH_BOT(%1)														\
        if (L4DTeam:GetClientTeam(%1) == %2)

#define FOR_EACH_HUMAN_ON_TEAM(%1,%2)										\
    FOR_EACH_HUMAN(%1)														\
        if (L4DTeam:GetClientTeam(%1) == %2)

#define FOR_EACH_ALIVE_BOT_ON_TEAM(%1,%2)									\
    FOR_EACH_ALIVE_BOT(%1)													\
        if (L4DTeam:GetClientTeam(%1) == %2)

#define FOR_EACH_ALIVE_HUMAN_ON_TEAM(%1,%2)									\
    FOR_EACH_ALIVE_HUMAN(%1)												\
        if (L4DTeam:GetClientTeam(%1) == %2)

#define FOR_EACH_DEAD_BOT_ON_TEAM(%1,%2)									\
    FOR_EACH_DEAD_BOT(%1)													\
        if (L4DTeam:GetClientTeam(%1) == %2)

#define FOR_EACH_DEAD_HUMAN_ON_TEAM(%1,%2)									\
    FOR_EACH_DEAD_HUMAN(%1)													\
        if (L4DTeam:GetClientTeam(%1) == %2)

#define FOR_EACH_CLIENT_ON_TEAM(%1,%2)										\
    FOR_EACH_CLIENT_IN_GAME(%1)												\
        if (L4DTeam:GetClientTeam(%1) == %2)

#define FOR_EACH_ALIVE_CLIENT_ON_TEAM(%1,%2)								\
    FOR_EACH_ALIVE_CLIENT(%1)												\
        if (L4DTeam:GetClientTeam(%1) == %2)

#define FOR_EACH_DEAD_CLIENT_ON_TEAM(%1,%2)									\
    FOR_EACH_DEAD_CLIENT(%1)												\
        if (L4DTeam:GetClientTeam(%1) == %2)

#define FOR_EACH_SURVIVOR_BOT(%1)											\
    FOR_EACH_BOT_ON_TEAM(%1, L4DTeam_Survivor)

#define FOR_EACH_SURVIVOR_HUMAN(%1)											\
    FOR_EACH_HUMAN_ON_TEAM(%1, L4DTeam_Survivor)

#define FOR_EACH_ALIVE_SURVIVOR_BOT(%1)										\
    FOR_EACH_ALIVE_BOT_ON_TEAM(%1, L4DTeam_Survivor)

#define FOR_EACH_ALIVE_SURVIVOR_HUMAN(%1)									\
    FOR_EACH_ALIVE_HUMAN_ON_TEAM(%1, L4DTeam_Survivor)

#define FOR_EACH_DEAD_SURVIVOR_BOT(%1)										\
    FOR_EACH_DEAD_BOT_ON_TEAM(%1, L4DTeam_Survivor)

#define FOR_EACH_DEAD_SURVIVOR_HUMAN(%1)									\
    FOR_EACH_DEAD_HUMAN_ON_TEAM(%1, L4DTeam_Survivor)

#define FOR_EACH_SURVIVOR(%1)												\
    FOR_EACH_CLIENT_ON_TEAM(%1, L4DTeam_Survivor)

#define FOR_EACH_ALIVE_SURVIVOR(%1)											\
    FOR_EACH_ALIVE_CLIENT_ON_TEAM(%1, L4DTeam_Survivor)

#define FOR_EACH_DEAD_SURVIVOR(%1)											\
    FOR_EACH_DEAD_CLIENT_ON_TEAM(%1, L4DTeam_Survivor)

#define FOR_EACH_SURVIVOR_INCAP(%1)											\
    FOR_EACH_ALIVE_CLIENT_ON_TEAM(%1, L4DTeam_Survivor)						\
        if (L4D_IsPlayerIncapacitated(%1))

#define FOR_EACH_SURVIVOR_INCAP_BOT(%1)										\
    FOR_EACH_ALIVE_BOT_ON_TEAM(%1, L4DTeam_Survivor)						\
        if (L4D_IsPlayerIncapacitated(%1))

#define FOR_EACH_SURVIVOR_INCAP_HUMAN(%1)									\
    FOR_EACH_ALIVE_HUMAN_ON_TEAM(%1, L4DTeam_Survivor)						\
        if (L4D_IsPlayerIncapacitated(%1))

#define FOR_EACH_INFECTED_BOT(%1)											\
    FOR_EACH_BOT_ON_TEAM(%1, L4DTeam_Infected)

#define FOR_EACH_INFECTED_HUMAN(%1)											\
    FOR_EACH_HUMAN_ON_TEAM(%1, L4DTeam_Infected)

#define FOR_EACH_ALIVE_INFECTED_BOT(%1)										\
    FOR_EACH_ALIVE_BOT_ON_TEAM(%1, L4DTeam_Infected)

#define FOR_EACH_ALIVE_INFECTED_HUMAN(%1)									\
    FOR_EACH_ALIVE_HUMAN_ON_TEAM(%1, L4DTeam_Infected)

#define FOR_EACH_DEAD_INFECTED_BOT(%1)										\
    FOR_EACH_DEAD_BOT_ON_TEAM(%1, L4DTeam_Infected)

#define FOR_EACH_DEAD_INFECTED_HUMAN(%1)									\
    FOR_EACH_DEAD_HUMAN_ON_TEAM(%1, L4DTeam_Infected)

#define FOR_EACH_INFECTED(%1)												\
    FOR_EACH_CLIENT_ON_TEAM(%1, L4DTeam_Infected)

#define FOR_EACH_ALIVE_INFECTED(%1)											\
    FOR_EACH_ALIVE_CLIENT_ON_TEAM(%1, L4DTeam_Infected)

#define FOR_EACH_DEAD_INFECTED(%1)											\
    FOR_EACH_DEAD_CLIENT_ON_TEAM(%1, L4DTeam_Infected)

#define FOR_EACH_HUMAN_SPECTATOR(%1)										\
    FOR_EACH_HUMAN_ON_TEAM(%1, L4DTeam_Spectator)