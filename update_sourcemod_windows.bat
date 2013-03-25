@echo off

copy sourcemod_windows\bin\sourcemod.2.l4d2.dll server_files_windows\addons\sourcemod\bin\ /Y
copy sourcemod_windows\bin\sourcemod.logic.dll server_files_windows\addons\sourcemod\bin\ /Y
copy sourcemod_windows\bin\sourcemod_mm.dll server_files_windows\addons\sourcemod\bin\ /Y
copy sourcemod_windows\bin\sourcepawn.jit.x86.dll server_files_windows\addons\sourcemod\bin\ /Y

copy sourcemod_windows\extensions\bintools.ext.dll server_files_windows\addons\sourcemod\extensions\ /Y
copy sourcemod_windows\extensions\clientprefs.ext.dll server_files_windows\addons\sourcemod\extensions\ /Y
copy sourcemod_windows\extensions\dbi.mysql.ext.dll server_files_windows\addons\sourcemod\extensions\ /Y
copy sourcemod_windows\extensions\dbi.sqlite.ext.dll server_files_windows\addons\sourcemod\extensions\ /Y
copy sourcemod_windows\extensions\geoip.ext.dll server_files_windows\addons\sourcemod\extensions\ /Y
copy sourcemod_windows\extensions\regex.ext.dll server_files_windows\addons\sourcemod\extensions\ /Y
copy sourcemod_windows\extensions\sdktools.ext.2.l4d2.dll server_files_windows\addons\sourcemod\extensions\ /Y
copy sourcemod_windows\extensions\topmenus.ext.dll server_files_windows\addons\sourcemod\extensions\ /Y
copy sourcemod_windows\extensions\updater.ext.dll server_files_windows\addons\sourcemod\extensions\ /Y
copy sourcemod_windows\extensions\webternet.ext.dll server_files_windows\addons\sourcemod\extensions\ /Y

copy sourcemod_windows\gamedata\ server_files_basic\addons\sourcemod\gamedata\ /Y

copy sourcemod_windows\plugins\admin-flatfile.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_windows\plugins\adminhelp.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_windows\plugins\adminmenu.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_windows\plugins\basebans.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_windows\plugins\basechat.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_windows\plugins\basecomm.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_windows\plugins\basecommands.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_windows\plugins\basetriggers.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_windows\plugins\basevotes.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_windows\plugins\clientprefs.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_windows\plugins\funcommands.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_windows\plugins\funvotes.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_windows\plugins\playercommands.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_windows\plugins\sounds.smx server_files_basic\addons\sourcemod\plugins\ /Y

copy sourcemod_windows\scripting\include\ sources\include\ /Y
copy sourcemod_windows\scripting\include\ bin\sourcepawn\include\ /Y
copy sourcemod_windows\scripting\compile.* bin\sourcepawn\ /Y
copy sourcemod_windows\scripting\spcomp.exe bin\sourcepawn\ /Y

copy GeoIP\GeoIP.dat server_files_basic\addons\sourcemod\configs\geoip\ /Y