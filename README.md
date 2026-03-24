![Elevate Logo](./Assets/Logo/logo-png.png)
![Elevate Demo](./Assets/Preview/elevate.gif)

# Project Setup and Usage Guide

This project includes scripts to configure and build your project. Below is a quick guide on how to use them.

## Directory Structure

```
ElevateEngine/
│
├── bootstrap.bat        (Windows)
├── bootstrap.sh         (macOS/Linux)
│
├── scripts/
│   ├── edit.bat         (Windows)
│   └── edit.sh          (macOS/Linux)
│
└── PREMAKE_CONFIG       (configuration file)
```

## Usage
### Download the Project
1. Clone the Repo
   ```cmd
   git clone https://github.com/justinfiset/Elevate-Engine
   ```
2. Open the Downloaded Folder
   ```cmd
   cd Elevate-Engine
   ```
3. Activate Submodules
   ```cmd
   git submodule update --init --recursive --remote
   ```
### First Time Setup
1. Run the `bootstrap` script:
   - For **Windows**:
	 ```cmd
	 bootstrap.bat
	 ```
   - For **macOS/Linux**:
	 ```bash
	 ./bootstrap.sh
	 ```

2. Select a build option (e.g., Visual Studio, GNU Make, etc.) when prompted.

### Edit Configuration

To change the configuration later, run the `edit` script:
- For **Windows**:
  ```cmd
  edit.bat
  ```
- For **macOS/Linux**:
  ```bash
  ./edit.sh
  ```

### Run the Build

Once the configuration is set, the script will automatically run `premake5` with the selected option.

### Build Status

| Platform | OS | Compiler | Status |
| :--- | :--- | :--- | :--- |
| ![Windows](https://img.shields.io/badge/Windows-0078D4?style=flat&logo=windows&logoColor=white) | `windows-latest` | MSVC (vs2022) | [![Windows Build](https://img.shields.io/github/actions/workflow/status/justinfiset/Elevate-Engine/build.yml?job=build-windows&label=)](https://github.com/justinfiset/Elevate-Engine/actions) |
| ![Linux](https://img.shields.io/badge/Linux-FCC624?style=flat&logo=linux&logoColor=black) | `ubuntu-latest` | GCC (gmake2) | [![Linux Build](https://img.shields.io/github/actions/workflow/status/justinfiset/Elevate-Engine/build.yml?job=build-linux&label=)](https://github.com/justinfiset/Elevate-Engine/actions) |
| ![WebAssembly](https://img.shields.io/badge/WebAssembly-654FF0?style=flat&logo=webassembly&logoColor=white) | `emscripten` (Windows Host) | Clang (Ninja) | [![Web Build](https://img.shields.io/github/actions/workflow/status/justinfiset/Elevate-Engine/build.yml?job=build-web-windows-ninja&label=)](https://github.com/justinfiset/Elevate-Engine/actions) |
