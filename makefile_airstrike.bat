@echo off
setlocal
pushd %~dp0

set "PLUGIN_SRC_DIR=sources"
set "SPCOMP=bin\sourcepawn\spcomp.exe"
set "PLUGIN_SRC_DIR=sources"
set "BUILD_DEST_DIR=build"
set "RELEASE_DIR=build\"
set "SPCOMP=bin\sourcepawn\spcomp.exe"


@echo on
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d2_airstrike.core.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d2_airstrike.tank.sp"
"%SPCOMP%" -O2 -D%PLUGIN_SRC_DIR% l4d2_airstrike.triggers.sp"
@echo off

move /Y "%PLUGIN_SRC_DIR%\l4d2_airstrike.core.smx" "%RELEASE_DIR%\l4d2_airstrike.core.smx"
move /Y "%PLUGIN_SRC_DIR%\l4d2_airstrike.tank.smx" "%RELEASE_DIR%\l4d2_airstrike.tank.smx"
move /Y "%PLUGIN_SRC_DIR%\l4d2_airstrike.triggers.smx" "%RELEASE_DIR%\l4d2_airstrike.triggers.smx"

popd
endlocal
