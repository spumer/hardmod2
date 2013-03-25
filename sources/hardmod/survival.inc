new bool:round_started;

public SurvivalOnPluginStart()
{
	HookEvent("survival_round_start", Event_SurvRoundStart);
	HookEvent("round_end", Event_SurvRoundEnd);
}

public Action:Event_SurvRoundStart(Handle:event, const String:name[], bool:dontBroadcast)
{
	round_started = true;
}

public Action:Event_SurvRoundEnd(Handle:event, const String:name[], bool:dontBroadcast)
{
	if (round_started)
	{
		rounds++;
	}
	round_started = false;
}