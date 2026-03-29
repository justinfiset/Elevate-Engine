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

function CommonProject.GetSafeProjectName(name)
	return name:gsub(" ", "-")
end

function CommonProject.SetupProject(directory)
	-- Make sure the directory exists before trying to set up the project
	if not os.isdir(directory) then
		error("Cannot create project. Directory does not exist: " .. directory)
	end

	local infos = CommonProject.ParseProjectFile(directory)

	local projectSafeName = CommonProject.GetSafeProjectName(infos.name)
	project(projectSafeName)
	targetname(projectSafeName)
	location(directory.."/Build/" .. projectSafeName)
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	CommonProject.SetupProjectKind()
	systemversion "latest"

	local targetDirLocation = "%{wks.location}/Build/bin/" .. outputdir .. "/%{prj.name:gsub(' ', '-')}"
	targetdir (targetDirLocation)
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
		directory.."/../../ElevateEngine/vendor/spdlog/include",
		directory.."/../../ElevateEngine/src",
	}

	links { "ElevateEngine" }

	-- Config sound engine if needed
	defines { "EE_NO_SOUNDENGINE=" .. (infos.usesSoundEngine and "0" or "1") }
	if infos.usesSoundEngine then
		Wwise.SetupProject()
	end

	BuildPlatform.SetPlatformDefines()

	defines
	{ 
		"EE_RESOURCE_DIR=\"" ..path.getabsolute(_MAIN_SCRIPT_DIR.."/ElevateEngine/Resources/").."\"",
		"GLM_ENABLE_EXPERIMENTAL",
	}

	filter "system:windows"
		links { "ws2_32" } -- For Wwise Communication WARNING NOT NEEDED IN RELEASE BUT STILL INCLUDED FOR THE MOMENT -- CHANGE THIS}
		buildoptions { "/Zc:wchar_t", "/utf-8" }
		
	filter "system:linux"
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

	filter "system:emscripten"
		links { "ElevateEngine", "ImGui","assimp" }

		linkoptions
        {
			"--preload-file ".._MAIN_SCRIPT_DIR.."/ElevateEngine/Resources/Engine@/Engine",
			"--preload-file "..directory.."/Content@/Content",
			"--preload-file "..directory.."/app.config@/app.config",
			"--preload-file "..directory.."/imgui.ini@/imgui.ini",
		}

	filter { "system:emscripten", "configurations:Editor_*" }
		linkoptions
		{
			"--preload-file ".._MAIN_SCRIPT_DIR.."/ElevateEngine/Resources/Editor@/Editor",
		}

	filter "configurations:Editor_Debug"
		defines { "EE_DEBUG", "EE_EDITOR_BUILD" }
		runtime "Debug"
		symbols "on"

	filter "configurations:Editor_Release"
		defines { "EE_RELEASE", "EE_EDITOR_BUILD" }
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

	BuildPlatform.ConfigureProject(directory, infos)

print("Finished Generating " .. infos.name .. " Solution.\n")
end
