@echo off
setlocal enabledelayedexpansion

:: Direct action argument
if not "%~1"=="" (
    set "ACTION=%~1"
    set "PAUSE_AFTER=0"
    goto run
)

set "PAUSE_AFTER=1"

:: Load configuration
if not exist PREMAKE_CONFIG (
    echo First time config detected.
    goto config
)

:: Edit configuration
if "%1"=="edit" (
    echo Editing configuration.
    goto config
)

:: Read configuration
set "contenu="
for /f "delims=" %%A in (PREMAKE_CONFIG) do set "contenu=%%A"

if not defined contenu (
    echo Error: PREMAKE_CONFIG file is empty.
    exit /b 1
)

set "ACTION=!contenu!"
goto run


:config
echo Please choose a build option:
echo 1 - Visual Studio 2022
echo 2 - Visual Studio 2019
echo 3 - GNU Make [gmake]
echo 4 - Xcode
echo 5 - Exit
echo.

set /p choice="Option: "

if "!choice!"=="1" set "ACTION=vs2022"
if "!choice!"=="2" set "ACTION=vs2019"
if "!choice!"=="3" set "ACTION=gmake2"
if "!choice!"=="4" set "ACTION=xcode4"
if "!choice!"=="5" exit /b 0

if not defined ACTION (
    echo Invalid option, please try again.
    pause
    goto config
)

echo !ACTION! > "%~dp0PREMAKE_CONFIG"
echo Configuration saved in PREMAKE_CONFIG file.

:: Generate resources before generating the solution.
pushd "%~dp0"
call vendor\premake\premake5.exe resourcegen

if errorlevel 1 (
    echo Resource generation failed.
    popd
    exit /b 1
)

popd

goto run

:run
echo Running with the selected option: !ACTION!

pushd "%~dp0"
call "%~dp0vendor\premake\premake5.exe" !ACTION!
set "RESULT=!ERRORLEVEL!"
popd

if "!PAUSE_AFTER!"=="1" pause

exit /b !RESULT!