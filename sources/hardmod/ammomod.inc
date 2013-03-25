#define REMOVE_AMMO false
#define FLAME true

#if CHEAT_ENABLED == false
	#undef CHEAT_ENABLED
	#define CHEAT_ENABLED true
	CheatOnPluginStart();
#endif

#if FLAME
	#include "hardmod/flame.inc"
#endif

new Handle:hm_ammo_nextbox;

new Handle:hm_ammo_count;
new Handle:hm_ammo_count_bonus;
new Handle:hm_ammo_medbox_count;

new Handle:hm_ammochance_nothing;
new Handle:hm_ammochance_firebox;
new Handle:hm_ammochance_boombox;
new Handle:hm_ammochance_explosionbox;
new Handle:hm_ammochance_freezebox;
#if AIRSTRIKE_ENABLED
new Handle:hm_ammochance_hellbox;
#endif
new Handle:hm_ammochance_laserbox;
new Handle:hm_ammochance_medbox;
new Handle:hm_ammochance_nextbox;
new Handle:hm_ammochance_icebox;
new Handle:hm_ammochance_panicbox;
new Handle:hm_ammochance_hardbox;
new Handle:hm_ammochance_witchbox;
new Handle:hm_ammochance_failbox;
#if COOP_STATS_ENABLED
new Handle:hm_ammochance_pointsbox;
new Handle:hm_ammo_pointsbonus;
new Handle:hm_ammo_pointsbonus_auto;
new Handle:hm_ammochance_prizebox;
#endif
#if COOP_AUTOD_ENABLED
new Handle:hm_ammochance_bonusbox;
new Handle:hm_ammo_loot_bonus;
#endif
new Handle:hm_ammochance_realismbox;
new Handle:hm_ammochance_tankbox;
new Handle:hm_ammochance_bloodbox;
#if FLAME
new Handle:hm_ammochance_flamebox;
#endif
new Handle:hm_ammochance_healbox;
new Handle:hm_ammochance_multiplebox;
new Handle:hm_ammochance_custombox;
new Handle:hm_ammochance_custombox2;
new Handle:hm_ammochance_custombox3;
new Handle:hm_ammochance_custombox4;
new Handle:hm_ammochance_custombox5;
new Handle:hm_ammochance_custombox6;
new Handle:hm_ammochance_custombox7;
new Handle:hm_custombox_name;
new Handle:hm_custombox_name2;
new Handle:hm_custombox_name3;
new Handle:hm_custombox_name4;
new Handle:hm_custombox_name5;
new Handle:hm_custombox_name6;
new Handle:hm_custombox_name7;
new Handle:hm_custombox_command;
new Handle:hm_custombox_command2;
new Handle:hm_custombox_command3;
new Handle:hm_custombox_command4;
new Handle:hm_custombox_command5;
new Handle:hm_custombox_command6;
new Handle:hm_custombox_command7;

new Handle:hm_ammo_multipleboxes;
new Handle:hm_ammo_witches;
new Handle:hm_ammo_harddamage;
new Handle:hm_ammo_nohumantank;

#if REMOVE_AMMO
	new Handle:hm_ammo_removechance;
	new PlayersLastAmmoUsed[L4D_MAXPLAYERS + 1];
#endif	

#if AIRSTRIKE_ENABLED
new bool:g_bRing = false;
#endif

#define SOUND_BLIP	"buttons/blip1.wav"

new String:cvar_custombox_name[24];
new String:cvar_custombox_name2[24];
new String:cvar_custombox_name3[24];
new String:cvar_custombox_name4[24];
new String:cvar_custombox_name5[24];
new String:cvar_custombox_name6[24];
new String:cvar_custombox_name7[24];
new String:cvar_custombox_command[128];
new String:cvar_custombox_command2[128];
new String:cvar_custombox_command3[128];
new String:cvar_custombox_command4[128];
new String:cvar_custombox_command5[128];
new String:cvar_custombox_command6[128];
new String:cvar_custombox_command7[128];

