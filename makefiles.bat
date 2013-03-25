@echo off
setlocal
pushd %~dp0

set "PLUGIN_SRC_DIR=sources"
set "SPCOMP=bin\sourcepawn\spcomp.exe"

@echo on
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% ACS_Classic.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% advertisements.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% coop_human_tank.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% fix_Sappy2.0.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d_counters.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d_gamemode_scripts.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d_gear_transfer.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d_itemsspawnremover.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d_kickloadstuckers.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d_map_scripts.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d_powerups_rush.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d_stoptk.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d_superboss_en.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d2_boomerbitchslap.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d2_BWDefib.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d2_charger_steering.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d2_custom_commands.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d2_events.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d2_gnome.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d2_knife_unlock.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d2_loot.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d2_monsterbots.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4dmultislots.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% L4DRestrictedZones.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% permamute.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% rcon_lock.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% rp_tools.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% sm_guardian.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% steam-group-admins-http.sp"
@echo off

move /Y "%PLUGIN_SRC_DIR%\*.smx" "release_temp\addons\sourcemod\plugins\" 

call makefile_hardmod.bat
call makefile_l4d_healthglow.bat

move /Y "build\hardmod_nostats.smx" "release_temp\hardmod_nostats\hardmod.smx"
move /Y "build\hardmod.smx" "release_temp\addons\sourcemod\plugins\"
move /Y "build\l4d_healthglow.smx" "release_temp\addons\sourcemod\plugins\"

popd
endlocal
