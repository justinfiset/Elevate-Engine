WebBuild = {}

local htmlTemplatePath = _MAIN_SCRIPT_DIR .. "/ElevateEngine/web/index.template.html"

function WebBuild.GenerateHTML(project, outputDir)
    local f = io.open(htmlTemplatePath, "r")
    if f then
        local content = f:read("*all")
        f:close()

        content = content:gsub("${PROJECT_NAME}", project.name)

        local outPath = outputDir .. "/index.html"
        local outFile = io.open(outPath, "w")
        if outFile then
            outFile:write(content)
            outFile:close()
            Logger.Info("Elevate Engine: Generated index.html for " .. project.name .. " at " .. outPath)
        end
    end
end

function WebBuild.ApplyCompilerSettings()
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
end

function WebBuild.CleanupBuildFiles()
    if _ACTION == "ninja" then
        premake.override(premake.main, "postAction", function(base, ...)
            base(...)

            local emsdk = os.getenv("EMSDK")
            local emar_cmd = "emar.bat"
            if emsdk then
                -- Utilisation de slashs normaux pour éviter les problèmes d'escape
                emar_cmd = '"' .. emsdk:gsub("\\", "/") .. '/upstream/emscripten/emar.bat"'
            end

            Logger.Info("Elevate Engine: Patching Ninja files for WebAssembly build...")
            local is_web_build = (_OPTIONS and _OPTIONS["os"] == "emscripten") or (os.target and os.target() == "emscripten")
            local files = os.matchfiles(_MAIN_SCRIPT_DIR .. "/**.ninja")
            for _, filename in ipairs(files) do
                Logger.Info("   Patching file: " .. filename)
                local f = io.open(filename, "r")
                if f then
                    local content = f:read("*all")
                    f:close()
                    local is_web_ninja = is_web_build or filename:find("emscripten") or filename:find("wasm") or content:find("emcc")
                    local nl = content:find("\r\n", 1, true) and "\r\n" or "\n"
                    local ar_rule_block = "  command = " .. emar_cmd .. " -rcs $out @$out.rsp" .. nl
                        .. "  rspfile = $out.rsp" .. nl
                        .. "  rspfile_content = $in"

                    -- 1. FIX PARENTHÈSES (Wwise)
                    content = content:gsub("[%(%)]", "_")
                    
                    -- 2. FIX CHEMINS : "D:/" -> "D$:/ "
                    content = content:gsub("([%a]):/", "%1$:/")
                    content = content:gsub("([%a]):\\", "%1$:/")

                    -- 3. FIX GLOBAL DES RÈGLES D'ARCHIVAGE (Catch-all)
                    -- Forcer toutes les règles ar_* du web vers emar + response file,
                    -- quel que soit l'outil déjà présent (ar/emar/llvm-ar) et même si la commande manque.
                    if is_web_ninja then
                        -- Cas standard: rule ar_* suivi d'une ligne command = ...
                        content = content:gsub("rule%s+ar_[%w_]+\r?\n%s*command%s*=%s*[^\r\n]*", function(rule_and_command)
                            local rule_name = rule_and_command:match("rule%s+(ar_[%w_]+)")
                            if rule_name then
                                return "rule " .. rule_name .. nl .. ar_rule_block
                            end
                            return rule_and_command
                        end)

                        -- Cas sans command (rare): injecte command + rsp avant description.
                        content = content:gsub(
                            "(rule%s+ar_[%w_]+)\r?\n%s*description%s*=%s*Archiving static library %$out",
                            "%1" .. nl .. ar_rule_block .. nl .. "  description = Archiving static library $out"
                        )
                    end

                    -- 4. FIX DES COMMANDES DIRECTES (Au cas où ce n'est pas une règle)
                    -- Remplace 'command = ar -rcs' par 'command = emar.bat -rcs' avec response files
                    content = content:gsub("command = ar %-rcs", "command = " .. emar_cmd .. " -rcs")
                    content = content:gsub("command = ar f%-rcs", "command = " .. emar_cmd .. " -rcs")

                    -- 5. REDIRECTIONS COMPILATION 
                    content = content:gsub("command%s*=%s*clang%+%+([^\n]*)", "command = cmd /c em++.bat%1")
                    content = content:gsub("command%s*=%s*clang%s([^\n]*)", "command = cmd /c emcc.bat %1")

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
end