@echo off
setlocal
pushd %~dp0

set "PLUGIN_SRC_DIR=src"
set "PLUGIN_NAME=main"
set "BUILD_DEST_DIR=build"
set "RELEASE_NAME=build\l4d2healthglow"
set "SPCOMP=%SOURCEMOD%\..\scripting\spcomp.exe"

echo --------------------------------------------
echo Building %PLUGIN_NAME%.sp ...
echo --------------------------------------------

if not exist %BUILD_DEST_DIR% (
mkdir %BUILD_DEST_DIR%
) 

"%SPCOMP%" -D%PLUGIN_SRC_DIR% "%PLUGIN_NAME%.sp"
move /Y "%PLUGIN_SRC_DIR%\%PLUGIN_NAME%.smx" "%RELEASE_NAME%.smx" 

echo --------------------------------------------
echo Built %PLUGIN_NAME%.sp as %RELEASE_NAME%.smx
echo --------------------------------------------

popd
endlocal