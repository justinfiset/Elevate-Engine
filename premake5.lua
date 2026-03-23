require "vendor/premake-export-compile-commands/export-compile-commands"

outputdir = "%{cfg.buildcfg:gsub(' ', '-')}-%{cfg.system}-%{cfg.architecture}"
ElevateConfigs = { "Editor_Debug", "Editor_Release", "Debug", "Release", "Dist" }
ElevatePlatforms = { "Win64", "Linux", "Web" }

include "scripts/premake/Logs.lua"
include "scripts/premake/FileUtils.lua"
include "scripts/premake/Wwise.lua"
include "scripts/premake/CommonProject.lua"
include "scripts/premake/WebBuild.lua"


workspace "ElevateEngine"
	architecture "x64"
	startproject "Sandbox"

	platforms (ElevatePlatforms)
	configurations (ElevateConfigs)

	-- Platform specific toolchain and settings 
	filter "action:vs*"
        platforms { "Win64" }

	filter "system:emscripten"
        toolset "clang"
        linkoptions 
        { 
            "-pthread",
            "-matomics",
            "-mbulk-memory",
            "-s STACK_SIZE=5MB",
            "-s USE_PTHREADS=1",
            "-s PTHREAD_POOL_SIZE=16",
            "-s USE_GLFW=3",
            "-s FULL_ES3=1",
            "-s AUDIO_WORKLET=1",
            "-s WASM_WORKERS=1",
            "-s ALLOW_MEMORY_GROWTH=1",
            "-s INITIAL_MEMORY=512MB",
            "-s FORCE_FILESYSTEM=1",
            "-s ERROR_ON_UNDEFINED_SYMBOLS=0",
            "-s EXPORTED_RUNTIME_METHODS=['FS','wasmTable']",
            "-qext"
        }

        buildoptions
		{
            "-pthread",
            "-matomics", 
            "-mbulk-memory" ,

            -- Mute third party lib warning we can't do anything about
			"-Wno-deprecated-literal-operator", 
			"-Wno-invalid-offsetof",
			"-Wno-deprecated-pragma"
    	}

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

if _ACTION == "ninja" then
    premake.override(premake.main, "postAction", function(base, ...)
        base(...)

        local emsdk = os.getenv("EMSDK")
        local emar_cmd = "emar.bat"
        if emsdk then
            -- Utilisation de slashs normaux pour éviter les problèmes d'escape
            emar_cmd = '"' .. emsdk:gsub("\\", "/") .. '/upstream/emscripten/emar.bat"'
        end

        local files = os.matchfiles("**.ninja")
        for _, filename in ipairs(files) do
            local f = io.open(filename, "r")
            if f then
                local content = f:read("*all")
                f:close()

                -- 1. FIX PARENTHÈSES (Wwise)
                content = content:gsub("[%(%)]", "_")
                
                -- 2. FIX CHEMINS : "D:/" -> "D$:/ "
                content = content:gsub("([%a]):/", "%1$:/")
                content = content:gsub("([%a]):\\", "%1$:/")

                -- 3. FIX GLOBAL DES RÈGLES D'ARCHIVAGE (Catch-all)
                -- On cherche toutes les règles qui utilisent 'ar' pour les rediriger vers emar + RSP
                -- Cela corrige Wwise, GLFW, Assimp et ImGui d'un coup.
                content = content:gsub("rule ar_([%w_]+)\n%s+command%s+=[^\n]+", function(rule_name)
                    -- Si on est dans un fichier ninja qui semble être pour le Web (contient emscripten ou wasm)
                    if filename:find("emscripten") or filename:find("wasm") or content:find("emcc") then
                        return "rule ar_" .. rule_name .. "\n  command = " .. emar_cmd .. " -rcs $out @$out.rsp\n  rspfile = $out.rsp\n  rspfile_content = $in"
                    end
                end)

                -- 4. FIX DES COMMANDES DIRECTES (Au cas où ce n'est pas une règle)
                -- Remplace 'command = ar -rcs' par 'command = emar.bat -rcs'
                if filename:find("emscripten") or filename:find("wasm32") then
                    content = content:gsub("command = ar%s+f-rcs", "command = " .. emar_cmd .. " -rcs")
                end

                -- 5. REDIRECTIONS COMPILATION 
                content = content:gsub("command%s*=%s*clang%+%+", "command = cmd /c em++.bat")
                content = content:gsub("command%s*=%s*clang%s", "command = cmd /c emcc.bat ")

                -- 6. DÉDOUBLONNAGE DES RÈGLES
                local seen_rules = {}
                content = content:gsub("(rule%s+([%w_]+)\n%s+command%s+=[^\n]+\n%s+description%s+=[^\n]+\n)", function(full_rule, rule_name)
                    if seen_rules[rule_name] then return "" else
                        seen_rules[rule_name] = true
                        return full_rule 
                    end
                end)

                -- 7. FORCE OUTPUT TO .JS (To generate the JS glue code + WASM)
                -- On cible spécifiquement la fin de la commande de linkage
                content = content:gsub("-o%s+(.+%.wasm)", "-o %1") -- On garde le .wasm
                content = content:gsub("%.wasm", ".js")           -- Mais on change l'extension globale

                local f_out = io.open(filename, "w")
                if f_out then
                    f_out:write(content)
                    f_out:close()
                end
            end
        end
        print("Elevate Engine: Ninja files fully patched.")
    end)
end

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