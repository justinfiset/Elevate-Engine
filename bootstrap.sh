
#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OS_TYPE=$(uname)

config() {
  echo "Please choose a build option:"
  echo "1 - Visual Studio 2022"
  echo "2 - Visual Studio 2019"
  echo "3 - GNU Make [gmake]"
  echo "4 - Xcode"
  echo "5 - Exit"
  echo ""

  read -r -p "Option : " choice
  choice=$(echo "$choice" | tr -d '[:space:]')

  case "$choice" in
    "1") ACTION="vs2022" ;;
    "2") ACTION="vs2019" ;;
    "3") ACTION="gmake2" ;;
    "4") ACTION="xcode4" ;;
    "5") exit 0 ;;
    *)
      echo "Invalid option, please try again."
      config
      return
      ;;
  esac

  echo "$ACTION" > "$SCRIPT_DIR/PREMAKE_CONFIG"
  echo "Configuration saved in 'PREMAKE_CONFIG' file."

  echo "Generating resources..."

  (
    cd "$SCRIPT_DIR" || exit 1

    if [ "$OS_TYPE" = "Darwin" ]; then
      ./vendor/premake/premake5-macos resourcegen
    elif [ "$OS_TYPE" = "Linux" ]; then
      ./vendor/premake/premake5-linux resourcegen
    else
      echo "Unsupported OS: $OS_TYPE"
      exit 1
    fi
  )

  if [ $? -ne 0 ]; then
    echo "Resource generation failed."
    exit 1
  fi
}

run() {
  echo "Running with the selected option: $*"

  if [ "$OS_TYPE" = "Darwin" ]; then
    PREMAKE_CMD="$SCRIPT_DIR/vendor/premake/premake5-macos"
  elif [ "$OS_TYPE" = "Linux" ]; then
    PREMAKE_CMD="$SCRIPT_DIR/vendor/premake/premake5-linux"
  else
    echo "Unsupported OS: $OS_TYPE"
    exit 1
  fi

  cd "$SCRIPT_DIR" || exit 1

  "$PREMAKE_CMD" "$@"
  exit $?
}

# --- Main ---

# Direct action argument: skip config and execute without any pause.
if [ "$#" -gt 0 ] && [ "$1" != "edit" ]; then
  run "$@"
fi

# Configuration mode.
if [ ! -f "$SCRIPT_DIR/PREMAKE_CONFIG" ] || [ "$1" = "edit" ]; then
  if [ ! -f "$SCRIPT_DIR/PREMAKE_CONFIG" ]; then
    echo "First time config detected."
  else
    echo "Editing configuration."
  fi

  config
else
  contenu=$(<"$SCRIPT_DIR/PREMAKE_CONFIG")

  if [ -z "$contenu" ]; then
    echo "Error: PREMAKE_CONFIG file is empty."
    exit 1
  fi

  ACTION="$contenu"
fi

run "$ACTION"
