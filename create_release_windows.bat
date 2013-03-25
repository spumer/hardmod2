@echo on
setlocal
pushd %~dp0

REM temporary directory that will hold the release files prior to zipping
set "TEMP_DIR=release_temp" 

echo Cleaning directory: %TEMP_DIR%
if exist %TEMP_DIR% (
rmdir %TEMP_DIR% /S /Q
)
mkdir %TEMP_DIR%
mkdir %TEMP_DIR%\hardmod_nostats

echo Creating release dir and copying files to correct locations
ROBOCOPY /E "server_files_basic" "%TEMP_DIR%"
ROBOCOPY /E "server_files_windows" "%TEMP_DIR%"

REM Build the .smx file
echo Calling makefiles.bat 
call makefiles.bat > compile.log

REM Now we need to put the built rotoblin.smx in the correct place.  Note: copy is picky about slashes.
copy /Y "build\hardmod.smx" "%TEMP_DIR%\addons\sourcemod\plugins\"

REM Finally, zip up the release folder using 7zip
echo zipping release dir

set "PACKAGED_ZIP=L4D2_Coop-16_pub_%date%_windows.zip"
if exist %PACKAGED_ZIP% (
del /F /Q %PACKAGED_ZIP%
)

REM Note: changing the working directory here temporarily just to make the zipping easier.  If I 
REM fail to do this, the release zip ends up with the top level directory inside it being the same
REM as the %TEMP_DIR% value.
pushd %TEMP_DIR%
"../bin/7za920/7za.exe" a ../%PACKAGED_ZIP% .
popd

popd
endlocal

rmdir release_temp /s /q