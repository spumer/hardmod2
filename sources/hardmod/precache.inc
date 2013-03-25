public PrecacheOnPluginStart()
{
	Version += 1;
	RegAdminCmd("sm_precacheallmodels", Command_PrecacheAllModels, ADMFLAG_RCON, "sm_precacheallmodels");
}

public Action:Command_PrecacheAllModels(client, args)
{
	PrecacheAllItems();
}

public CheckPrecacheModel(const String:Model[])
{
	if (!IsModelPrecached(Model))
	{
		PrecacheModel(Model);
	}
}

public PrecacheHealth()
{
	CheckPrecacheModel("models/w_models/weapons/w_eq_Medkit.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_eq_defibrillator.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_eq_painpills.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_eq_adrenaline.mdl");
}	

public PrecacheMeleeWeapons()
{
	CheckPrecacheModel("models/weapons/melee/w_cricket_bat.mdl");
	CheckPrecacheModel("models/weapons/melee/w_crowbar.mdl");
	CheckPrecacheModel("models/weapons/melee/w_electric_guitar.mdl");
	CheckPrecacheModel("models/weapons/melee/w_chainsaw.mdl");
	CheckPrecacheModel("models/weapons/melee/w_katana.mdl");
	CheckPrecacheModel("models/weapons/melee/w_machete.mdl");
	CheckPrecacheModel("models/weapons/melee/w_tonfa.mdl");
	CheckPrecacheModel("models/weapons/melee/w_frying_pan.mdl");
	CheckPrecacheModel("models/weapons/melee/w_fireaxe.mdl");
	CheckPrecacheModel("models/w_models/Weapons/w_bat.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_knife_t.mdl");
	CheckPrecacheModel("models/weapons/melee/w_golfclub.mdl");
//	CheckPrecacheModel("models/weapons/melee/w_riotshield.mdl");
}

public PrecacheWeapons()
{
	CheckPrecacheModel("models/w_models/weapons/w_pistol_B.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_desert_eagle.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_smg_uzi.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_smg_a.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_shotgun.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_pumpshotgun_A.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_shotgun_spas.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_autoshot_m4super.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_sniper_military.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_sniper_mini14.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_rifle_m16a2.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_desert_rifle.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_rifle_ak47.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_m60.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_smg_mp5.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_sniper_scout.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_sniper_awp.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_rifle_sg552.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_grenade_launcher.mdl");
}

public PrecacheThrowWeapons()
{
	CheckPrecacheModel("models/w_models/weapons/w_eq_pipebomb.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_eq_molotov.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_eq_bile_flask.mdl");
}

public PrecacheAmmoPacks()
{
	CheckPrecacheModel("models/w_models/weapons/w_eq_explosive_ammopack.mdl");
	CheckPrecacheModel("models/w_models/weapons/w_eq_incendiary_ammopack.mdl");
}

public PrecacheMisc()
{
	CheckPrecacheModel("models/props_junk/explosive_box001.mdl");
	CheckPrecacheModel("models/props_junk/gascan001a.mdl");
	CheckPrecacheModel("models/props_equipment/oxygentank01.mdl");
	CheckPrecacheModel("models/props_junk/propanecanister001a.mdl");
}

public PrecacheSurvivors()
{
	CheckPrecacheModel("models/survivors/survivor_gambler.mdl");
	CheckPrecacheModel("models/survivors/survivor_manager.mdl");
	CheckPrecacheModel("models/survivors/survivor_coach.mdl");
	CheckPrecacheModel("models/survivors/survivor_producer.mdl");
	CheckPrecacheModel("models/survivors/survivor_teenangst.mdl");
	CheckPrecacheModel("models/survivors/survivor_biker.mdl");
	CheckPrecacheModel("models/survivors/survivor_namvet.mdl");
	CheckPrecacheModel("models/survivors/survivor_mechanic.mdl");
	
	CheckPrecacheModel("models/infected/witch.mdl");
	CheckPrecacheModel("models/infected/witch_bride.mdl");
	
//	CheckPrecacheModel("");
}

public PrecacheSounds()
{
//	PrecacheSound("ambient/atmosphere/firewerks_launch_04.wav", true);
}

public PrecacheAllItems()
{
	PrecacheSurvivors();
	PrecacheHealth();
	PrecacheMeleeWeapons();
	PrecacheWeapons();
	PrecacheThrowWeapons();
	PrecacheAmmoPacks();
	PrecacheMisc();
	PrecacheSounds();
}