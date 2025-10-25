workspace "ElevateEngine"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Editor Debug",
        "Editor Release",
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "ElevateEngine/vendor/GLFW"
    -- include "ElevateEngine/vendor/Glad"
    include "ElevateEngine/vendor/imgui.lua"
    -- include "ElevateEngine/vendor/tinyfiledialogs"  
    include "ElevateEngine/vendor/assimp.lua"

group ""
    include "ElevateEngine/engine.lua"
    include "Sandbox/sandbox.lua"
    include "ElevateLauncher/launcher.lua"