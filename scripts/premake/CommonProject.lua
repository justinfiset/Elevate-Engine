CommonProject = {}

function CommonProject.ParseProjectFile(directory)
	Logger.Info("Current directory: " .. directory)

	local projectFile = io.open(directory.."/.eeproj", "r")
	if not projectFile then
		error("No .eeproj file found in directory.")
	end
	Logger.Success(".eeproj file found.")

	local content = projectFile:read("*a")
	projectFile:close()

	-- Json parsing for basic informations
	local project = {}
	project.name = content:match('"name"%s*:%s*"([^"]+)"')
	if not project.name then
		error("Project name not found in .eeproj file.")
	end
	Logger.Success("Project name: " .. project.name)

    project.usesSoundEngine = content:match('"usesSoundEngine"%s*:%s*(true)') == "true"
    if project.usesSoundEngine then
        Logger.Success("Project uses Wwise Sound Engine.")

        project.useWaapi = content:match('"usesWaapi"%s*:%s*(true)') == "true"
        if project.useWaapi then
            Logger.Success("  Project uses WAAPI.")
        end
    else
        Logger.Warning("Project does not use Wwise Sound Engine.")
    end
    
    return project
end

function CommonProject.SetupProjectKind()
	filter "configurations:Dist"
        kind "WindowedApp"
    filter "not configurations:Dist"
    	kind "ConsoleApp"
    filter {}
end

function CommonProject.SetupProject(directory)
	-- Make sure the directory exists before trying to set up the project
	if not os.isdir(directory) then
		error("Cannot create project. Directory does not exist: " .. directory)
	end

	local infos = CommonProject.ParseProjectFile(directory)

	project(infos.name)
	targetname(infos.name)
	location(directory.."/Build/" .. infos.name)
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	CommonProject.SetupProjectKind()

	targetdir ("%{wks.location}/Build/bin/" .. outputdir .. "/%{prj.name:gsub(' ', '-')}")
	objdir ("%{wks.location}/Build/bin-int/" .. outputdir .. "/%{prj.name:gsub(' ', '-')}")
	debugdir (directory)

	files 
	{
		directory.."/Source/**.h",
		directory.."/Source/**.inl",
		directory.."/Source/**.cpp",
		
		directory.."/Source/**.vert",
		directory.."/Source/**.frag",
	}

	includedirs
	{
		directory.."/../../ElevateEngine/vendor/entt/include",
		directory.."/../../ElevateEngine/vendor/ImGui/",
		directory.."/../../ElevateEngine/vendor/glm/",
		directory.."/../../ElevateEngine/src"
	}

	links
    {
        "ElevateEngine"
    }

	defines
    {
        "EE_NO_SOUNDENGINE=" .. (infos.usesSoundEngine and "0" or "1")
    }

	if infos.usesSoundEngine then
		Wwise.SetupProject()
	end

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
			"ElevateEngine",
			"ImGui",
			"assimp",
			"GLFW",
			"GL",
			"X11",
			"Xrandr",
			"Xinerama",
			"Xcursor",
			"Xi",
			"dl",
			"pthread",
			"m"
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

print("Finished Generating " .. infos.name .. " Solution.\n")
end
