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
IncludeDir["ImGui"] = "ElevateEngine/vendor/ImGui/"
IncludeDir["glm"] = "ElevateEngine/vendor/glm/"
IncludeDir["rapidjson"] = "ElevateEngine/vendor/rapidjson/include"
IncludeDir["stb"] = "ElevateEngine/vendor/stb/"
IncludeDir["spdlog"] = "ElevateEngine/vendor/spdlog/include"
IncludeDir["assimp"] = "ElevateEngine/vendor/assimp/include"
IncludeDir["tinyfiledialogs"] = "ElevateEngine/vendor/tinyfiledialogs"
IncludeDir["ImGuizmo"] = "ElevateEngine/vendor/ImGuizmo"
IncludeDir["entt"] = "ElevateEngine/vendor/entt/include"

group "Dependencies"
    include "ElevateEngine/vendor/GLFW"
    include "ElevateEngine/vendor/Glad"
    include "ElevateEngine/vendor/imgui.lua"
    include "ElevateEngine/vendor/tinyfiledialogs"  
    include "ElevateEngine/vendor/assimp.lua"

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
        "_CRT_SECURE_NO_WARNINGS",
        "IMGUI_DEFINE_MATH_OPERATORS",
        "IMGUI_IMPL_OPENGL_LOADER_GLAD"
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
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.entt}"
    }

    links
    {
	"Glad",
        "GLFW",
        "ImGui",
        "assimp",
        "tinyfiledialogs",
    }

    filter { "files:**/vendor/ImGui/**.cpp" }
        flags { "NoPCH" }

    filter { "files:**/vendor/ImGuizmo/**.cpp" }
        flags { "NoPCH" }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "EE_PLATFORM_WINDOWS",
            "GLFW_INCLUDE_NONE",
        }

	links
	{
	    "opengl32.lib"
	}

    filter "system:linux"
        systemversion "latest"

        defines
        {
            "EE_PLATFORM_LINUX",
            "GLFW_INCLUDE_NONE",
            "STBI_NO_SIMD"
        }

	links
	{
	    "GL",
	    "GLU",
	    "X11",
	    "dl",
	    "pthread",
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

project "Sandbox"
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

	"%{IncludeDir.ImGui}/backends/imgui_impl_glfw.cpp",
        "%{IncludeDir.ImGui}/backends/imgui_impl_opengl3.cpp",

        "ElevateEngine/vendor/Glad/src/glad.c",
    }

    includedirs
    {
	"%{IncludeDir.Glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.rapidjson}",
        "%{IncludeDir.assimp}",
        "%{IncludeDir.tinyfiledialogs}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.entt}",
        
        "%{prj.name}/vendor/include",
        "ElevateEngine/vendor/",
        "ElevateEngine/src"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "EE_PLATFORM_WINDOWS",
            "ASSIMP_STATIC"
        }

	links 
	{
	    "ElevateEngine",
	}

    filter "system:linux"
        systemversion "latest"

        defines
        {
            "EE_PLATFORM_LINUX",
            "ASSIMP_STATIC",
	    "STBI_NO_SIMD"        
	}

	links
	{
	    "GL",
	    "GLU",
	    "X11",
	    "Xrandr",
	    "Xinerama",
	    "Xcursor",
	    "Xi",
	    "dl",
	    "pthread",
	    "Glad",
            "GLFW",
            "ImGui",
	    "ElevateEngine",
	    "assimp",
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
