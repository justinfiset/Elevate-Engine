function printInfo(message) 
    print(" > " .. message)
end

function printSuccess(message)
    print(" + " .. message)
end

function printWarning(message)
    print(" * " .. message)
end

function parseProjectFile()
    printInfo("Current directory: " .. os.getcwd())

    local projectFile = io.open(".eeproj", "r")
    if not projectFile then
        error("No .eeproj file found in directory.")
    end
    printSuccess(".eeproj file found.")

    local content = projectFile:read("*a")
    projectFile:close()

    -- Json parsing for basic informations
    local project = {}
    project.name = content:match('"name"%s*:%s*"([^"]+)"')
    if not project.name then
        error("Project name not found in .eeproj file.")
    end
    printSuccess("Project name: " .. project.name)

    project.usesSoundEngine = content:match('"usesSoundEngine"%s*:%s*(true)') == "true"
    if( project.usesSoundEngine ) then
        printSuccess("Project uses Wwise Sound Engine.")
    else
        printWarning("Project does not use Wwise Sound Engine.")
    end
    
    return project
end

local projectInfos = parseProjectFile()

project(projectInfos.name)
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
        "EE_NO_SOUNDENGINE=" .. (projectInfos.usesSoundEngine and "0" or "1")
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