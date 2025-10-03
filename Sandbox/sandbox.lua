project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin/temps")
    debugdir ("./")

    files 
    {
        "src/**.h",
        "src/**.cpp",
        "src/**.vert",
        "src/**.frag",
    }

    local wwiseSDK = os.getenv("WWISESDK")
    local wwiseIncludePath = wwiseSDK .. "/include"
    -- TODO MAKE THIS PATH DYNAMIC AND NOT HARD CODED - LIKE THIS FOR TEST AND LEARNING PURPOSES
    local wwiseLinkPath = wwiseSDK .. "/x64_vc170/Debug(StaticCRT)/lib"

    includedirs
    {
        wwiseIncludePath,
        "../ElevateEngine/vendor/spdlog/include",
        "../ElevateEngine/vendor/entt/include",
        "../ElevateEngine/vendor/glm/",
        "../ElevateEngine/src"
    }

    links 
    {
        "ElevateEngine",

        "AkSoundEngine",
        "AkMemoryMgr",
        "AkStreamMgr",
        "AkSpatialAudio",
        "CommunicationCentral", -- Not needed for release config
        "AkVorbisDecoder"
    }

    libdirs
    {
        wwiseLinkPath
    }

    defines
    {
        "EE_EDITOR_BUILD"
    }

    filter "system:windows"
        systemversion "latest"
        defines { "EE_PLATFORM_WINDOWS" }

        links
        {
            "ws2_32" -- For Wwise Communication WARNING NOT NEEDED IN RELEASE BUT STILL INCLUDED FOR THE MOMENT
        }

        buildoptions { "/Zc:wchar_t" }

    filter "system:linux"
        systemversion "latest"
        defines { "EE_PLATFORM_LINUX" }

        links
        {
            "GL", "GLU",
            "X11", "Xrandr", "Xinerama", "Xcursor", "Xi",
            "dl", "pthread"
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
