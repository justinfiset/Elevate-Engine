workspace "ElevateEngine"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "ElevateEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "ElevateEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "ElevateEngine/vendor/imgui/"
IncludeDir["glm"] = "ElevateEngine/vendor/glm/"
IncludeDir["spdlog"] = "ElevateEngine/vendor/spdlog/include"

group "Dependencies"
    include "ElevateEngine/vendor/GLFW"
    include "ElevateEngine/vendor/Glad"
    include "ElevateEngine/vendor/imgui"

group ""
project  "ElevateEngine"
    location "ElevateEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/temps")

    pchheader "eepch.h"
    pchsource "ElevateEngine/src/eepch.cpp"

    files 
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",

        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    defines 
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/src",

        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.spdlog}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "RH_PLATFORM_WINDOWS",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "RH_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "RH_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "RH_DIST"
        runtime "Release"
        optimize "on"

project  "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/temps")

    files 
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/vendor/include",
        "%{IncludeDir.glm}",
        "ElevateEngine/vendor/",
        "ElevateEngine/src"
    }

    links
    {
        "ElevateEngine"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "RH_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "RH_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "RH_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "RH_DIST"
        runtime "Release"
        optimize "on"