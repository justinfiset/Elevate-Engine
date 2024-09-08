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
IncludeDir["rapidjson"] = "ElevateEngine/vendor/rapidjson/include"
IncludeDir["stb"] = "ElevateEngine/vendor/stb/"
IncludeDir["spdlog"] = "ElevateEngine/vendor/spdlog/include"
IncludeDir["assimp"] = "ElevateEngine/vendor/assimp/include"
IncludeDir["tinyfiledialogs"] = "ElevateEngine/vendor/tinyfiledialogs"
IncludeDir["ImGuizmo"] = "ElevateEngine/vendor/ImGuizmo"

group "Dependencies"
    include "ElevateEngine/vendor/GLFW"
    include "ElevateEngine/vendor/Glad"
    include "ElevateEngine/vendor/imgui"
    include "ElevateEngine/vendor/tinyfiledialogs"

group ""
project "ElevateEngine"
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

        "%{prj.name}/src/**.vert",
        "%{prj.name}/src/**.frag",

        "%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
        "%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp",
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
        "%{IncludeDir.rapidjson}",
        "%{IncludeDir.stb}",     
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.tinyfiledialogs}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.assimp}",
        "%{IncludeDir.ImGuizmo}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",    
        "tinyfiledialogs",
        "opengl32.lib",
        "ElevateEngine/vendor/assimp/lib/x64/assimp-vc143-mt.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "EE_PLATFORM_WINDOWS",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "EE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "EE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "EE_DIST"
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
        "%{prj.name}/src/**.cpp",

        "%{prj.name}/src/**.vert",
        "%{prj.name}/src/**.frag",
    }

    includedirs
    {
        "%{prj.name}/vendor/include",
        "%{IncludeDir.glm}",
        "%{IncludeDir.rapidjson}",
        "%{IncludeDir.assimp}",
        "ElevateEngine/vendor/",
        "%{IncludeDir.tinyfiledialogs}",
        "%{IncludeDir.ImGuizmo}",
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
            "EE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "EE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "EE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "EE_DIST"
        runtime "Release"
        optimize "on"