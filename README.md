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
