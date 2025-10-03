project "ElevateEngine"
    location "ElevateEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin/temps")

    pchheader "eepch.h"
    pchsource "src/eepch.cpp"

    IncludeDir = {}
    IncludeDir["GLFW"] = "vendor/GLFW/include" 
    IncludeDir["Glad"] = "vendor/Glad/include"
    IncludeDir["ImGui"] = "vendor/ImGui/"
    IncludeDir["glm"] = "vendor/glm/"
    IncludeDir["rapidjson"] = "vendor/rapidjson/include"
    IncludeDir["stb"] = "vendor/stb/"
    IncludeDir["spdlog"] = "vendor/spdlog/include"
    IncludeDir["assimp"] = "vendor/assimp/include"
    IncludeDir["tinyfiledialogs"] = "vendor/tinyfiledialogs"
    IncludeDir["ImGuizmo"] = "vendor/ImGuizmo"
    IncludeDir["entt"] = "vendor/entt/include"

    local wwiseSDK = os.getenv("WWISESDK")
    local wwiseIncludePath = wwiseSDK .. "/include"
    -- TODO MAKE THIS PATH DYNAMIC AND NOT HARD CODED - LIKE THIS FOR TEST AND LEARNING PURPOSES
    local wwiseLinkPath = wwiseSDK .. "/x64_vc170/Debug(StaticCRT)/lib"

    files 
    {
        "src/**.h",
        "src/**.cpp",

        "src/**.vert",
        "src/**.frag",

        "vendor/ImGuizmo/ImGuizmo.cpp",
        "vendor/Glad/src/glad.c",
        "vendor/tinyfiledialogs/tinyfiledialogs.cpp",
    }

    defines 
    {
        "EE_ENGINE_BUILD",
        "_CRT_SECURE_NO_WARNINGS",
        "IMGUI_DEFINE_MATH_OPERATORS",
    }

    includedirs
    {
        "src",

        wwiseIncludePath,
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

    libdirs
    {
        wwiseLinkPath
    }

    links
    {
	    -- "Glad", NOT A PROJECT ANYMORE
        "GLFW",
        "ImGui",
        "assimp",
        -- "tinyfiledialogs", NOT A PROJECT ANYMORE

        "AkSoundEngine",
        "AkMemoryMgr",
        "AkStreamMgr",
        "AkSpatialAudio",
        "CommunicationCentral", -- Not needed for release config
        "AkVorbisDecoder",
        "AkOpusDecoder"
    }


    filter { "files:vendor/**.c" }
            flags { "NoPCH" }

    filter { "files:vendor/**.cpp" }
        flags { "NoPCH" } 

    filter "system:windows"
        systemversion "latest"
        buildoptions { "/Zc:wchar_t" }

        defines
        {
            "EE_PLATFORM_WINDOWS",
        }

        links
        {
            "opengl32.lib",
            "ws2_32" -- For Wwise Communication WARNING NOT NEEDED IN RELEASE BUT STILL INCLUDED FOR THE MOMENT
        }

    filter "system:linux"
        systemversion "latest"

        defines
        {
            "EE_PLATFORM_LINUX",
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