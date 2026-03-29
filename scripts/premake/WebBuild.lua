WebBuild = {}

local htmlTemplatePath = _MAIN_SCRIPT_DIR .. "/ElevateEngine/Web/Templates/index.template.html"
local htmlAssetsPath = _MAIN_SCRIPT_DIR .. "/ElevateEngine/Web/Assets/"

function WebBuild.GenerateHTML(project, outputDir)
    local f = io.open(htmlTemplatePath, "r")
    if f then
        local safeName = CommonProject.GetSafeProjectName(project.name)
        local content = f:read("*all")
        f:close()

        -- Copy Assets
        FileUtils.SafeCopy(htmlAssetsPath, outputDir)

        content = content:gsub("%$%{PROJECT_NAME%}", project.name)
        content = content:gsub("%$%{PROJECT_SAFE_NAME%}", safeName)

        local outPath = outputDir .. "/index.html"
        local outFile = io.open(outPath, "w")
        if outFile then
            outFile:write(content)
            outFile:close()
            Logger.Info("Generated index.html at " .. outPath)
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
        "-s ASSERTIONS=1",
        "-s EMULATE_FUNCTION_POINTER_CASTS=1",
        "-s NO_EXIT_RUNTIME=1",
        "-s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=0",
        "-s EXPORTED_RUNTIME_METHODS=['FS','wasmTable','ccall','cwrap']",
        "-s ALLOW_MEMORY_GROWTH=1",
        "-s INITIAL_MEMORY=512MB",
        "-s FORCE_FILESYSTEM=1",
        "-s ERROR_ON_UNDEFINED_SYMBOLS=0",
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

local function isWebBuild()
    return (_OPTIONS and _OPTIONS["os"] == "emscripten") or (os.target and os.target() == "emscripten")
end

local function detectNewline(content)
    return content:find("\r\n", 1, true) and "\r\n" or "\n"
end

local function makeArchiveRuleBlock(emar_cmd, nl)
    return "  command = " .. emar_cmd .. " -rcs $out @$out.rsp" .. nl
        .. "  rspfile = $out.rsp" .. nl
        .. "  rspfile_content = $in"
end

local function normalizeArchiveRules(content, ar_rule_block, nl)
    -- Emscripten generates non-standard archive rules that we need to patch to work with our emar wrapper
    content = content:gsub("rule%s+ar_[%w_]+\r?\n%s*command%s*=%s*[^\r\n]*", function(rule_and_command)
        local rule_name = rule_and_command:match("rule%s+(ar_[%w_]+)")
        if rule_name then
            return "rule " .. rule_name .. nl .. ar_rule_block
        end
        return rule_and_command
    end)
    
    -- In case there are any rules we missed, also directly replace any command lines for archiving
    content = content:gsub(
        "(rule%s+ar_[%w_]+)\r?\n%s*description%s*=%s*Archiving static library %$out",
        "%1" .. nl .. ar_rule_block .. nl .. "  description = Archiving static library $out"
    )

    return content
end

local function deduplicateRules(content, nl)
    local seen_rules = {}
    local pattern = "(rule%s+([%w_]+)\r?\n%s+command%s+=[^\r\n]+\r?\n%s+description%s+=[^\r\n]+\r?\n)"
    return content:gsub(pattern, function(full_rule, rule_name)
        if seen_rules[rule_name] then
            return ""
        end
        seen_rules[rule_name] = true
        return full_rule
    end)
end

local function patchNinjaContent(content, filename, emar_cmd, web_build)
    local is_web_ninja = web_build or filename:find("emscripten") or filename:find("wasm") or content:find("emcc")
    local nl = detectNewline(content)
    local ar_rule_block = makeArchiveRuleBlock(emar_cmd, nl)

    -- Make sure there are no parentheses in paths (Ninja doesn't like them)
    content = content:gsub("[%(%)]", "_")
    -- Fix absolute path with drive letters
    content = content:gsub("([%a]):/", "%1$:/")
    content = content:gsub("([%a]):\\", "%1$:/")
    -- Fix archive rules if it's a web ninja (Emscripten generates non-standard rules that we need to patch)
    if is_web_ninja then
        content = normalizeArchiveRules(content, ar_rule_block, nl)
    end
    -- Redirect compilation commands to use emcc/em++ wrappers that set the right environment for Emscripten
    content = content:gsub("command = ar %-rcs", "command = " .. emar_cmd .. " -rcs")
    content = content:gsub("command = ar f%-rcs", "command = " .. emar_cmd .. " -rcs")
    -- Redirect clang/clang++ to emcc/em++
    content = content:gsub("command%s*=%s*clang%+%+([^\n]*)", "command = cmd /c em++.bat%1")
    content = content:gsub("command%s*=%s*clang%s([^\n]*)", "command = cmd /c emcc.bat %1")
    -- Remove duplicate rules
    content = deduplicateRules(content, nl)
    -- Force output to .js for wasm builds
    content = content:gsub("-o%s+(.+%.wasm)", "-o %1")
    content = content:gsub("%.wasm", ".js")

    return content
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
            local web_build = isWebBuild()
            local files = os.matchfiles(_MAIN_SCRIPT_DIR .. "/**.ninja")
            for _, filename in ipairs(files) do
                Logger.Info("   Patching file: " .. filename)
                local f = io.open(filename, "r")
                if f then
                    local content = f:read("*all")
                    f:close()
                    content = patchNinjaContent(content, filename, emar_cmd, web_build)

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