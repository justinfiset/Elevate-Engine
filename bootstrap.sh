#!/bin/bash

OS_TYPE=$(uname)

if [ ! -f PREMAKE_CONFIG ]; then
  echo "First time config detected."
  config
else
  if [ "$1" == "edit" ]; then
    echo "Editing configuration."
    config
  else
    contenu=$(<PREMAKE_CONFIG)
    
    if [ -z "$contenu" ]; then
      echo "Error: PREMAKE_CONFIG file is empty."
      exit 1
    fi
    
    ACTION=$contenu
  fi
fi

run

config() {
  echo "Please choose a build option:"
  echo "1 - Visual Studio 2022"
  echo "2 - Visual Studio 2019"
  echo "3 - GNU Make [gmake2]"
  echo "4 - Xcode"
  echo "5 - Exit"
  echo ""

  read -p "Option : " choice

  choice=$(echo "$choice" | tr -d '[:space:]')

  case "$choice" in
    "1")
      ACTION="vs2022"
      ;;
    "2")
      ACTION="vs2019"
      ;;
    "3")
      ACTION="gmake2"
      ;;
    "4")
      ACTION="xcode4"
      ;;
    "5")
      exit 0
      ;;
    *)
      echo "Invalid option, please try again."
      exit 1
      ;;
  esac

  echo "$ACTION" > PREMAKE_CONFIG
  echo "Configuration saved in 'PREMAKE_CONFIG' file."
}

run() {
  echo "Running with the selected option: $ACTION"

  if [ "$OS_TYPE" == "Darwin" ]; then
    PREMAKE_CMD="./vendor/premake/premake5-macos"
  elif [ "$OS_TYPE" == "Linux" ]; then
    PREMAKE_CMD="./vendor/premake/premake5-linux"
  else
    echo "Unsupported OS: $OS_TYPE"
    exit 1
  fi

  $PREMAKE_CMD $ACTION
  exit 0
}
