@echo off
setlocal enabledelayedexpansion

if not exist PREMAKE_CONFIG (
  echo "First time config detected."
  goto :config
) else (
  if "%1"=="edit" (
    echo "Editing configuration."
    goto :config
  ) else (
    for /f "delims=" %%A in (PREMAKE_CONFIG) do set "contenu=%%A"
    
    if not defined contenu (
      echo Error: PREMAKE_CONFIG file is empty.
      exit /b
    )
    
    set ACTION=!contenu!
  )
)

goto :run

:config
echo Please choose a build option :
echo 1 - Visual Studio 2022
echo 2 - Visual Studio 2019
echo 3 - GNU Make [gmake2]
echo 4 - Xcode
echo 5 - Exit
echo.

set /p choice="Option : "

for /f "tokens=* delims=" %%a in ("!choice!") do set choice=%%a

if "!choice!"=="1" set ACTION=vs2022
if "!choice!"=="2" set ACTION=vs2019
if "!choice!"=="3" set ACTION=gmake2
if "!choice!"=="4" set ACTION=xcode4
if "!choice!"=="5" exit /b

if not defined ACTION (
  echo Invalid option, please try again.
  pause
  exit /b
)

echo !ACTION! > PREMAKE_CONFIG
echo Configuration saved in "PREMAKE_CONFIG" file.

:run
echo Running with the selected option: !ACTION!
call vendor\premake\premake5.exe !ACTION!
pause
exit /b