public AmmoModOnPluginStart()
{
	Version += 32;
	
	RegAdminCmd("info_weapongrade", Command_WeaponGrade, ADMFLAG_ROOT, "info_weapongrade");
	
	HookEvent("upgrade_pack_added", Event_UpgradePackAdded);
	HookEvent("upgrade_pack_used", Event_UpgradePackUsed);
#if REMOVE_AMMO
	HookEvent("player_use", Event_AmmoPlayerUse);
	HookEvent("ammo_pickup", Event_AmmoPickup);
#endif
	hm_ammo_nextbox = CreateConVar("hm_ammo_nextbox", "random", "", FCVAR_PLUGIN);
	hm_ammo_count = CreateConVar("hm_ammo_count", "15", "", FCVAR_PLUGIN);
	hm_ammo_count_bonus = CreateConVar("hm_ammo_count_bonus", "50", "", FCVAR_PLUGIN);
	hm_ammo_medbox_count = CreateConVar("hm_ammo_medbox_count", "9", "", FCVAR_PLUGIN);
	hm_ammochance_nothing = CreateConVar("hm_ammochance_nothing", "240", "", FCVAR_PLUGIN);
	hm_ammochance_firebox = CreateConVar("hm_ammochance_firebox", "10", "", FCVAR_PLUGIN);
	hm_ammochance_boombox = CreateConVar("hm_ammochance_boombox", "10", "", FCVAR_PLUGIN);
	hm_ammochance_explosionbox = CreateConVar("hm_ammochance_explosionbox", "6", "", FCVAR_PLUGIN);
	hm_ammochance_freezebox = CreateConVar("hm_ammochance_freezebox", "4", "", FCVAR_PLUGIN);
#if AIRSTRIKE_ENABLED
	hm_ammochance_hellbox = CreateConVar("hm_ammochance_hellbox", "0", "", FCVAR_PLUGIN);
#endif
	hm_ammochance_laserbox = CreateConVar("hm_ammochance_laserbox", "20", "", FCVAR_PLUGIN);
	hm_ammochance_medbox = CreateConVar("hm_ammochance_medbox", "9", "", FCVAR_PLUGIN);
	hm_ammochance_nextbox = CreateConVar("hm_ammochance_nextbox", "36", "", FCVAR_PLUGIN);
	hm_ammochance_icebox = CreateConVar("hm_ammochance_icebox", "10", "", FCVAR_PLUGIN);
	hm_ammochance_panicbox = CreateConVar("hm_ammochance_panicbox", "20", "", FCVAR_PLUGIN);
	hm_ammochance_hardbox = CreateConVar("hm_ammochance_hardbox", "0", "", FCVAR_PLUGIN);
	hm_ammochance_witchbox = CreateConVar("hm_ammochance_witchbox", "20", "", FCVAR_PLUGIN);
	hm_ammochance_failbox = CreateConVar("hm_ammochance_failbox", "0", "", FCVAR_PLUGIN);
#if COOP_STATS_ENABLED
	hm_ammochance_pointsbox = CreateConVar("hm_ammochance_pointsbox", "1", "", FCVAR_PLUGIN);
	hm_ammo_pointsbonus = CreateConVar("hm_ammo_pointsbonus", "500", "", FCVAR_PLUGIN);
	hm_ammo_pointsbonus_auto = CreateConVar("hm_ammo_pointsbonus_auto", "10", "", FCVAR_PLUGIN);
	hm_ammochance_prizebox = CreateConVar("hm_ammochance_prizebox", "5", "", FCVAR_PLUGIN);
#endif
#if COOP_AUTOD_ENABLED
	hm_ammochance_bonusbox = CreateConVar("hm_ammochance_bonusbox", "3", "", FCVAR_PLUGIN);
	hm_ammo_loot_bonus = CreateConVar("hm_ammo_loot_bonus", "2", "", FCVAR_PLUGIN);
#endif	
	hm_ammochance_realismbox = CreateConVar("hm_ammochance_realismbox", "2", "", FCVAR_PLUGIN);
	hm_ammochance_tankbox = CreateConVar("hm_ammochance_tankbox", "8", "", FCVAR_PLUGIN);
	hm_ammochance_bloodbox = CreateConVar("hm_ammochance_bloodbox", "2", "", FCVAR_PLUGIN);
#if FLAME
	hm_ammochance_flamebox = CreateConVar("hm_ammochance_flamebox", "6", "", FCVAR_PLUGIN);
#endif
	hm_ammochance_healbox = CreateConVar("hm_ammochance_healbox", "9", "", FCVAR_PLUGIN);
	hm_ammochance_multiplebox = CreateConVar("hm_ammochance_multiplebox", "3", "", FCVAR_PLUGIN);
	hm_ammochance_custombox = CreateConVar("hm_ammochance_custombox", "0", "", FCVAR_PLUGIN);
	hm_ammochance_custombox2 = CreateConVar("hm_ammochance_custombox2", "0", "", FCVAR_PLUGIN);
	hm_ammochance_custombox3 = CreateConVar("hm_ammochance_custombox3", "0", "", FCVAR_PLUGIN);
	hm_ammochance_custombox4 = CreateConVar("hm_ammochance_custombox4", "0", "", FCVAR_PLUGIN);
	hm_ammochance_custombox5 = CreateConVar("hm_ammochance_custombox5", "0", "", FCVAR_PLUGIN);
	hm_ammochance_custombox6 = CreateConVar("hm_ammochance_custombox6", "0", "", FCVAR_PLUGIN);
	hm_ammochance_custombox7 = CreateConVar("hm_ammochance_custombox7", "0", "", FCVAR_PLUGIN);
	hm_custombox_name = CreateConVar("hm_custombox_name", "", "", FCVAR_PLUGIN);
	hm_custombox_name2 = CreateConVar("hm_custombox_name2", "", "", FCVAR_PLUGIN);
	hm_custombox_name3 = CreateConVar("hm_custombox_name3", "", "", FCVAR_PLUGIN);
	hm_custombox_name4 = CreateConVar("hm_custombox_name4", "", "", FCVAR_PLUGIN);
	hm_custombox_name5 = CreateConVar("hm_custombox_name5", "", "", FCVAR_PLUGIN);
	hm_custombox_name6 = CreateConVar("hm_custombox_name6", "", "", FCVAR_PLUGIN);
	hm_custombox_name7 = CreateConVar("hm_custombox_name7", "", "", FCVAR_PLUGIN);
	hm_custombox_command = CreateConVar("hm_custombox_command", "custombox_command #", "", FCVAR_PLUGIN);
	hm_custombox_command2 = CreateConVar("hm_custombox_command2", "custombox_command2 #", "", FCVAR_PLUGIN);
	hm_custombox_command3 = CreateConVar("hm_custombox_command3", "custombox_command2 #", "", FCVAR_PLUGIN);
	hm_custombox_command4 = CreateConVar("hm_custombox_command4", "custombox_command2 #", "", FCVAR_PLUGIN);
	hm_custombox_command5 = CreateConVar("hm_custombox_command5", "custombox_command2 #", "", FCVAR_PLUGIN);
	hm_custombox_command6 = CreateConVar("hm_custombox_command6", "custombox_command2 #", "", FCVAR_PLUGIN);
	hm_custombox_command7 = CreateConVar("hm_custombox_command7", "custombox_command2 #", "", FCVAR_PLUGIN);

	hm_ammo_multipleboxes = CreateConVar("hm_ammo_multipleboxes", "15", "", FCVAR_PLUGIN);
	hm_ammo_witches = CreateConVar("hm_ammo_witches", "7", "", FCVAR_PLUGIN);
	hm_ammo_harddamage = CreateConVar("hm_ammo_harddamage", "7", "", FCVAR_PLUGIN);
	hm_ammo_nohumantank = CreateConVar("hm_ammo_nohumantank", "0", "", FCVAR_PLUGIN);

#if REMOVE_AMMO
	hm_ammo_removechance = CreateConVar("hm_ammo_removechance", "0", "", FCVAR_PLUGIN);
#endif
}

