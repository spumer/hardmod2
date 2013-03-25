/* (c) Infected Spawn API by V10 aka maldersoft (http://maldersoft.spb.ru)
* This API written for l4d2 plugin coders to correct spawn infected bosses with more options (owner,ghost, startorigin, etc) 
* Also plugin implement easy utils functions (stocks)
*/

#include <sourcemod>
#include <sdktools>

#define InfectedApiVersion 1.1

/* ChangeLog:
 * 1.1
 * - added function GetInfectedCounts
 * - added function GetMaxInfectedCounts
 * 1.0
 * - Initial release
 * 
 */
 
#define TEAM_SURVIVOR 2
#define TEAM_INFECTED 3

#define ZC_SMOKER 1
#define ZC_BOOMER 2
#define ZC_HUNTER 3
#define ZC_SPITTER 4
#define ZC_JOCKEY 5
#define ZC_CHARGER 6
#define ZC_TANK 7

new String:g_sBossNames[8][10]={"","smoker","boomer","hunter","spitter","jockey","charger","tank"};
new g_iCTPRentindex = -1;
new g_InfectedApiInitialized=false;

/**
* Initialize Infected API.
*
* @return		True if successfully initialized, false otherwise.
* 
*/
stock InitializeInfectedApi(){
	for (new i = 0; i <= GetMaxEntities(); i++){
		if (!IsValidEntity(i)) continue	;
		decl String:netclass[64];
		GetEntityNetClass(i, netclass, sizeof(netclass));
		if (!StrEqual(netclass, "CTerrorPlayerResource")) continue;
		
		g_iCTPRentindex = i;
		break;
	}		
	g_InfectedApiInitialized=true;
	#if InfectedAPIDebug
	PrintToServer("Infected API Initialized!");
	#endif
	return true;
}

/**
* Spawn the specified zombie boss
*
* @param client				Client index.
* @param Class				Class of boss (see constants ZC_ in top of this file)
* @param fGhost				True = spawned as ghost, False= spawned as alived..
* @param fAuto				Use director auto position? True = use, False = spawn in eye direction (if param Origin not set)
* @param fGhostFinale 		True = spawn ghost in finale also, False = spawn alived in Finale (ignore fGhost)
* @param Origin 			Set position new zombie
* @param Angles 			Set angles new zombie
* @return					True if successfully spawn, false otherwise.
* @error			Invalid handle, or bounds error.
* 					Invalid infected class
* 					Client is alive
* 					Client is not in game
* 					Client is not infected team
*/

stock SpawnInfectedBoss(any:client,any:Class,fGhost=false, fAuto=true, fGhostFinale=false, Float:Origin[3]={0.0,0.0,0.0}, Float:Angles[3]={0.0,0.0,0.0})
{
	new bool:resetGhostState[MAXPLAYERS+1];
	new bool:resetIsAlive[MAXPLAYERS+1];
	new bool:resetLifeState[MAXPLAYERS+1];
	new String:options[30];
	#if InfectedAPIDebug
	PrintToServer("Spawn zombie: type=%s",g_sBossNames[Class]);
	#endif
	if (Class<ZC_SMOKER || Class>ZC_TANK) return false;
	if (GetClientTeam(client) != TEAM_INFECTED) return false;
	if (!IsClientInGame(client)) return false;
	if (IsPlayerAlive(client)) return false;
	if (!g_InfectedApiInitialized) InitializeInfectedApi();
	
	for (new i=1; i<=MaxClients; i++){ 
		if (i == client) continue; //dont disable the chosen one
		if (!IsClientInGame(i)) continue; //not ingame? skip
		if (GetClientTeam(i) != TEAM_INFECTED) continue; //not infected? skip
		if (IsFakeClient(i)) continue; //a bot? skip
		
		if (IsPlayerGhost(i)){
			resetGhostState[i] = true;
			SetPlayerGhostStatus(i, false);
			resetIsAlive[i] = true;
			SetPlayerIsAlive(i, true);
		}
		else if (!IsPlayerAlive(i)){
			resetLifeState[i] = true;
			SetPlayerLifeState(i, false)
		}
	}
	//spawn zombie
	Format(options,30,"%s%s",g_sBossNames[Class],(fAuto?" auto":""));
	CheatCommand(client, "z_spawn",options );
	
	#if InfectedAPIDebug
	PrintToServer("Spawn zombie: options=%s",options);
	#endif
	
	// We restore the player's status
	for (new i=1; i<=MaxClients; i++){
		if (resetGhostState[i]) SetPlayerGhostStatus(i, true);
		if (resetIsAlive[i]) SetPlayerIsAlive(i, false);
		if (resetLifeState[i]) SetPlayerLifeState(i, true);
	}
	
	if (Origin[0]!=0.0){
		TeleportEntity(client, Origin, Angles, {0.0,0.0,0.0})
	}
	
	if (fGhost && Class!=ZC_TANK && (!IsFinale() || fGhostFinale)) InitializeAsGhost(client,true);
	
	return true;
}
/**
* Try make infected ghost
* 
* @param client	Client index.
* @param SavePos	True = save position, False = spawn in director pos
* @return			True if successfully initialized, false otherwise.
* @error			Invalid handle, or bounds error.
* 					Client is alive
* 					Client is already ghost
* 					Client is not in game
* 					Client is not infected team
*/

