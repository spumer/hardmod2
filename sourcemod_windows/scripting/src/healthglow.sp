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

#define GLOW_HEALTH_HIGH 100 // Not used but just for completeness sake
#define GLOW_HEALTH_MED 40
#define GLOW_HEALTH_LOW 25

/*
 * L4D2_IsSurvivorGlowDisabled is used to "detect" whether realism mode is active.
 * As in no survivor glows in realism, means "less health glows" by this plugin.
 *
 * Minimum range ensures that glows are not shown when survivors are inside each other.
 * Also hides the players own glow from themself when in third person shoulder mode.
 */

#define GLOW_HEALTH_HIGH_TYPE L4D2_IsSurvivorGlowDisabled() ? L4D2Glow_None : L4D2Glow_OnUse
#define GLOW_HEALTH_HIGH_RANGE 64
#define GLOW_HEALTH_HIGH_MINRANGE 22
#define GLOW_HEALTH_HIGH_COLOR_R 0
#define GLOW_HEALTH_HIGH_COLOR_G 100
#define GLOW_HEALTH_HIGH_COLOR_B 0
#define GLOW_HEALTH_HIGH_FLASHING false

#define GLOW_HEALTH_MED_TYPE L4D2Glow_OnUse
#define GLOW_HEALTH_MED_RANGE L4D2_IsSurvivorGlowDisabled() ? 64 : 80
#define GLOW_HEALTH_MED_MINRANGE 22
#define GLOW_HEALTH_MED_COLOR_R 95
#define GLOW_HEALTH_MED_COLOR_G 95
#define GLOW_HEALTH_MED_COLOR_B 0
#define GLOW_HEALTH_MED_FLASHING false

#define GLOW_HEALTH_LOW_TYPE L4D2Glow_OnUse
#define GLOW_HEALTH_LOW_RANGE L4D2_IsSurvivorGlowDisabled() ? 64 : 96
#define GLOW_HEALTH_LOW_MINRANGE 22
#define GLOW_HEALTH_LOW_COLOR_R 135
#define GLOW_HEALTH_LOW_COLOR_G 0
#define GLOW_HEALTH_LOW_COLOR_B 0
#define GLOW_HEALTH_LOW_FLASHING false

#define GLOW_HEALTH_THIRDSTRIKE_TYPE L4D2_IsSurvivorGlowDisabled() ? L4D2Glow_OnUse : L4D2Glow_Constant
#define GLOW_HEALTH_THIRDSTRIKE_RANGE L4D2_IsSurvivorGlowDisabled() ? 96 : 0
#define GLOW_HEALTH_THIRDSTRIKE_MINRANGE 22
#define GLOW_HEALTH_THIRDSTRIKE_COLOR_R 100
#define GLOW_HEALTH_THIRDSTRIKE_COLOR_G 100
#define GLOW_HEALTH_THIRDSTRIKE_COLOR_B 100
#define GLOW_HEALTH_THIRDSTRIKE_FLASHING false

/*
 * ==================================================
 *                     Variables
 * ==================================================
 */

/*
 * --------------------
 *       Private
 * --------------------
 */

static          bool:   g_bIsGlowing[MAXPLAYERS + 1];
static          bool:   g_bIsIT[MAXPLAYERS + 1];

static                  g_iMaxIncaps = 2;
static          bool:   g_bIsGlowDisabled = false;

static          bool:   g_bIsPluginEnding = false;

/*
 * ==================================================
 *                     Forwards
 * ==================================================
 */

/**
 * Called on plugin start.
 *
 * @noreturn
 */
public _HealthGlow_OnPluginStart()
{
    HookConVarChange(FindConVar("survivor_max_incapacitated_count"), _HG_OnIncapMax_ConVarChange);
    HookConVarChange(FindConVar("sv_disable_glow_survivors"), _HG_OnGlowDisable_ConVarChange);

    /* SI grab events */
    HookEvent("pounce_end", _HG_UpdateGlow_Victim_Event);
    HookEvent("tongue_release", _HG_UpdateGlow_Victim_Event);
    HookEvent("jockey_ride_end", _HG_UpdateGlow_Victim_Event);
    HookEvent("charger_carry_end", _HG_UpdateGlow_Victim_Event);
    HookEvent("charger_pummel_end", _HG_UpdateGlow_Victim_Event);

    HookEvent("lunge_pounce", _HG_UpdateGlow_Victim_Event);
    HookEvent("tongue_grab", _HG_UpdateGlow_Victim_Event);
    HookEvent("jockey_ride", _HG_UpdateGlow_Victim_Event);
    HookEvent("charger_carry_start", _HG_UpdateGlow_Victim_Event);
    HookEvent("charger_pummel_start", _HG_UpdateGlow_Victim_Event);

    /* SI Boomer events */
    HookEvent("player_now_it", _HG_UpdateGlow_NowIT_Event);
    HookEvent("player_no_longer_it", _HG_UpdateGlow_NoLongerIt_Event);

    /* Survivor related events */
    HookEvent("revive_success", _HG_UpdateGlow_Subject_Event);
    HookEvent("heal_success", _HG_UpdateGlow_Subject_Event);
    HookEvent("player_incapacitated_start", _HG_UpdateGlow_UserId_Event);
    HookEvent("player_ledge_grab", _HG_UpdateGlow_UserId_Event);
    HookEvent("player_death", _HG_UpdateGlow_UserId_Event);
    HookEvent("defibrillator_used", _HG_UpdateGlow_Subject_Event);
    HookEvent("player_hurt", _HG_UpdateGlow_UserId_Event);

    HookEvent("player_bot_replace", _HG_BotReplacePlayer_Event);
    HookEvent("bot_player_replace", _HG_PlayerReplaceBot_Event);
}

