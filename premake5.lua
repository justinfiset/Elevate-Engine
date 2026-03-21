require "vendor/premake-export-compile-commands/export-compile-commands"

include "scripts/premake/Logs.lua"
include "scripts/premake/Wwise.lua"
include "scripts/premake/CommonProject.lua"

outputdir = "%{cfg.buildcfg:gsub(' ', '-')}-%{cfg.system}-%{cfg.architecture}"

workspace "ElevateEngine"
	architecture "x64"
	startproject "Sandbox"

	platforms { "Win64", "Linux", "Web" }

	configurations
	{
		"Editor_Debug",
		"Editor_Release",
		"Debug",
		"Release",
		"Dist"
	}

	if _ACTION == "ninja" then
        premake.override(premake.main, "postAction", function(base, ...)
            base(...) 

            local files = os.matchfiles("**.ninja")
            for _, filename in ipairs(files) do
                local f = io.open(filename, "r")
                if f then
                    local content = f:read("*all")
                    f:close()

                    -- 1. FIX PARENTHÈSES : Indispensable pour tes fichiers Wwise
                    content = content:gsub("[%(%)]", "_")
                    
                    -- 2. FIX COLONS : Chemins Windows (C:\ -> /c/)
                    content = content:gsub("([%s])([%a]):\\", "%1/%2/")
                    content = content:gsub("([%s])([%a]):/", "%1/%2/")

                    -- 3. FIX DOUBLONS : On ne garde que la première définition de chaque règle
                    local rules_to_fix = { "copy", "prebuild", "postbuild", "prelink", "custom" }
                    for _, rule_name in ipairs(rules_to_fix) do
                        local count = 0
                        -- Ce pattern capture la règle et ses deux lignes de commande/description
                        local pattern = "rule " .. rule_name .. "\n  command = [^\n]+\n  description = [^\n]+\n"
                        content = content:gsub(pattern, function(match)
                            count = count + 1
                            return (count > 1) and "" or match
                        end)
                    end

                    local f_out = io.open(filename, "w")
                    if f_out then
                        f_out:write(content)
                        f_out:close()
                    end
                end
            end
            print("Elevate Engine: Ninja files sanitized (Rules & Parentheses).")
        end)
    end

	-- Platform specific toolchain and settings 
	filter "action:vs*"
        platforms { "Win64" }
    filter {}

	filter "system:emscripten"
        toolset "clang"
    filter {}

	filter "platforms:Win64"
        system "windows"
        architecture "x64"

    filter "platforms:Linux"
        system "linux"
        architecture "x64"
        toolset "gcc"
		filter "action:gmake*"
            toolset "gcc"
        filter {}

	filter "platforms:Web"
        system "emscripten"
        architecture "wasm32"
        toolset "clang"

	filter {}

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