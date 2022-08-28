 workspace "RedHammer"
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
IncludeDir["GLFW"] = "RedHammer/vendor/GLFW/include"
IncludeDir["Glad"] = "RedHammer/vendor/Glad/include"
IncludeDir["ImGui"] = "RedHammer/vendor/imgui/"

group "Dependencies"
    include "RedHammer/vendor/imgui"
    include "RedHammer/vendor/Glad"

include "RedHammer/vendor"

group ""
project  "RedHammer"
    location "RedHammer"
    kind "SharedLib"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/temps")

    pchheader "rhpch.h"
    pchsource "RedHammer/src/rhpch.cpp"

    files 
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/include",

        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "RH_PLATFORM_WINDOWS",
            "RH_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
        }

    filter "configurations:Debug"
        defines "RH_DEBUG"
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines "RH_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "RH_DIST"
        buildoptions "/MD"
        optimize "On"

project  "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"

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
        "RedHammer/src"
    }

    links
    {
        "RedHammer"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "RH_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "RH_DEBUG"
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines "RH_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "RH_DIST"
        buildoptions "/MD"
        optimize "On"