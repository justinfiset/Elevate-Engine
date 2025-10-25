project "Sandbox"
    location "Build"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin/temps")
    debugdir ("./")

    files 
    {
        "Source/**.h",
        "Source/**.inl",
        "Source/**.cpp",
        
        "Source/**.vert",
        "Source/**.frag",
    }

    local wwiseSDK = os.getenv("WWISESDK")
    local wwiseIncludePath = wwiseSDK .. "/include"
    -- TODO MAKE THIS PATH DYNAMIC AND NOT HARD CODED - LIKE THIS FOR TEST AND LEARNING PURPOSES
    local wwiseLinkPath = wwiseSDK .. "/x64_vc170/Debug(StaticCRT)/lib"

    includedirs
    {
        "../ElevateEngine/vendor/entt/include",
        wwiseIncludePath,
        "../ElevateEngine/vendor/glm/",
        "../ElevateEngine/src"
    }

    links 
    {
        "ElevateEngine",

        "AkSoundEngine",
        "AkMemoryMgr",
        "AkMusicEngine",
        "AkStreamMgr",
        "AkSpatialAudio",
        "CommunicationCentral", -- Not needed for release config -- TODO CHANGE THIS
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
            "ws2_32" -- For Wwise Communication WARNING NOT NEEDED IN RELEASE BUT STILL INCLUDED FOR THE MOMENT -- CHANGE THIS
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

    filter "configurations:Editor Debug"
		defines
		{
			"EE_DEBUG",
			"EE_EDITOR_BUILD"
		}
		runtime "Debug"
		symbols "on"

	filter "configurations:Editor Release"
		defines 
		{
			"EE_RELEASE",
			"EE_EDITOR_BUILD"
		}
        runtime "Release"
        optimize "on"

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