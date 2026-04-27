require "vendor/premake-export-compile-commands/export-compile-commands"

outputdir = "%{cfg.buildcfg:gsub(' ', '-')}-%{cfg.system}-%{cfg.architecture}"
ElevateConfigs = { "Editor_Debug", "Editor_Release", "Debug", "Release", "Dist" }

include "scripts/premake/Logs.lua"
include "scripts/premake/FileUtils.lua"
include "scripts/premake/Wwise.lua"
include "scripts/premake/CommonProject.lua"
include "scripts/premake/PlatformStrategies.lua"

workspace "ElevateEngine"
	architecture "x64"
	startproject "Sandbox"
	configurations (ElevateConfigs)

    BuildPlatform.ConfigureWorkspace()
	Wwise.SetupWorkspace()
    BuildPlatform.ConfigureWorkspaceAction()

	include "ElevateEngine/engine.lua"

	group "Dependencies"
		include "ElevateEngine/vendor/glfw.lua"
		include "ElevateEngine/vendor/imgui.lua"
		include "ElevateEngine/vendor/assimp.lua"

	group "Tests"
		CommonProject.SetupProject("ElevateTests", function(d, i)
			directory = d or "ElevateTests"
			infos = i or CommonProject.GetProjectInfos(directory)
			include "ElevateTests/tests.lua"
		end)

	-- Setup the Wwise SoundEngine if the SDK is found
	Wwise.Initialize()

	group "User Projects"
		local userProjects = os.matchdirs("Projects/*")

		for _, projectPath in ipairs(userProjects) do
			includePath = projectPath .. "/project.lua"
			Logger.Info("Loading User Project: " .. projectPath)
			if os.isfile(includePath) then
				include(includePath)
			else -- If there is no custom config, try to set up a default project
				CommonProject.SetupProject(projectPath)
			end
		end
