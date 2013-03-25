#if defined _f18airstrike_included_
  #endinput
#endif
#define _f18airstrike_included_


/**
 * Creates an F-18 Airstrike at the location and from the direction specified.
 *
 * @param origin[0]    Float: origin vector index 0.
 * @param origin[1]    Float: origin vector index 1.
 * @param origin[2]    Float: origin vector index 2.
 * @param direction    Float: angle direction for the jet to appear.
 * @return             none.
 */
native F18_ShowAirstrike(Float:origin0, Float:origin1, Float:origin2, Float:direction);


/** 0=Off, 1=On. */
forward F18_OnPluginState(pluginstate);


/** 0=Started, 1=Stopped. */
forward F18_OnRoundState(roundstate);