new Handle:IsPanicEnabled;
new Handle:IsPanicCycleEnabled;
new bool:panicflag = false;

public PanicOnPluginStart()
{
	IsPanicEnabled = CreateConVar("hm_panic", "0", "", FCVAR_PLUGIN);
	IsPanicCycleEnabled = CreateConVar("hm_panic_cycle", "0", "", FCVAR_PLUGIN);
	HookConVarChange(IsPanicEnabled, PanicEnabledChanged);
	
	HookEvent("create_panic_event", PanicEvent_CreatePanicEvent, EventHookMode_Post);
	HookEvent("panic_event_finished", PanicEvent_PanicEventFinished, EventHookMode_Post);
	HookEvent("round_start", PanicEvent_RoundStart, EventHookMode_Post);
}

public PanicEnabledChanged(Handle:hVariable, const String:strOldValue[], const String:strNewValue[])
{
	if (GetConVarInt(IsPanicEnabled) > 0)
	{
		SetConVarInt(FindConVar("director_panic_forever"), 1, false, false);
		if (!panicflag)
		{
			CheatCMD(GetAnyClient(), "director_force_panic_event", "");
		}
	}
	else
	{
		SetConVarInt(FindConVar("director_panic_forever"), 0, false, false);
	}
}

public Action:PanicEvent_CreatePanicEvent(Handle:event, const String:name[], bool:dontBroadcast)
{
	PrintToAdmins("Action:PanicEvent_CreatePanicEvent()");
	panicflag = true;
}

public Action:PanicEvent_PanicEventFinished(Handle:event, const String:name[], bool:dontBroadcast)
{
	PrintToAdmins("Action:PanicEvent_PanicEventFinished()");
	panicflag = false;
	if (GetConVarInt(IsPanicEnabled) > 0 && GetConVarInt(IsPanicCycleEnabled)) 
	{
		CreateTimer(1.0, CreatePanicEvent);
	}
}

public Action:PanicEvent_RoundStart(Handle:event, const String:name[], bool:dontBroadcast)
{
	PrintToAdmins("Action:PanicEvent_RoundStart()");
	panicflag = false;
}

public Action:CreatePanicEvent(Handle:timer)
{
	PrintToAdmins("Action:CreatePanicEvent(), panicflag(%d)", panicflag);
	if (!panicflag)
	{
		CheatCMD(GetAnyClient(), "director_force_panic_event", "");
		if (GetConVarInt(IsPanicCycleEnabled) > 0)
		{
			CreateTimer(1.0, CreatePanicEvent);
		}
	}
}