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
IncludeDir["glm"] = "RedHammer/vendor/glm/"

group "Dependencies"
    include "RedHammer/vendor/GLFW"
    include "RedHammer/vendor/Glad"
    include "RedHammer/vendor/imgui"

group ""
project  "RedHammer"
    location "RedHammer"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/temps")

    pchheader "rhpch.h"
    pchsource "RedHammer/src/rhpch.cpp"

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
        "%{prj.name}/vendor/include",

        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glm}",
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
        "RedHammer/vendor/",
        "RedHammer/src"
    }

    links
    {
        "RedHammer"
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