#define SPECTATORTEAM 1
#define SURVIVORTEAM 2
#define INFECTEDTEAM 3
#define ZOMBIECLASS_SMOKER 1
#define ZOMBIECLASS_BOOMER 2
#define ZOMBIECLASS_HUNTER 3
#define ZOMBIECLASS_SPITTER 4
#define ZOMBIECLASS_JOCKEY 5
#define ZOMBIECLASS_CHARGER 6
#define ZOMBIECLASS_WITCH 7
#define ZOMBIECLASS_TANK 8
#define MAX_PLAYERS 32
#define CVAR_FLAGS FCVAR_PLUGIN
#pragma semicolon 1
#include <sourcemod>
#include <sdktools>
#include <adminmenu>

//////////////////////////////////////////
//		Infected GUI Menus
//////////////////////////////////////////

public Action:InfectedMenuFunc(client, args) {
	new Handle:menu = CreateMenu(InfectedMainMenuHandler);
	SetMenuTitle(menu, "\x04Points: \x03 %d", points[client]);
	
	AddMenuItem(menu, "option1", "Hunter");
	AddMenuItem(menu, "option2", "Smoker");
	AddMenuItem(menu, "option3", "Boomer");
	AddMenuItem(menu, "option4", "Jockey");
	AddMenuItem(menu, "option5", "Charger");
	AddMenuItem(menu, "option6", "Spitter");
	AddMenuItem(menu, "option7", "Witch");
	AddMenuItem(menu, "option8", "Tank");
	AddMenuItem(menu, "option9", "Horde");
	AddMenuItem(menu, "option10", "Mega Horde");
	//AddMenuItem(menu, "option11", "Ignorant Horde");
	//AddMenuItem(menu, "option12", "Uncommon Horde");
	//AddMenuItem(menu, "option13", "Death Horde");
	//AddMenuItem(menu, "option9", "Panic Event");
		
	SetMenuExitButton(menu, true);
	DisplayMenu(menu, client, MENU_TIME_FOREVER);

	return Plugin_Handled;
}

public InfectedMainMenuHandler(Handle:menu, MenuAction:action, client, itemNum)
{
    if (action == MenuAction_Select)
    {    
        switch (itemNum)
        {
            case 0:													// Purchase a hunter
            {
				if (HunterActiveAmount >= GetConVarInt(HunterActiveLimit))
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Hunter Limit Reached.");
				}
				else if (points[client] >= GetConVarInt(HunterCost))
				{
					new spawnflags = GetCommandFlags("z_spawn");
					SetCommandFlags("z_spawn", spawnflags & ~FCVAR_CHEAT);
					HunterActiveAmount++;
					FakeClientCommand(client, "z_spawn hunter auto");
					SetCommandFlags("z_spawn", spawnflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(HunterCost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
			}
            case 1:													// Purchase a smoker
            {
				if (SmokerActiveAmount >= GetConVarInt(SmokerActiveLimit))
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Smoker Limit Reached.");
				}
				else if (points[client] >= GetConVarInt(SmokerCost))
				{
					new spawnflags = GetCommandFlags("z_spawn");
					SetCommandFlags("z_spawn", spawnflags & ~FCVAR_CHEAT);
					SmokerActiveAmount++;
					FakeClientCommand(client, "z_spawn smoker auto");
					SetCommandFlags("z_spawn", spawnflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(SmokerCost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 2:													// Purchase a boomer
            {
				if (BoomerActiveAmount >= GetConVarInt(BoomerActiveLimit))
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Boomer Limit Reached.");
				}
				else if (points[client] >= GetConVarInt(BoomerCost))
				{
					new spawnflags = GetCommandFlags("z_spawn");
					SetCommandFlags("z_spawn", spawnflags & ~FCVAR_CHEAT);
					BoomerActiveAmount++;
					FakeClientCommand(client, "z_spawn boomer auto");
					SetCommandFlags("z_spawn", spawnflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(BoomerCost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 3:													// Purchase a jockey
            {
				if (JockeyActiveAmount >= GetConVarInt(JockeyActiveLimit))
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Jockey Limit Reached.");
				}
				else if (points[client] >= GetConVarInt(JockeyCost))
				{
					new spawnflags = GetCommandFlags("z_spawn");
					SetCommandFlags("z_spawn", spawnflags & ~FCVAR_CHEAT);
					JockeyActiveAmount++;
					FakeClientCommand(client, "z_spawn jockey auto");
					SetCommandFlags("z_spawn", spawnflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(JockeyCost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 4:													// Purchase a charger
            {
				if (ChargerActiveAmount >= GetConVarInt(ChargerActiveLimit))
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Charger Limit Reached.");
				}
				else if (points[client] >= GetConVarInt(ChargerCost))
				{
					new spawnflags = GetCommandFlags("z_spawn");
					SetCommandFlags("z_spawn", spawnflags & ~FCVAR_CHEAT);
					ChargerActiveAmount++;
					FakeClientCommand(client, "z_spawn charger auto");
					SetCommandFlags("z_spawn", spawnflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(ChargerCost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 5:													// Purchase a spitter
            {
				if (SpitterActiveAmount >= GetConVarInt(SpitterActiveLimit))
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Spitter Limit Reached.");
				}
				else if (points[client] >= GetConVarInt(SpitterCost))
				{
					new spawnflags = GetCommandFlags("z_spawn");
					SetCommandFlags("z_spawn", spawnflags & ~FCVAR_CHEAT);
					SpitterActiveAmount++;
					FakeClientCommand(client, "z_spawn spitter auto");
					SetCommandFlags("z_spawn", spawnflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(SpitterCost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 6:													// Purchase a witch
            {
				WitchBuyFunc(client);
            }
            case 7:													// Purchase a tank
            {
				TankBuyFunc(client);
            }
            case 8:
            {
				if (points[client] >= GetConVarInt(CommonSpawnCost))
				{
					if (IsCommonTooSoon < 1)
					{
						new spawnflags = GetCommandFlags("z_spawn");
						SetCommandFlags("z_spawn", spawnflags & ~FCVAR_CHEAT);
						FakeClientCommand(client, "z_spawn mob auto");
						SetCommandFlags("z_spawn", spawnflags|FCVAR_CHEAT);
						IsCommonTooSoon++;
						points[client] -= GetConVarInt(CommonSpawnCost);
						PrintToChatAll("\x04[ \x03BUY \x04] \x03Incoming \x04Horde \x03Courtesy of %N", client);
						CreateTimer(60.0,CommonTooSoonIsActive);
					}
					else
					{
						PrintToChat(client, "\x04[ \x03BUY \x04] \x03You must wait 30 seconds between Horde!");
					}
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
			}
			case 9:
			{
				if (points[client] >= GetConVarInt(HordeSpawnCost))
				{
					if (IsPanicTooSoon < 1)
					{
						new panicflags = GetCommandFlags("director_force_panic_event");
						SetCommandFlags("director_force_panic_event", panicflags & ~FCVAR_CHEAT);
						FakeClientCommand(client, "director_force_panic_event");
						SetCommandFlags("director_force_panic_event", panicflags|FCVAR_CHEAT);
						IsPanicTooSoon++;
						points[client] -= GetConVarInt(HordeSpawnCost);
						CreateTimer(90.0,PanicTooSoonIsActive);
						PrintToChatAll("\x04[ \x03BUY \x04] \x03Incoming \x04Panic Event \x03Courtesy of %N", client);
					}
					else
					{
						PrintToChat(client, "\x04[ \x03BUY \x04] \x03You must wait 90 seconds between Panic Events!");
					}
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x04Not Enough Points.");
				}
			}
        }
    }
}