/**
 * Called on plugin end.
 *
 * @noreturn
 */
public _HealthGlow_OnPluginEnd()
{
    g_bIsPluginEnding = true;

    FOR_EACH_SURVIVOR(client)
    {
        L4D2_RemoveEntGlow(client);
    }
}

public _HG_OnAllPluginsLoaded()
{
    g_iMaxIncaps = GetConVarInt(FindConVar("survivor_max_incapacitated_count"));
    g_bIsGlowDisabled = GetConVarBool(FindConVar("sv_disable_glow_survivors"));

    FOR_EACH_SURVIVOR(client)
    {
        UpdateSurvivorHealthGlow(client);
    }

    /* For people using admin cheats and other stuff that changes survivor 
     * health */
    CreateTimer(5.0, _HG_UpdateGlows_Timer, _, TIMER_REPEAT);
}

public _HG_OnIncapMax_ConVarChange(Handle:convar, const String:oldValue[], const String:newValue[])
{
    g_iMaxIncaps = GetConVarInt(convar);
}

public _HG_OnGlowDisable_ConVarChange(Handle:convar, const String:oldValue[], const String:newValue[])
{
    g_bIsGlowDisabled = GetConVarBool(convar);
}

public Action:_HG_UpdateGlows_Timer(Handle:timer)
{
    if (g_bIsPluginEnding) return Plugin_Stop;

    FOR_EACH_SURVIVOR(client)
    {
        UpdateSurvivorHealthGlow(client);
    }

    return Plugin_Continue;
}

public _HG_UpdateGlow_UserId_Event(Handle:event, const String:name[], bool:dontBroadcast)
{
    new client = GetClientOfUserId(GetEventInt(event, "userid"));
    if (client <= 0 || !IsClientInGame(client) || L4DTeam:GetClientTeam(client) != L4DTeam_Survivor) return;
    UpdateSurvivorHealthGlow(client);
}

public _HG_UpdateGlow_Subject_Event(Handle:event, const String:name[], bool:dontBroadcast)
{
    new client = GetClientOfUserId(GetEventInt(event, "subject"));
    if (client <= 0 || !IsClientInGame(client) || L4DTeam:GetClientTeam(client) != L4DTeam_Survivor) return;
    UpdateSurvivorHealthGlow(client);
}

public _HG_UpdateGlow_Victim_Event(Handle:event, const String:name[], bool:dontBroadcast)
{
    new client = GetClientOfUserId(GetEventInt(event, "victim"));
    if (client <= 0 || !IsClientInGame(client) || L4DTeam:GetClientTeam(client) != L4DTeam_Survivor) return;
    UpdateSurvivorHealthGlow(client);
}

public _HG_UpdateGlow_NowIT_Event(Handle:event, const String:name[], bool:dontBroadcast)
{
    new client = GetClientOfUserId(GetEventInt(event, "userid"));
    if (client <= 0 || !IsClientInGame(client) || L4DTeam:GetClientTeam(client) != L4DTeam_Survivor) return;
    g_bIsIT[client] = true;
    UpdateSurvivorHealthGlow(client);
}

public _HG_UpdateGlow_NoLongerIt_Event(Handle:event, const String:name[], bool:dontBroadcast)
{
    new client = GetClientOfUserId(GetEventInt(event, "userid"));
    if (client <= 0 || !IsClientInGame(client) || L4DTeam:GetClientTeam(client) != L4DTeam_Survivor) return;
    g_bIsIT[client] = false;
    UpdateSurvivorHealthGlow(client);
}

/**
 * Called when player replace bot event is fired.
 *
 * @param event         Handle to event.
 * @param name          String containing the name of the event.
 * @param dontBroadcast True if event was not broadcast to clients, false otherwise.
 * @noreturn
 */