AmmoModOnMapStart()
{
#if FLAME
	FlameOnMapStart();
#endif
}

public Action:Command_WeaponGrade(client, args)
{
	PrintToChat(client, "Weapon Grade = %d", GetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_upgradeBitVec"));
}

public Action:Event_UpgradePackAdded(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	
	new upgradeid = GetEventInt(event, "upgradeid");
	if (!IsValidEdict(upgradeid))
	{
		return;
	}
	decl String:class[256];
	GetEdictClassname(upgradeid, class, sizeof(class));
	
	if (StrEqual(class, "upgrade_laser_sight"))
	{
		if (GetRandomInt(1, 2) == 1)
		{
			RemoveEdict(upgradeid);
		}
		return;
	}
	decl String:PrimaryWeaponName[64];
	GetEdictClassname(GetPlayerWeaponSlot(client, 0), PrimaryWeaponName, sizeof(PrimaryWeaponName));

	if (StrEqual(PrimaryWeaponName, "weapon_grenade_launcher", false))
	{
		new MaxAmmo = GetRandomInt(1, 10);
		if (MaxAmmo == 1)
		{
			SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_iClip1", GetConVarInt(hm_ammo_count_bonus));
			SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_nUpgradedPrimaryAmmoLoaded", GetConVarInt(hm_ammo_count_bonus), 1);
		}
		else
		{
			SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_iClip1", GetConVarInt(hm_ammo_count));
			SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_nUpgradedPrimaryAmmoLoaded", GetConVarInt(hm_ammo_count), 1);
		}
		RemoveEdict(upgradeid);
		return;
	}
	if (StrEqual(PrimaryWeaponName, "weapon_rifle_m60", false))
	{
		RemoveEdict(upgradeid);
		new ammo = GetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_iClip1");
		new ammoupgrade = GetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_upgradeBitVec");
		if (ammoupgrade == 4 || ammoupgrade == 5 || ammoupgrade == 6) ammoupgrade = 4;
		else ammoupgrade = 0;
		if (ammo < 0)
		{
			ammo += 356;
			if (ammo > 250)	ammo = 250;
		}
		else if (ammo <= 150) ammo += 100;
		else ammo = 250;
		SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_iClip1", ammo);
		SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_nUpgradedPrimaryAmmoLoaded", 0);
		SetEntProp(GetPlayerWeaponSlot(client, 0), Prop_Send, "m_upgradeBitVec", ammoupgrade);
		return;
	}	
	else if (GetSpecialAmmoInPlayerGun(client) > 1)
	{
		new AMMORND = GetRandomInt(1, 3);
		SetSpecialAmmoInPlayerGun(client, AMMORND * GetSpecialAmmoInPlayerGun(client));
	}
	RemoveEdict(upgradeid);
}

public Action:Event_UpgradePackUsed(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	new upgradeid = GetEventInt(event, "upgradeid");
	
	new Float:position[3];
	GetEntPropVector(client, Prop_Send, "m_vecOrigin", position);
	
	new String:cvar_hm_ammo_nextbox[24];
	GetConVarString(hm_ammo_nextbox, cvar_hm_ammo_nextbox, sizeof(cvar_hm_ammo_nextbox));
	
	if (StrEqual(cvar_hm_ammo_nextbox, "random", false))
	{
		new Sum = 0;
		Sum += GetConVarInt(hm_ammochance_nothing);
		Sum += GetConVarInt(hm_ammochance_firebox);
		Sum += GetConVarInt(hm_ammochance_boombox);
		Sum += GetConVarInt(hm_ammochance_explosionbox);
		Sum += GetConVarInt(hm_ammochance_freezebox);
#if AIRSTRIKE_ENABLED		
		Sum += GetConVarInt(hm_ammochance_hellbox);
#endif		
		Sum += GetConVarInt(hm_ammochance_laserbox);
		Sum += GetConVarInt(hm_ammochance_medbox);
		Sum += GetConVarInt(hm_ammochance_nextbox);
		Sum += GetConVarInt(hm_ammochance_icebox);
		Sum += GetConVarInt(hm_ammochance_panicbox);
		Sum += GetConVarInt(hm_ammochance_hardbox);
		Sum += GetConVarInt(hm_ammochance_witchbox);
		Sum += GetConVarInt(hm_ammochance_failbox);
#if COOP_STATS_ENABLED
		Sum += GetConVarInt(hm_ammochance_pointsbox);
		Sum += GetConVarInt(hm_ammochance_prizebox);
#endif
#if COOP_AUTOD_ENABLED
		Sum += GetConVarInt(hm_ammochance_bonusbox);
#endif	
		Sum += GetConVarInt(hm_ammochance_realismbox);
		Sum += GetConVarInt(hm_ammochance_tankbox);
		Sum += GetConVarInt(hm_ammochance_bloodbox);
#if FLAME
		Sum += GetConVarInt(hm_ammochance_flamebox);
#endif
		Sum += GetConVarInt(hm_ammochance_healbox);
		Sum += GetConVarInt(hm_ammochance_multiplebox);
		Sum += GetConVarInt(hm_ammochance_custombox);
		Sum += GetConVarInt(hm_ammochance_custombox2);
		Sum += GetConVarInt(hm_ammochance_custombox3);
		Sum += GetConVarInt(hm_ammochance_custombox4);
		Sum += GetConVarInt(hm_ammochance_custombox5);
		Sum += GetConVarInt(hm_ammochance_custombox6);
		Sum += GetConVarInt(hm_ammochance_custombox7);

		if (Sum > 0)
		{ 
			new Float:X = 1000.0 / Sum;
			new Float:Y = GetRandomFloat(0.0, 1000.0);
			new Float:A = 0.0;
			new Float:B = GetConVarInt(hm_ammochance_nothing) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "nothing";
			A = A + B;
			B = GetConVarInt(hm_ammochance_firebox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "firebox";
			A = A + B;
			B = GetConVarInt(hm_ammochance_boombox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "boombox";
			A = A + B;
			B = GetConVarInt(hm_ammochance_explosionbox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "explosionbox";
			A = A + B;
			B = GetConVarInt(hm_ammochance_freezebox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "freezebox";
#if AIRSTRIKE_ENABLED			
			A = A + B;
			B = GetConVarInt(hm_ammochance_hellbox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "hellbox";
#endif			
			A = A + B;
			B = GetConVarInt(hm_ammochance_laserbox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "laserbox";
			A = A + B;
			B = GetConVarInt(hm_ammochance_medbox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "medbox";
			A = A + B;
			B = GetConVarInt(hm_ammochance_nextbox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "nextbox";
			A = A + B;
			B = GetConVarInt(hm_ammochance_icebox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "icebox";
			A = A + B;
			B = GetConVarInt(hm_ammochance_panicbox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "panicbox";
			A = A + B;
			B = GetConVarInt(hm_ammochance_hardbox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "hardbox";
			A = A + B;
			B = GetConVarInt(hm_ammochance_witchbox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "witchbox";
			A = A + B;
			B = GetConVarInt(hm_ammochance_failbox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "failbox";
#if COOP_STATS_ENABLED
			A = A + B;
			B = GetConVarInt(hm_ammochance_pointsbox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "pointsbox";
#endif
#if COOP_AUTOD_ENABLED
			A = A + B;
			B = GetConVarInt(hm_ammochance_bonusbox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "bonusbox";
#endif
#if COOP_STATS_ENABLED			
			A = A + B;
			B = GetConVarInt(hm_ammochance_prizebox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "prizebox";
#endif
			A = A + B;
			B = GetConVarInt(hm_ammochance_realismbox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "realismbox";
			A = A + B;
			B = GetConVarInt(hm_ammochance_tankbox) * X;
			if (Y >= A && Y < A + B)
			{
				cvar_hm_ammo_nextbox = "tankbox";
#if PLAYERS_ENABLED
				if (NoTankPlayer[client]) cvar_hm_ammo_nextbox = "nothing";
#endif
			}
			A = A + B;
			B = GetConVarInt(hm_ammochance_bloodbox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "bloodbox";
#if FLAME
			A = A + B;
			B = GetConVarInt(hm_ammochance_flamebox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "flamebox";
#endif
			A = A + B;
			B = GetConVarInt(hm_ammochance_healbox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "healbox";
			A = A + B;
			B = GetConVarInt(hm_ammochance_multiplebox) * X;
			if (Y >= A && Y < A + B) cvar_hm_ammo_nextbox = "multiplebox";
			A = A + B;
			B = GetConVarInt(hm_ammochance_custombox) * X;
			if (Y >= A && Y < A + B)
			{
				GetConVarString(hm_custombox_name, cvar_custombox_name, sizeof(cvar_custombox_name));
				cvar_hm_ammo_nextbox = cvar_custombox_name;
			}
			A = A + B;
			B = GetConVarInt(hm_ammochance_custombox2) * X;
			if (Y >= A && Y < A + B)
			{
				GetConVarString(hm_custombox_name2, cvar_custombox_name2, sizeof(cvar_custombox_name2));
				cvar_hm_ammo_nextbox = cvar_custombox_name2;
			}
			A = A + B;
			B = GetConVarInt(hm_ammochance_custombox3) * X;
			if (Y >= A && Y < A + B)
			{
				GetConVarString(hm_custombox_name3, cvar_custombox_name3, sizeof(cvar_custombox_name3));
				cvar_hm_ammo_nextbox = cvar_custombox_name3;
			}
			A = A + B;
			B = GetConVarInt(hm_ammochance_custombox4) * X;
			if (Y >= A && Y < A + B)
			{
				GetConVarString(hm_custombox_name4, cvar_custombox_name4, sizeof(cvar_custombox_name4));
				cvar_hm_ammo_nextbox = cvar_custombox_name4;
			}
			A = A + B;
			B = GetConVarInt(hm_ammochance_custombox5) * X;
			if (Y >= A && Y < A + B)
			{
				GetConVarString(hm_custombox_name5, cvar_custombox_name5, sizeof(cvar_custombox_name5));
				cvar_hm_ammo_nextbox = cvar_custombox_name5;
			}
			A = A + B;
			B = GetConVarInt(hm_ammochance_custombox6) * X;
			if (Y >= A && Y < A + B)
			{
				GetConVarString(hm_custombox_name6, cvar_custombox_name6, sizeof(cvar_custombox_name6));
				cvar_hm_ammo_nextbox = cvar_custombox_name6;
			}
			A = A + B;
			B = GetConVarInt(hm_ammochance_custombox7) * X;
			if (Y >= A && Y < A + B)
			{
				GetConVarString(hm_custombox_name7, cvar_custombox_name7, sizeof(cvar_custombox_name7));
				cvar_hm_ammo_nextbox = cvar_custombox_name7;
			}
		}
	}
	if (StrEqual(cvar_hm_ammo_nextbox, "nothing", false)) cvar_hm_ammo_nextbox = "random";
	else if (StrEqual(cvar_hm_ammo_nextbox, "firebox", false))
	{
		RemoveEdict(upgradeid);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
		Fire(position);
	}
	else if (StrEqual(cvar_hm_ammo_nextbox, "boombox", false))
	{
		RemoveEdict(upgradeid);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
		Boom(position);
		Boom(position);
		Boom2(position);
	}
	else if (StrEqual(cvar_hm_ammo_nextbox, "explosionbox", false))
	{
		RemoveEdict(upgradeid);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
		Boom(position);
		Boom2(position);
		Fire(position);
	}
	else if (StrEqual(cvar_hm_ammo_nextbox, "freezebox", false))
	{
		RemoveEdict(upgradeid);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
		Blizzard(client, position);
	}
#if AIRSTRIKE_ENABLED	
	else if (StrEqual(cvar_hm_ammo_nextbox, "hellbox", false))
	{
		RemoveEdict(upgradeid);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
		TimedAirStrike(client);
	}
#endif	
	else if (StrEqual(cvar_hm_ammo_nextbox, "laserbox", false))
	{
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
		ReplaceAmmoWithLaser(upgradeid);
	}
	else if (StrEqual(cvar_hm_ammo_nextbox, "medbox", false))
	{
		RemoveEdict(upgradeid);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
		MedBox(client);
	}
	else if (StrEqual(cvar_hm_ammo_nextbox, "icebox", false))
	{
		RemoveEdict(upgradeid);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
		FreezePlayer(client, position);
	}
	else if (StrEqual(cvar_hm_ammo_nextbox, "witchbox", false))
	{
		RemoveEdict(upgradeid);
		WitchBox(client);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
	}	
	else if (StrEqual(cvar_hm_ammo_nextbox, "panicbox", false))
	{
		RemoveEdict(upgradeid);
		PanicEvent();
		//CheatCMD(client, "director_force_panic_event", "");
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
	}
	else if (StrEqual(cvar_hm_ammo_nextbox, "failbox", false))
	{
		RemoveEdict(upgradeid);
		SetConVarInt(FindConVar("l4d2_loot_g_chance_nodrop"), 105, false, false);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
	}
#if COOP_STATS_ENABLED
	else if (StrEqual(cvar_hm_ammo_nextbox, "pointsbox", false))
	{
		RemoveEdict(upgradeid);
		new cvar_hardmod_pointsbonus = GetConVarInt(hm_ammo_pointsbonus);
		if (GetConVarInt(hm_ammo_pointsbonus_auto) > 0)
		{
			new player_points = RoundToZero(ClientPoints[client] / GetConVarInt(hm_ammo_pointsbonus_auto) * 1.0);
			if (player_points < cvar_hardmod_pointsbonus) player_points = cvar_hardmod_pointsbonus;
			else cvar_hardmod_pointsbonus = player_points;
		}
		AddCachedPoints(client, cvar_hardmod_pointsbonus);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
	}
	else if (StrEqual(cvar_hm_ammo_nextbox, "prizebox", false))
	{
		RemoveEdict(upgradeid);
		PrizeCircle(client, position);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
	}
#endif
#if COOP_AUTOD_ENABLED
	else if (StrEqual(cvar_hm_ammo_nextbox, "bonusbox", false))
	{
		RemoveEdict(upgradeid);
		ChangeCvar("l4d2_loot_g_bonus", GetConVarInt(hm_ammo_loot_bonus), 0, 10);
		ChangeCvar("hm_ammochance_bonusbox", 0, 0, 100);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
	}
#endif
	else if (StrEqual(cvar_hm_ammo_nextbox, "realismbox", false))
	{
		RemoveEdict(upgradeid);
		ChangeCvar("hm_ammochance_realismbox", 0, 0, 100);
		ChangeCvar("sv_disable_glow_survivors", 1, 0, 1);
		ChangeCvar("sv_disable_glow_faritems", 1, 0, 1);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
	}
	else if (StrEqual(cvar_hm_ammo_nextbox, "tankbox", false))
	{
		RemoveEdict(upgradeid);
		ChangeCvar("hm_ammochance_tankbox", 0, 0, 100);
		if (GetConVarInt(hm_ammo_nohumantank) > 0) CheatCMD(GetClient(), "z_spawn", "tank auto");
		else ServerCommand("sm_spawntank %d", GetClientUserId(client));
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
	}
	else if (StrEqual(cvar_hm_ammo_nextbox, "hardbox", false))
	{
		RemoveEdict(upgradeid);
		new cvar_hm_ammo_harddamage = GetConVarInt(hm_ammo_harddamage);
		ChangeCvar("hm_damage_boomer", cvar_hm_ammo_harddamage, 0, 100);
		ChangeCvar("hm_damage_charger", cvar_hm_ammo_harddamage, 0, 100);
		ChangeCvar("hm_damage_hunter", cvar_hm_ammo_harddamage, 0, 100);
		ChangeCvar("hm_damage_jockey", cvar_hm_ammo_harddamage, 0, 100);
		ChangeCvar("hm_damage_smoker", cvar_hm_ammo_harddamage, 0, 100);
		ChangeCvar("hm_damage_spitter1", cvar_hm_ammo_harddamage, 0, 100);
		ChangeCvar("hm_damage_spitter2", cvar_hm_ammo_harddamage, 0, 100);
		ChangeCvar("hm_damage_tank", cvar_hm_ammo_harddamage, 0, 100);
		ChangeCvar("hm_damage_common", cvar_hm_ammo_harddamage, 0, 100);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
	}
	else if (StrEqual(cvar_hm_ammo_nextbox, "bloodbox", false))
	{
		RemoveEdict(upgradeid);
		ServerCommand("sm_gamemode mutation3");
		ChangeCvar("hm_ammochance_bloodbox", 0, 0, 100); // seems broken
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		for (new i = 1; i <= L4D_MAXPLAYERS; i++) if (IsClientInGame(i) && IsPlayerAlive(i) && GetClientTeam(i) == TEAM_SURVIVORS) SwitchHealth(i);
		cvar_hm_ammo_nextbox = "random";
	}
#if FLAME
	else if (StrEqual(cvar_hm_ammo_nextbox, "flamebox", false))
	{
		RemoveEdict(upgradeid);
		IgnitePlayer(client, 30.0);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
	}
#endif
	else if (StrEqual(cvar_hm_ammo_nextbox, "healbox", false))
	{
		RemoveEdict(upgradeid);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
		CreateRingEffect(client, 250, 255, 0, 0, 2.0);
		HealCircle(client, position);
	}
	else if (StrEqual(cvar_hm_ammo_nextbox, "multiplebox", false))
	{
		RemoveEdict(upgradeid);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
		MultipleBox(client);
	}
	else if (StrEqual(cvar_hm_ammo_nextbox, cvar_custombox_name, false))
	{
		RemoveEdict(upgradeid);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
		GetConVarString(hm_custombox_command, cvar_custombox_command, sizeof(cvar_custombox_command));
		new String:cvar_userid[10];
		new String:cvar_user[10];
		new String:cvar_name[MAX_NAME_LENGTH];
		Format(cvar_userid, sizeof(cvar_userid), "%d", GetClientUserId(client));
		Format(cvar_user, sizeof(cvar_user), "%d", client);
		Format(cvar_name, sizeof(cvar_name), "%N", client);
		ReplaceString(cvar_custombox_command, sizeof(cvar_custombox_command), "%userid%", cvar_userid, false);
		ReplaceString(cvar_custombox_command, sizeof(cvar_custombox_command), "%client%", cvar_user, false);
		ReplaceString(cvar_custombox_command, sizeof(cvar_custombox_command), "%username%", cvar_name, false);
		ServerCommand("%s", cvar_custombox_command);
		ServerCommand("hardmod/custombox.cfg");
	}
	else if (StrEqual(cvar_hm_ammo_nextbox, cvar_custombox_name2, false))
	{
		RemoveEdict(upgradeid);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
		GetConVarString(hm_custombox_command2, cvar_custombox_command2, sizeof(cvar_custombox_command2));
		new String:cvar_userid[10];
		new String:cvar_user[10];
		new String:cvar_name[MAX_NAME_LENGTH];
		Format(cvar_userid, sizeof(cvar_userid), "%d", GetClientUserId(client));
		Format(cvar_user, sizeof(cvar_user), "%d", client);
		Format(cvar_name, sizeof(cvar_name), "%N", client);
		ReplaceString(cvar_custombox_command2, sizeof(cvar_custombox_command2), "%userid%", cvar_userid, false);
		ReplaceString(cvar_custombox_command2, sizeof(cvar_custombox_command2), "%client%", cvar_user, false);
		ReplaceString(cvar_custombox_command2, sizeof(cvar_custombox_command2), "%username%", cvar_name, false);
		ServerCommand("%s", cvar_custombox_command2);
		ServerCommand("hardmod/custombox2.cfg");
	}
	else if (StrEqual(cvar_hm_ammo_nextbox, cvar_custombox_name3, false))
	{
		RemoveEdict(upgradeid);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
		GetConVarString(hm_custombox_command3, cvar_custombox_command3, sizeof(cvar_custombox_command3));
		new String:cvar_userid[10];
		new String:cvar_user[10];
		new String:cvar_name[MAX_NAME_LENGTH];
		Format(cvar_userid, sizeof(cvar_userid), "%d", GetClientUserId(client));
		Format(cvar_user, sizeof(cvar_user), "%d", client);
		Format(cvar_name, sizeof(cvar_name), "%N", client);
		ReplaceString(cvar_custombox_command3, sizeof(cvar_custombox_command3), "%userid%", cvar_userid, false);
		ReplaceString(cvar_custombox_command3, sizeof(cvar_custombox_command3), "%client%", cvar_user, false);
		ReplaceString(cvar_custombox_command3, sizeof(cvar_custombox_command3), "%username%", cvar_name, false);
		ServerCommand("%s", cvar_custombox_command3);
		ServerCommand("hardmod/custombox3.cfg");
	}
	else if (StrEqual(cvar_hm_ammo_nextbox, cvar_custombox_name4, false))
	{
		RemoveEdict(upgradeid);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
		GetConVarString(hm_custombox_command4, cvar_custombox_command4, sizeof(cvar_custombox_command4));
		new String:cvar_userid[10];
		new String:cvar_user[10];
		new String:cvar_name[MAX_NAME_LENGTH];
		Format(cvar_userid, sizeof(cvar_userid), "%d", GetClientUserId(client));
		Format(cvar_user, sizeof(cvar_user), "%d", client);
		Format(cvar_name, sizeof(cvar_name), "%N", client);
		ReplaceString(cvar_custombox_command4, sizeof(cvar_custombox_command4), "%userid%", cvar_userid, false);
		ReplaceString(cvar_custombox_command4, sizeof(cvar_custombox_command4), "%client%", cvar_user, false);
		ReplaceString(cvar_custombox_command4, sizeof(cvar_custombox_command4), "%username%", cvar_name, false);
		ServerCommand("%s", cvar_custombox_command4);
		ServerCommand("hardmod/custombox4.cfg");
	}
	else if (StrEqual(cvar_hm_ammo_nextbox, cvar_custombox_name5, false))
	{
		RemoveEdict(upgradeid);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
		GetConVarString(hm_custombox_command5, cvar_custombox_command5, sizeof(cvar_custombox_command5));
		new String:cvar_userid[10];
		new String:cvar_user[10];
		new String:cvar_name[MAX_NAME_LENGTH];
		Format(cvar_userid, sizeof(cvar_userid), "%d", GetClientUserId(client));
		Format(cvar_user, sizeof(cvar_user), "%d", client);
		Format(cvar_name, sizeof(cvar_name), "%N", client);
		ReplaceString(cvar_custombox_command5, sizeof(cvar_custombox_command5), "%userid%", cvar_userid, false);
		ReplaceString(cvar_custombox_command5, sizeof(cvar_custombox_command5), "%client%", cvar_user, false);
		ReplaceString(cvar_custombox_command5, sizeof(cvar_custombox_command5), "%username%", cvar_name, false);
		ServerCommand("%s", cvar_custombox_command5);
		ServerCommand("hardmod/custombox5.cfg");
	}
	else if (StrEqual(cvar_hm_ammo_nextbox, cvar_custombox_name6, false))
	{
		RemoveEdict(upgradeid);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
		GetConVarString(hm_custombox_command6, cvar_custombox_command6, sizeof(cvar_custombox_command6));
		new String:cvar_userid[10];
		new String:cvar_user[10];
		new String:cvar_name[MAX_NAME_LENGTH];
		Format(cvar_userid, sizeof(cvar_userid), "%d", GetClientUserId(client));
		Format(cvar_user, sizeof(cvar_user), "%d", client);
		Format(cvar_name, sizeof(cvar_name), "%N", client);
		ReplaceString(cvar_custombox_command6, sizeof(cvar_custombox_command6), "%userid%", cvar_userid, false);
		ReplaceString(cvar_custombox_command6, sizeof(cvar_custombox_command6), "%client%", cvar_user, false);
		ReplaceString(cvar_custombox_command6, sizeof(cvar_custombox_command6), "%username%", cvar_name, false);
		ServerCommand("%s", cvar_custombox_command6);
		ServerCommand("hardmod/custombox6.cfg");
	}
	else if (StrEqual(cvar_hm_ammo_nextbox, cvar_custombox_name7, false))
	{
		RemoveEdict(upgradeid);
		PrintHintTextToAll("%N have found a %s!", client, cvar_hm_ammo_nextbox);
		cvar_hm_ammo_nextbox = "random";
		GetConVarString(hm_custombox_command7, cvar_custombox_command7, sizeof(cvar_custombox_command7));
		new String:cvar_userid[10];
		new String:cvar_user[10];
		new String:cvar_name[MAX_NAME_LENGTH];
		Format(cvar_userid, sizeof(cvar_userid), "%d", GetClientUserId(client));
		Format(cvar_user, sizeof(cvar_user), "%d", client);
		Format(cvar_name, sizeof(cvar_name), "%N", client);
		ReplaceString(cvar_custombox_command7, sizeof(cvar_custombox_command7), "%userid%", cvar_userid, false);
		ReplaceString(cvar_custombox_command7, sizeof(cvar_custombox_command7), "%client%", cvar_user, false);
		ReplaceString(cvar_custombox_command7, sizeof(cvar_custombox_command7), "%username%", cvar_name, false);
		ServerCommand("%s", cvar_custombox_command7);
		ServerCommand("hardmod/custombox7.cfg");
	}
	else if (StrEqual(cvar_hm_ammo_nextbox, "nextbox", false))
	{
		RemoveEdict(upgradeid);
		new switch_a = 2;
#if AIRSTRIKE_ENABLED
		switch_a = 1;
#endif
		switch(GetRandomInt(switch_a, 17))
		{
			case 1: cvar_hm_ammo_nextbox = "hellbox";
			case 2: cvar_hm_ammo_nextbox = "firebox";
			case 3: cvar_hm_ammo_nextbox = "boombox";
			case 4: cvar_hm_ammo_nextbox = "nextbox";
			case 5: cvar_hm_ammo_nextbox = "laserbox";
			case 6: cvar_hm_ammo_nextbox = "medbox";
			case 7: cvar_hm_ammo_nextbox = "icebox";
			case 8: cvar_hm_ammo_nextbox = "witchbox";
			case 9: cvar_hm_ammo_nextbox = "panicbox";
			case 10: cvar_hm_ammo_nextbox = "freezebox";
			case 11: cvar_hm_ammo_nextbox = "bloodbox";
			case 12: cvar_hm_ammo_nextbox = "flamebox";
			case 13: cvar_hm_ammo_nextbox = "healbox";
			case 14: cvar_hm_ammo_nextbox = "prizebox";
			case 15: cvar_hm_ammo_nextbox = "explosionbox";
			case 16: cvar_hm_ammo_nextbox = "multiplebox";
			case 17: cvar_hm_ammo_nextbox = "pointsbox";
		}
		PrintHintTextToAll("%N have found a nextbox (%s)!", client, cvar_hm_ammo_nextbox);
	}
	else cvar_hm_ammo_nextbox = "random";
	SetConVarString(hm_ammo_nextbox, cvar_hm_ammo_nextbox);
}

#if REMOVE_AMMO
public Action:Event_AmmoPlayerUse(Handle:event, const String:name[], bool:dontBroadcast)
{
	new targetid = GetEventInt(event, "targetid");
	new String:target_name[50];
	GetEdictClassname(targetid, target_name, 50);
	if (StrEqual(target_name, "weapon_ammo_spawn"))
	{
		new client = GetClientOfUserId(GetEventInt(event, "userid"));
		PlayersLastAmmoUsed[client] = targetid;
	}
}

public Action:Event_AmmoPickup(Handle:event, const String:name[], bool:dontBroadcast)
{
	new client = GetClientOfUserId(GetEventInt(event, "userid"));
	if (GetRandomInt(1, 100) < GetConVarInt(hm_ammo_removechance))
	{
		new targetid = GetEventInt(event, "targetid");
		CreateTimer(0.1, TimedRemoveEdict, targetid);
		RemoveEntity(PlayersLastAmmoUsed[client]);
	}
}
#endif

public ChangeCvar(const String:cvar_name[], cvar_value, cvar_value_min, cvar_value_max)
{
	new Handle:gamecvar = FindConVar(cvar_name);
	if (gamecvar == INVALID_HANDLE)
	{
		return;
	}
	new gamecvar_value = GetConVarInt(gamecvar);
	if (gamecvar_value + cvar_value < cvar_value_min) gamecvar_value = cvar_value_min;
	else if (gamecvar_value + cvar_value > cvar_value_max) gamecvar_value = cvar_value_max;
	else gamecvar_value = gamecvar_value + cvar_value;
	SetConVarInt(gamecvar, gamecvar_value, false, false);
}

#if AIRSTRIKE_ENABLED
public TimedAirStrike(client)
{
	g_bRing = true;
	CreateTimer(0.1, timerRing, client, TIMER_REPEAT);
	CreateTimer(6.0, timerRingTimeout);	
	CreateTimer(6.0, timerStartStrike, client);
}

public Action:timerStartStrike(Handle:timer, any:client)
{
	Airstrike(client);
	return Plugin_Continue;
}

public Action:timerRing(Handle:timer, any:client)
{ // CreateRingEffect(client, colRed, colGre, colBlu, alpha, Float:width, Float:duration)
	if (!g_bRing)
	{
//		PrintToChatAll("Action:timerRing() :: STOP");
		return Plugin_Stop;
	}
//	new Float:position[3];
//	GetEntPropVector(client, Prop_Send, "m_vecOrigin", position);
//	position[2] += 10;
	CreateRingEffect(client, 250, 0, 0, 255, 2.0);
	new Float:vec[3];
	GetClientAbsOrigin(client, vec);
	vec[2] += 10;
	EmitAmbientSound(SOUND_BLIP, vec, client, SNDLEVEL_RAIDSIREN);	
	return Plugin_Continue;
}

public Action:timerRingTimeout(Handle:timer)
{
	g_bRing = false;
}
#endif

public ReplaceAmmoWithLaser(entity)
{
	new LaserEntity = CreateEntityByName("upgrade_laser_sight");
	if (LaserEntity == -1)
	{
		return;
	}
	new Float:vecOrigin[3];
	new Float:angRotation[3];
	GetEntPropVector(entity, Prop_Send, "m_vecOrigin", vecOrigin);
	GetEntPropVector(entity, Prop_Send, "m_angRotation", angRotation);		
	RemoveEdict(entity);
	TeleportEntity(LaserEntity, vecOrigin, angRotation, NULL_VECTOR);
	DispatchSpawn(LaserEntity);
}

public MedBox(client)
{
	new ItemNumber;
	new String:ItemName[36];
	for (new i = 0; i < GetConVarInt(hm_ammo_medbox_count); i++)
	{
		ItemNumber = GetRandomInt(1, 4);
		switch (ItemNumber)
		{
			case 1: ItemName = "weapon_defibrillator";
			case 2: ItemName = "weapon_first_aid_kit";
			case 3: ItemName = "weapon_pain_pills";
			case 4: ItemName = "weapon_adrenaline";
		}
		SpawnItem(client, ItemName);
	}
}

public MultipleBox(client)
{
	new ItemNumber;
	new String:ItemName[36];
	for (new i = 0; i < GetConVarInt(hm_ammo_multipleboxes); i++)
	{
		ItemNumber = GetRandomInt(1, 2);
		switch (ItemNumber)
		{
			case 1: ItemName = "weapon_upgradepack_explosive";
			case 2: ItemName = "weapon_upgradepack_incendiary";
		}
		SpawnItem(client, ItemName);
	}
}

stock GetSpecialAmmoInPlayerGun(client) //returns the amount of special rounds in your gun
{
	if (!client) return 0;
	new gunent = GetPlayerWeaponSlot(client, 0);
	if (IsValidEdict(gunent)) return GetEntProp(gunent, Prop_Send, "m_nUpgradedPrimaryAmmoLoaded", 1);
	return 0;
}

SetSpecialAmmoInPlayerGun(client, amount)
{
	if (!client) return;
	new gunent = GetPlayerWeaponSlot(client, 0);
	if (IsValidEdict(gunent)) SetEntProp(gunent, Prop_Send, "m_nUpgradedPrimaryAmmoLoaded", amount, 1);
}

WitchBox(client)
{
	new flags = GetCommandFlags("z_spawn");
	SetCommandFlags("z_spawn", flags & ~FCVAR_CHEAT);
	for (new i = 1; i < GetConVarInt(hm_ammo_witches); i++)
	{
		FakeClientCommand(client, "z_spawn witch auto");
	}
	SetCommandFlags("z_spawn", flags);
}