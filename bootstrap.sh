
#!/bin/sh

SCRIPT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
OS_TYPE=$(uname -s)

CONFIG_FILE="$SCRIPT_DIR/PREMAKE_CONFIG"

case "$OS_TYPE" in
  Darwin)
    PREMAKE_CMD="$SCRIPT_DIR/vendor/premake/premake5-macos"
    ;;
  Linux)
    PREMAKE_CMD="$SCRIPT_DIR/vendor/premake/premake5-linux"
    ;;
  *)
    echo "Unsupported OS: $OS_TYPE"
    exit 1
    ;;
esac

config() {
  echo "Please choose a build option:"
  echo "1 - Visual Studio 2022"
  echo "2 - Visual Studio 2019"
  echo "3 - GNU Make [gmake]"
  echo "4 - Xcode"
  echo "5 - Exit"
  echo ""

  printf "Option : "
  read -r choice

  case "$choice" in
    1) ACTION="vs2022" ;;
    2) ACTION="vs2019" ;;
    3) ACTION="gmake2" ;;
    4) ACTION="xcode4" ;;
    5) exit 0 ;;
    *)
      echo "Invalid option, please try again."
      config
      return
      ;;
  esac

  echo "$ACTION" > "$CONFIG_FILE"
  echo "Configuration saved in PREMAKE_CONFIG."

  echo "Generating resources..."

  (
    cd "$SCRIPT_DIR" || exit 1
    "$PREMAKE_CMD" resourcegen
  ) || {
    echo "Resource generation failed."
    exit 1
  }
}

run() {
  echo "Running with the selected option: $*"

  if [ ! -x "$PREMAKE_CMD" ]; then
    echo "Premake executable not found or not executable:"
    echo "$PREMAKE_CMD"
    exit 1
  fi

  cd "$SCRIPT_DIR" || exit 1

  "$PREMAKE_CMD" "$@"
  exit $?
}

# --- Main ---

# Direct arguments: skip configuration.
if [ "$#" -gt 0 ] && [ "$1" != "edit" ]; then
  run "$@"
fi

# Configuration mode.
if [ ! -f "$CONFIG_FILE" ] || [ "$1" = "edit" ]; then
  if [ ! -f "$CONFIG_FILE" ]; then
    echo "First time config detected."
  else
    echo "Editing configuration."
  fi

  config
else
  ACTION=$(cat "$CONFIG_FILE")

  if [ -z "$ACTION" ]; then
    echo "Error: PREMAKE_CONFIG file is empty."
    exit 1
  fi
fi

run "$ACTION"
