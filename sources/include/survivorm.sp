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
//		Survivor GUI Menus
//////////////////////////////////////////

public Action:SurvivorMenuFunc(client, args) {
	new Handle:menu = CreateMenu(SurvivorMainMenuHandler);
	SetMenuTitle(menu, "\x04Points: \x03 %d", points[client]);
	
	AddMenuItem(menu, "option1", "Primary");
	AddMenuItem(menu, "option2", "Secondary");
	AddMenuItem(menu, "option3", "Health");
	AddMenuItem(menu, "option4", "Explosives");
	AddMenuItem(menu, "option5", "Upgrades");
	
	SetMenuExitButton(menu, true);
	DisplayMenu(menu, client, MENU_TIME_FOREVER);

	return Plugin_Handled;
}

public SurvivorMainMenuHandler(Handle:menu, MenuAction:action, client, itemNum)
{
    if ( action == MenuAction_Select )
    {
        switch (itemNum)
        {
            case 0:													// Primary Weapons
            {
				FakeClientCommand(client, "MainPrimaryMenu");
			}
            case 1:													// Secondary Weapons
            {
				FakeClientCommand(client, "MainSecondaryMenu");
            }
            case 2:													// Health
            {
				FakeClientCommand(client, "MainHealthMenu");
            }
			case 3:													// Explosives
            {
				FakeClientCommand(client, "MainExplosivesMenu");
            }
			case 4:													// Upgrades
            {
				FakeClientCommand(client, "MainUpgradesMenu");
            }
        }
    }
}

public Action:SurvivorMainPrimaryMenu(client,args)
{
	MainPrimaryMenu(client, args);
	return Plugin_Handled;
}

public Action:MainPrimaryMenu(client, args) {
	new Handle:menu = CreateMenu(SurvivorMainPrimaryMenuHandler);
	SetMenuTitle(menu, "Points: %d", points[client]);

	AddMenuItem(menu, "option1", "Tier 1");
	AddMenuItem(menu, "option2", "Tier 2");
	AddMenuItem(menu, "option3", "Back");
	
	SetMenuExitButton(menu, true);
	DisplayMenu(menu, client, MENU_TIME_FOREVER);
    
	return Plugin_Handled;
}

public SurvivorMainPrimaryMenuHandler(Handle:menu, MenuAction:action, client, itemNum)
{
    //new giveflags = GetCommandFlags("give");
	//SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
    if (action == MenuAction_Select)
    {
        switch (itemNum)
        {
            case 0:													// Tier 1 Weapons
            {
				FakeClientCommand(client, "SurvivorTier1Menu");
			}
            case 1:													// Tier 2 Weapons
            {
				FakeClientCommand(client, "SurvivorTier2Menu");
            }
            case 2:													// Return To Previous Menu
            {
				FakeClientCommand(client, "SurvivorMenuFunc");
            }
        }
    }
    //SetCommandFlags("give", giveflags|FCVAR_CHEAT);
}

public Action:SurvivorTier1Menu(client,args)
{
	Tier1Menu(client, args);
	return Plugin_Handled;
}

public Action:Tier1Menu(client, args) {
	new Handle:menu = CreateMenu(Tier1MenuHandler);
	SetMenuTitle(menu, "Points: %d", points[client]);

	AddMenuItem(menu, "option1", "Uzi");
	AddMenuItem(menu, "option2", "Tmp-5");
	AddMenuItem(menu, "option3", "MP5");
	AddMenuItem(menu, "option4", "Pump Shotgun");
	AddMenuItem(menu, "option5", "Previous");
	
	SetMenuExitButton(menu, true);
	DisplayMenu(menu, client, MENU_TIME_FOREVER);
    
	return Plugin_Handled;
}

