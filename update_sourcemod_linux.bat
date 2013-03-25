@echo off

copy sourcemod_linux\bin\sourcemod.2.l4d2.so server_files_linux\addons\sourcemod\bin\ /Y
copy sourcemod_linux\bin\sourcemod.logic.so server_files_linux\addons\sourcemod\bin\ /Y
copy sourcemod_linux\bin\sourcemod_mm_i486.so server_files_linux\addons\sourcemod\bin\ /Y
copy sourcemod_linux\bin\sourcepawn.jit.x86.so server_files_linux\addons\sourcemod\bin\ /Y

copy sourcemod_linux\extensions\bintools.ext.so server_files_linux\addons\sourcemod\extensions\ /Y
copy sourcemod_linux\extensions\clientprefs.ext.so server_files_linux\addons\sourcemod\extensions\ /Y
copy sourcemod_linux\extensions\dbi.mysql.ext.so server_files_linux\addons\sourcemod\extensions\ /Y
copy sourcemod_linux\extensions\dbi.sqlite.ext.so server_files_linux\addons\sourcemod\extensions\ /Y
copy sourcemod_linux\extensions\geoip.ext.so server_files_linux\addons\sourcemod\extensions\ /Y
copy sourcemod_linux\extensions\regex.ext.so server_files_linux\addons\sourcemod\extensions\ /Y
copy sourcemod_linux\extensions\sdktools.ext.2.l4d2.so server_files_linux\addons\sourcemod\extensions\ /Y
copy sourcemod_linux\extensions\topmenus.ext.so server_files_linux\addons\sourcemod\extensions\ /Y
copy sourcemod_linux\extensions\updater.ext.so server_files_linux\addons\sourcemod\extensions\ /Y
copy sourcemod_linux\extensions\webternet.ext.so server_files_linux\addons\sourcemod\extensions\ /Y

copy sourcemod_linux\gamedata\ server_files_basic\addons\sourcemod\gamedata\ /Y

copy sourcemod_linux\plugins\admin-flatfile.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_linux\plugins\adminhelp.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_linux\plugins\adminmenu.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_linux\plugins\basebans.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_linux\plugins\basechat.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_linux\plugins\basecomm.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_linux\plugins\basecommands.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_linux\plugins\basetriggers.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_linux\plugins\basevotes.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_linux\plugins\clientprefs.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_linux\plugins\funcommands.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_linux\plugins\funvotes.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_linux\plugins\playercommands.smx server_files_basic\addons\sourcemod\plugins\ /Y
copy sourcemod_linux\plugins\sounds.smx server_files_basic\addons\sourcemod\plugins\ /Y

copy GeoIP\GeoIP.dat server_files_basic\addons\sourcemod\configs\geoip\ /Y