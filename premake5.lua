require "vendor/premake-export-compile-commands/export-compile-commands"

include "scripts/premake/Logs.lua"
include "scripts/premake/Wwise.lua"
include "scripts/premake/CommonProject.lua"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "ElevateEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Editor Debug",
		"Editor Release",
		"Debug",
		"Release",
		"Dist"
	}

	Wwise.SetupWorkspace()

include "ElevateEngine/engine.lua"

group "Dependencies"
	include "ElevateEngine/vendor/glfw.lua"
	include "ElevateEngine/vendor/imgui.lua"
	include "ElevateEngine/vendor/assimp.lua"

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