public Tier1MenuHandler(Handle:menu, MenuAction:action, client, itemNum)
{
    if (action == MenuAction_Select)
    {
        switch (itemNum)
        {
            case 0:													// Purchase an Uzi
            {
				if (points[client] >= GetConVarInt(Primary1Cost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give smg");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(Primary1Cost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
			}
            case 1:													// Purchase a TMP-5
            {
				if (points[client] >= GetConVarInt(Primary1Cost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give smg_silenced");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(Primary1Cost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 2:													// Purchase a MP5
            {
				if (points[client] >= GetConVarInt(Primary1Cost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give smg_mp5");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(Primary1Cost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 3:													// Purchase a Pump Shotgun
            {
				if (points[client] >= GetConVarInt(Primary1Cost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give shotgun_chrome");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(Primary1Cost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 4:													// Previous Page
            {
				FakeClientCommand(client, "MainPrimaryMenu");
            }
        }
    }
    //SetCommandFlags("upgrade_add", upgradeflags|FCVAR_CHEAT);
	//SetCommandFlags("give", giveflags|FCVAR_CHEAT);
	//SetCommandFlags("z_spawn", spawnflags|FCVAR_CHEAT);
}

public Action:SurvivorTier2Menu(client,args)
{
	Tier2Menu(client, args);
	return Plugin_Handled;
}

public Action:Tier2Menu(client, args) {
	new Handle:menu = CreateMenu(Tier2MenuHandler);
	SetMenuTitle(menu, "Points: %d", points[client]);

	AddMenuItem(menu, "option1", "Auto Shotgun");
	AddMenuItem(menu, "option2", "SPAS Shotgun");
	AddMenuItem(menu, "option3", "M4A1");
	AddMenuItem(menu, "option4", "AK47");
	AddMenuItem(menu, "option5", "SG552");
	AddMenuItem(menu, "option6", "Hunting Rifle");
	AddMenuItem(menu, "option7", "Scout");
	AddMenuItem(menu, "option8", "Military");
	AddMenuItem(menu, "option9", "AWP");
	AddMenuItem(menu, "option10", "Previous");
	
	SetMenuExitButton(menu, true);
	DisplayMenu(menu, client, MENU_TIME_FOREVER);
    
	return Plugin_Handled;
}

public Tier2MenuHandler(Handle:menu, MenuAction:action, client, itemNum)
{
    if (action == MenuAction_Select)
    {
        switch (itemNum)
        {
            case 0:													// Purchase an AutoShotgun
            {
				if (points[client] >= GetConVarInt(Primary2Cost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give autoshotgun");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(Primary2Cost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
			}
            case 1:													// Purchase a SPAS Shotgun
            {
				if (points[client] >= GetConVarInt(Primary2Cost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give shotgun_spas");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(Primary2Cost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 2:													// Purchase a M4A1
            {
				if (points[client] >= GetConVarInt(Primary2Cost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give rifle");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(Primary2Cost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 3:													// Purchase a AK47
            {
				if (points[client] >= GetConVarInt(Primary2Cost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give rifle_ak47");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(Primary2Cost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 4:													// Purchase a SG552
            {
				if (points[client] >= GetConVarInt(Primary2Cost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give rifle_sg552");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(Primary2Cost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 5:													// Purchase a Hunting Rifle
            {
				if (points[client] >= GetConVarInt(Primary2Cost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give hunting_rifle");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(Primary2Cost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 6:													// Purchase a scout sniper
            {
				if (points[client] >= GetConVarInt(Primary2Cost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give sniper_scout");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(Primary2Cost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 7:													// Purchase a military sniper
            {
				if (points[client] >= GetConVarInt(Primary2Cost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give sniper_military");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(Primary2Cost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 8:													// Purchase a AWP sniper
            {
				if (points[client] >= GetConVarInt(Primary2Cost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give sniper_awp");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(Primary2Cost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 9:													// Previous Page
            {
				FakeClientCommand(client, "MainPrimaryMenu");
            }
        }
    }
}

public Action:SurvivorMainSecondaryMenu(client,args)
{
	MainSecondaryMenu(client, args);
	return Plugin_Handled;
}

public Action:MainSecondaryMenu(client, args)
{
	new Handle:menu = CreateMenu(SurvivorMainSecondaryMenuHandle);
	SetMenuTitle(menu, "Points: %d", points[client]);

	AddMenuItem(menu, "option1", "PP7");
	AddMenuItem(menu, "option2", "Desert Eagle");
	AddMenuItem(menu, "option3", "Melee");
	AddMenuItem(menu, "option4", "Back");
	
	SetMenuExitButton(menu, true);
	DisplayMenu(menu, client, MENU_TIME_FOREVER);
    
	return Plugin_Handled;
}

public SurvivorMainSecondaryMenuHandle(Handle:menu, MenuAction:action, client, itemNum)
{
    if (action == MenuAction_Select)
    {
        switch (itemNum)
        {
            case 0:													// Purchase an Pistol
            {
				if (points[client] >= GetConVarInt(SecondaryCost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give pistol");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(SecondaryCost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
			}
            case 1:													// Purchase a Desert Eagle
            {
				if (points[client] >= GetConVarInt(SecondaryCost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give pistol_magnum");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(SecondaryCost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 2:
            {
				if (points[client] >= GetConVarInt(SecondaryCost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give katana");
					FakeClientCommand(client, "give machete");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(SecondaryCost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
			}
            case 3:													// Previous Page
            {
				FakeClientCommand(client, "SurvivorMenuFunc");
            }
        }
    }
    //SetCommandFlags("upgrade_add", upgradeflags|FCVAR_CHEAT);
	//SetCommandFlags("give", giveflags|FCVAR_CHEAT);
	//SetCommandFlags("z_spawn", spawnflags|FCVAR_CHEAT);
}

public Action:SurvivorMainHealthMenu(client,args)
{
	MainHealthMenu(client, args);
	return Plugin_Handled;
}

public Action:MainHealthMenu(client, args) {
	new Handle:menu = CreateMenu(SurvivorMainHealthMenuHandler);
	SetMenuTitle(menu, "Points: %d", points[client]);

	AddMenuItem(menu, "option1", "Adrenaline");
	AddMenuItem(menu, "option2", "Defibrillator");
	AddMenuItem(menu, "option3", "First Aid Kit");
	AddMenuItem(menu, "option4", "Pain Pills");
	AddMenuItem(menu, "option5", "Full Health");
	AddMenuItem(menu, "option6", "Refill Ammo");
	AddMenuItem(menu, "option7", "Back");
	
	SetMenuExitButton(menu, true);
	DisplayMenu(menu, client, MENU_TIME_FOREVER);
    
	return Plugin_Handled;
}

public SurvivorMainHealthMenuHandler(Handle:menu, MenuAction:action, client, itemNum)
{
    if (action == MenuAction_Select)
    {
        switch (itemNum)
        {
            case 0:													// Purchase a adrenaline
            {
				if (points[client] >= GetConVarInt(HealthItemCost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give adrenaline");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(HealthItemCost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
			}
            case 1:													// Purchase a Defibrillator
            {
				if (DefibAmount[client] < GetConVarInt(DefibAllowed))
				{
					if (points[client] >= GetConVarInt(HealthItemCost))
					{
						new giveflags = GetCommandFlags("give");
						SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
						FakeClientCommand(client, "give defibrillator");
						SetCommandFlags("give", giveflags|FCVAR_CHEAT);
						points[client] -= GetConVarInt(HealthItemCost);
						DefibAmount[client]++;
						PrintToChat(client, "\x04[ \x03BUY \x04] \x03Purchased \x04 %d \x03/ \x04 %d \x03Defibs.", DefibAmount[client], GetConVarInt(DefibAllowed));
					}
					else
					{
						PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
					}
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Purchase Limit Reached!");
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Purchased \x04 %d \x03/ \x04 %d \x03Defibs.", DefibAmount[client], GetConVarInt(DefibAllowed));
				}
            }
            case 2:													// Purchase a First Aid Kit
            {
				if (MedkitAmount[client] < GetConVarInt(MedkitAllowed))
				{
					if (points[client] >= GetConVarInt(HealthItemCost))
					{
						new giveflags = GetCommandFlags("give");
						SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
						FakeClientCommand(client, "give first_aid_kit");
						SetCommandFlags("give", giveflags|FCVAR_CHEAT);
						points[client] -= GetConVarInt(HealthItemCost);
					}
					else
					{
						PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
					}
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Purchase Limit Reached!");
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Purchased \x04 %d \x03/ \x04 %d \x03Medkits.", MedkitAmount[client], GetConVarInt(MedkitAllowed));
				}
            }
            case 3:													// Purchase Pain Pills
            {
				if (points[client] >= GetConVarInt(HealthItemCost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give pain_pills");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(HealthItemCost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 4:													// Purchase Full Health
            {
				HealthBuyFunc(client);
            }
            case 5:													// Purchase Ammo Refill
            {
				if (points[client] >= GetConVarInt(AmmoCost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give ammo");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(AmmoCost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 6:													// Previous Page
            {
				FakeClientCommand(client, "SurvivorMenuFunc");
            }
        }
    }
    //SetCommandFlags("upgrade_add", upgradeflags|FCVAR_CHEAT);
	//SetCommandFlags("give", giveflags|FCVAR_CHEAT);
	//SetCommandFlags("z_spawn", spawnflags|FCVAR_CHEAT);
}

public Action:SurvivorMainExplosivesMenu(client,args)
{
	MainExplosivesMenu(client, args);
	return Plugin_Handled;
}

public Action:MainExplosivesMenu(client, args) {
	new Handle:menu = CreateMenu(SurvivorMainExplosives);
	SetMenuTitle(menu, "Points: %d", points[client]);

	AddMenuItem(menu, "option1", "Gascan");
	AddMenuItem(menu, "option2", "Molotov");
	AddMenuItem(menu, "option3", "Pipebomb");
	AddMenuItem(menu, "option4", "Vomitjar");
	AddMenuItem(menu, "option5", "Back");
	
	SetMenuExitButton(menu, true);
	DisplayMenu(menu, client, MENU_TIME_FOREVER);
    
	return Plugin_Handled;
}

public SurvivorMainExplosives(Handle:menu, MenuAction:action, client, itemNum)
{
	if (action == MenuAction_Select)
	{
        switch (itemNum)
        {
            case 0:													// Purchase a gascan
            {
				if (points[client] >= GetConVarInt(ExplosiveCost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give gascan");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(ExplosiveCost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
			}
            case 1:													// Purchase a Molotov
            {
				if (points[client] >= GetConVarInt(MolotovCost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give molotov");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(MolotovCost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 2:													// Purchase a Pipebomb
            {
				if (points[client] >= GetConVarInt(PipebombCost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give pipe_bomb");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(PipebombCost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 3:													// Purchase a Vomitjar
            {
				if (points[client] >= GetConVarInt(VomitjarCost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give vomitjar");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(VomitjarCost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 4:													// Previous Page
            {
				FakeClientCommand(client, "SurvivorMenuFunc");
            }
        }
    }
    //SetCommandFlags("upgrade_add", upgradeflags|FCVAR_CHEAT);
	//SetCommandFlags("give", giveflags|FCVAR_CHEAT);
	//SetCommandFlags("z_spawn", spawnflags|FCVAR_CHEAT);
}

public Action:SurvivorMainUpgradesMenu(client,args)
{
	MainUpgradesMenu(client, args);
	return Plugin_Handled;
}

public Action:MainUpgradesMenu(client, args) {
	new Handle:menu = CreateMenu(SurvivorMainUpgradesMenuHandle);
	SetMenuTitle(menu, "Points: %d", points[client]);

	AddMenuItem(menu, "option1", "Explosive Ammo");
	AddMenuItem(menu, "option2", "Incendiary Ammo");
	AddMenuItem(menu, "option3", "Laser Sight");
	AddMenuItem(menu, "option4", "Back");
	
	SetMenuExitButton(menu, true);
	DisplayMenu(menu, client, MENU_TIME_FOREVER);
    
	return Plugin_Handled;
}

public SurvivorMainUpgradesMenuHandle(Handle:menu, MenuAction:action, client, itemNum)
{
	if (action == MenuAction_Select)
	{
        switch (itemNum)
        {
            case 0:													// Purchase a explosive upgradepack
            {
				if (points[client] >= GetConVarInt(UpgradeCost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give upgradepack_explosive");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(UpgradeCost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
			}
            case 1:													// Purchase a incendiary upgradepack
            {
				if (points[client] >= GetConVarInt(UpgradeCost))
				{
					new giveflags = GetCommandFlags("give");
					SetCommandFlags("give", giveflags & ~FCVAR_CHEAT);
					FakeClientCommand(client, "give upgradepack_incendiary");
					SetCommandFlags("give", giveflags|FCVAR_CHEAT);
					points[client] -= GetConVarInt(UpgradeCost);
				}
				else
				{
					PrintToChat(client, "\x04[ \x03BUY \x04] \x03Not Enough Points.");
				}
            }
            case 2:
            {														// Purhcase a Laser Sight Upgrade
				LaserBuyFunc(client);
			}
            case 3:													// Previous Page
            {
				FakeClientCommand(client, "SurvivorMenuFunc");
            }
        }
    }
    //SetCommandFlags("upgrade_add", upgradeflags|FCVAR_CHEAT);
	//SetCommandFlags("give", giveflags|FCVAR_CHEAT);
	//SetCommandFlags("z_spawn", spawnflags|FCVAR_CHEAT);
}
