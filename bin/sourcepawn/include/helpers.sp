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

#define MAX_ENTITIES                    2048 // Max number of networked entities

/*
 * ==================================================
 *                     Public API
 * ==================================================
 */

/**
 * Executes cheat command on client.
 *
 * @param client        Client to execute cheat command, if not provided a 
 *                      random client will be picked.
 * @param command       Cheat command.
 * @param arguments     Arguments for command.
 * @return              True if executed, false otherwise.
 */
stock bool:CheatCommand(client = 0, String:command[], String:arguments[]="")
{
    if (client < 1 || client > MaxClients || !IsClientInGame(client))
    {
        client = GetAnyClient();
        if (client == -1) return false; // No players found to exec cheat cmd, return false
    }

    /* Apply root admin flag to user for compatiable with "Admin Cheats" plugin
     * by devicenull */
    new userFlags = GetUserFlagBits(client);
    SetUserFlagBits(client, ADMFLAG_ROOT);

    new flags = GetCommandFlags(command);
    SetCommandFlags(command, flags & ~FCVAR_CHEAT);
    FakeClientCommand(client, "%s %s", command, arguments);
    SetCommandFlags(command, flags);
    SetUserFlagBits(client, userFlags);

    return true;
}

/**
 * Wrapper for FindEntityByClassname to fall back on last valid entity.
 *
 * @param startEnt      The entity index after which to begin searching from. 
 *                      Use -1 to start from the first entity.
 * @param classname     Classname of the entity to find.
 * @return              Entity index >= 0 if found, -1 otherwise.
 */
stock FindEntityByClassnameEx(startEnt, const String:classname[])
{
    while (startEnt > -1 && !IsValidEntity(startEnt)) startEnt--;
    return FindEntityByClassname(startEnt, classname);
}

/**
 * Returns any ingame client.
 *
 * @param filterBots    Whether or not bots are also returned.
 * @return              Client index if found, -1 otherwise.
 */
stock GetAnyClient(bool:filterBots = false)
{
    FOR_EACH_CLIENT_IN_GAME(client)
    {
        if (filterBots && IsFakeClient(client)) continue;
        return client;
    }
    return -1;
}

/**
 * Returns the client count put in the server.
 *
 * @param inGameOnly    Whether or not connecting players are also counted.
 * @param fliterBots    Whether or not bots are also counted.
 * @return              Client count in the server.
 */
stock GetClientCountEx(bool:inGameOnly, bool:filterBots)
{
    new clients = 0;
    FOR_EACH_CLIENT_CONNECTED(client)
    {
        if (inGameOnly && !IsClientInGame(client)) continue;
        if (filterBots && IsFakeClient(client)) continue;
        clients++;
    }
    return clients;
}

/**
 * Retrives entity's absolute origin.
 *
 * @param entity        Entity index.
 * @param origin        Destination vector buffer to store origin in.
 * @noreturn
 */
stock GetEntityAbsOrigin(entity, Float:origin[3])
{
    if (entity < 1 || !IsValidEntity(entity)) return;

    decl Float:mins[3], Float:maxs[3];
    GetEntPropVector(entity, Prop_Send, "m_vecOrigin", origin);
    GetEntPropVector(entity, Prop_Send, "m_vecMins", mins);
    GetEntPropVector(entity, Prop_Send, "m_vecMaxs", maxs);

    for (new i = 0; i < 3; i++)
    {
        origin[i] += (mins[i] + maxs[i]) * 0.5;
    }
}

/**
 * Returns whether translation file is valid and readable.
 *
 * @param name          Name of translation file.
 * @return              True if valid, false otherwise.
 */
stock bool:IsTranslationValid(const String:name[])
{
    decl String:path[PLATFORM_MAX_PATH], Handle:file;
    BuildPath(Path_SM, path, PLATFORM_MAX_PATH, "translations/%s.txt", name);
    if (!FileExists(path, false))
    {
        return false;
    }
    else if ((file = OpenFile(path, "r")) == INVALID_HANDLE)
    {
        return false;
    }
    else
    {
        CloseHandle(file);
        return true;
    }
}

/**
 * Converts string to lower case.
 * Cant convert multibyte characters.
 *
 * @param string        String to convert.
 * @param len           String length (includes null terminator).
 * @noreturn
 */
stock StringToLower(String:string[], len)
{
    len--;
    new i = 0;

    for (i = 0; i < len; i++)
    {
        if (string[i] == '\0') break;
        string[i] = CharToLower(string[i]);
    }

    string[i] = '\0';
}

/**
 * Sends a SayText2 usermessage to a client
 *
 * @param client        Client index.
 * @param author        Author index.
 * @param message       Message.
 * @noreturn
 */
stock SayText2(client, author, const String:format[], any:...)
{
    decl String:buffer[256];
    VFormat(buffer, 256, format, 4);

    new Handle:hBuffer = StartMessageOne("SayText2", client);
    BfWriteByte(hBuffer, author);
    BfWriteByte(hBuffer, true);
    BfWriteString(hBuffer, buffer);
    EndMessage();
}