public _HG_PlayerReplaceBot_Event(Handle:event, const String:name[], bool:dontBroadcast)
{
    new client = GetClientOfUserId(GetEventInt(event, "player"));
    if (!client || L4DTeam:GetClientTeam(client) != L4DTeam_Survivor) return;

    new bot = GetClientOfUserId(GetEventInt(event, "bot"));

    UpdateSurvivorHealthGlow(client);
    UpdateSurvivorHealthGlow(bot);
}

/**
 * Called when bot replace player event is fired.
 *
 * @param event         Handle to event.
 * @param name          String containing the name of the event.
 * @param dontBroadcast True if event was not broadcast to clients, false otherwise.
 * @noreturn
 */
public _HG_BotReplacePlayer_Event(Handle:event, const String:name[], bool:dontBroadcast)
{
    new client = GetClientOfUserId(GetEventInt(event, "player"));
    if (!client || L4DTeam:GetClientTeam(client) != L4DTeam_Survivor) return;

    new bot = GetClientOfUserId(GetEventInt(event, "bot"));

    UpdateSurvivorHealthGlow(client);
    UpdateSurvivorHealthGlow(bot);
}

/*
 * ==================================================
 *                     Public API
 * ==================================================
 */

stock UpdateSurvivorHealthGlow(client)
{
    if (g_bIsPluginEnding || !client || !IsClientInGame(client)) return;

    if (L4DTeam:GetClientTeam(client) != L4DTeam_Survivor || // If client isn't survivor
        !IsPlayerAlive(client) ||                            // or isn't alive
        L4D_IsPlayerIncapacitated(client) ||                 // or incapacitated
        L4D2_GetInfectedAttacker(client) > 0 ||              // or infected player is pining survivor
        g_bIsIT[client] ||                                   // or is IT (boomer vomit)
        !L4D2_IsPlayerSurvivorGlowEnable(client))            // or survivor glow is disabled on JUST this player
    {
        if (g_bIsGlowing[client])
        {
            g_bIsGlowing[client] = false;
            L4D2_RemoveEntGlow(client);
        }
        return;
    }

    new health = GetClientHealth(client);
    new bool:lastLife = L4D_GetPlayerReviveCount(client) >= L4D_GetMaxReviveCount();

    new L4D2GlowType:type;
    new color[3];
    new range;
    new minRange;
    new bool:flashing;
    GetHealthGlowForClient(health, lastLife, type, range, minRange, color, flashing);

    L4D2_SetEntGlow(client, type, range, minRange, color, flashing);
    g_bIsGlowing[client] = true;
}

/*
 * ==================================================
 *                    Private API
 * ==================================================
 */

static GetHealthGlowForClient(health, bool:lastLife, &L4D2GlowType:type, &range, &minRange, color[3], &bool:flashing)
{
    if (lastLife)
    {
        type = GLOW_HEALTH_THIRDSTRIKE_TYPE;
        range = GLOW_HEALTH_THIRDSTRIKE_RANGE;
        minRange = GLOW_HEALTH_THIRDSTRIKE_MINRANGE;
        color = {GLOW_HEALTH_THIRDSTRIKE_COLOR_R, GLOW_HEALTH_THIRDSTRIKE_COLOR_G, GLOW_HEALTH_THIRDSTRIKE_COLOR_B};
        flashing = GLOW_HEALTH_THIRDSTRIKE_FLASHING;
        return;
    }

    if (health <= GLOW_HEALTH_LOW)
    {
        type = GLOW_HEALTH_LOW_TYPE;
        range = GLOW_HEALTH_LOW_RANGE;
        minRange = GLOW_HEALTH_LOW_MINRANGE;
        color = {GLOW_HEALTH_LOW_COLOR_R, GLOW_HEALTH_LOW_COLOR_G, GLOW_HEALTH_LOW_COLOR_B};
        flashing = GLOW_HEALTH_MED_FLASHING;
    }
    else if (health <= GLOW_HEALTH_MED)
    {
        type = GLOW_HEALTH_MED_TYPE;
        range = GLOW_HEALTH_MED_RANGE;
        minRange = GLOW_HEALTH_MED_MINRANGE;
        color = {GLOW_HEALTH_MED_COLOR_R, GLOW_HEALTH_MED_COLOR_G, GLOW_HEALTH_MED_COLOR_B};
        flashing = GLOW_HEALTH_MED_FLASHING;
    }
    else
    {
        type = GLOW_HEALTH_HIGH_TYPE;
        range = GLOW_HEALTH_HIGH_RANGE;
        minRange = GLOW_HEALTH_HIGH_MINRANGE;
        color = {GLOW_HEALTH_HIGH_COLOR_R, GLOW_HEALTH_HIGH_COLOR_G, GLOW_HEALTH_HIGH_COLOR_B};
        flashing = GLOW_HEALTH_HIGH_FLASHING;
    }
}

static L4D_GetMaxReviveCount()
{
    return g_iMaxIncaps;
}

static bool:L4D2_IsSurvivorGlowDisabled()
{
    return g_bIsGlowDisabled;
}