stock InitializeAsGhost(client, SavePos=false)
{
	if (!IsClientInGame(client)) return false;
	if (GetClientTeam(client) != 3) return false;
	if (!IsPlayerAlive(client)) return false;
	if (IsPlayerGhost(client)) return false;
	
	new Handle:datapack=CreateDataPack();
	WritePackCell(datapack,client);
	WritePackCell(datapack,SavePos);
	if (SavePos){
		new Float:AbsOrigin[3];
		new Float:EyeAngles[3];
		new Float:Velocity;
		GetClientAbsOrigin(client, AbsOrigin);
		WritePackFloat(datapack,AbsOrigin[0]);
		WritePackFloat(datapack,AbsOrigin[1]);
		WritePackFloat(datapack,AbsOrigin[2]);
		GetClientEyeAngles(client, EyeAngles);
		WritePackFloat(datapack,EyeAngles[0]);
		WritePackFloat(datapack,EyeAngles[1]);
		WritePackFloat(datapack,EyeAngles[2]);
		Velocity = GetEntPropFloat(client, Prop_Send, "m_vecVelocity[0]");
		WritePackFloat(datapack,Velocity);
		Velocity = GetEntPropFloat(client, Prop_Send, "m_vecVelocity[1]");
		WritePackFloat(datapack,Velocity);
		Velocity = GetEntPropFloat(client, Prop_Send, "m_vecVelocity[2]");	
		WritePackFloat(datapack,Velocity);
	}
	return CreateTimer(0.1,subInitializeAsGhost,datapack,TIMER_REPEAT);
}

/**
* Get all infected boss counts
* 
* @return			Array longs from ZC_SMOKER to ZC_CHARGER (Array[ZC_TANK])
* Notice:			This function not return tank count
*/
stock GetInfectedCounts(){
	new ZombieCounts[ZC_TANK];
	for (new i=1; i<=MaxClients; i++){ 
		if (!IsClientInGame(i)) continue; //not ingame? skip
		if (IsFakeClient(i)) continue; //a bot? skip
		if (GetClientTeam(i) != 3) continue; //not infected? skip
		if (!IsPlayerAlive(i)) continue;
		new ZClass = GetEntProp(i, Prop_Send, "m_zombieClass");
		if (ZClass!=ZC_TANK) ZombieCounts[ZClass]++;
	}
	return ZombieCounts;
}

/**
* Get MAX all infected boss counts
* 
* @return			Array longs from ZC_SMOKER to ZC_CHARGER (Array[ZC_TANK])
* Notice:			This function not return tank count
*/
stock GetMaxInfectedCounts(){
	new ZombieCounts[ZC_TANK];
	for (new i=ZC_SMOKER; i<=ZC_CHARGER; i++){ 
		new String:CVarName[50];
		Format(CVarName,50,"z_versus_%s_limit",g_sBossNames[i]);
		new Handle:cvar=FindConVar(CVarName);
		ZombieCounts[i]=GetConVarInt(cvar);
	}
	return ZombieCounts;
}

stock SetPlayerIsAlive(client, bool:alive)
{
	new offset = FindSendPropInfo("CTransitioningPlayer", "m_isAlive");
	if (alive) SetEntData(client, offset, 1, 1, true);
	else SetEntData(client, offset, 0, 1, true);
}


stock bool:IsPlayerGhost(client)
{
	if (GetEntProp(client, Prop_Send, "m_isGhost", 1)) return true;
	return false;
}

stock SetPlayerGhostStatus(client, bool:ghost)
{
	if(ghost){	
		SetEntProp(client, Prop_Send, "m_isGhost", 1, 1);
		SetEntityMoveType(client, MOVETYPE_ISOMETRIC)
	}else{
		SetEntProp(client, Prop_Send, "m_isGhost", 0, 1);
		SetEntityMoveType(client, MOVETYPE_WALK)
	}
}

stock SetPlayerLifeState(client, bool:ready)
{
	if (ready) SetEntProp(client, Prop_Data, "m_lifeState", 1, 1);
	else SetEntProp(client, Prop_Data, "m_lifeState", 0, 1);
}

bool:IsFinale()
{
	if (!g_InfectedApiInitialized) InitializeInfectedApi();
	//If we found the CTerrorPlayerResource index,
	if (g_iCTPRentindex != -1)
	{	
		//check if we're in a finale.
		if(GetEntProp(g_iCTPRentindex,Prop_Send,"m_isFinale")) return true
	}
	return false
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

public Action:subInitializeAsGhost(Handle:timer,Handle:datapack)
{
	ResetPack(datapack); //reset the reading position inside our datapack
	new client=ReadPackCell(datapack);
	#if InfectedAPIDebug
	PrintToServer("Spawn zombie: subInitializeAsGhost");
	#endif
	if (IsPlayerGhost(client)){
		ClientCommand(client,"-use");		
		SetEntProp(client,Prop_Send,"m_isCulling",0,1);
		new SavePos=ReadPackCell(datapack);
		if (SavePos){
			new Float:AbsOrigin[3];
			new Float:EyeAngles[3];
			new Float:Velocity[3];
			AbsOrigin[0]=ReadPackFloat(datapack);
			AbsOrigin[1]=ReadPackFloat(datapack);
			AbsOrigin[2]=ReadPackFloat(datapack);
			EyeAngles[0]=ReadPackFloat(datapack);
			EyeAngles[1]=ReadPackFloat(datapack);
			EyeAngles[2]=ReadPackFloat(datapack);
			Velocity[0] = ReadPackFloat(datapack);
			Velocity[1] = ReadPackFloat(datapack);
			Velocity[2] = ReadPackFloat(datapack);
			//Teleport the player back,
			TeleportEntity(client, AbsOrigin, EyeAngles, Velocity);			
		}
		CloseHandle(datapack);
		return Plugin_Stop;
	}else{
		SetEntProp(client,Prop_Send,"m_isCulling",1,1);
		ClientCommand(client,"+use");
		return Plugin_Continue;
	}
}
