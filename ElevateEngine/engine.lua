local function values(t)
    local res = {}
    for _, v in pairs(t) do
        table.insert(res, v)
    end
    return res
end

local function normalizePath(filename)
    return filename:gsub("\\", "/")
end

local function moduleNameForFile(filename)
    local file = io.open(filename, "r")
    if not file then
        return nil
    end

    local moduleName = file:read("*all"):match("export%s+module%s+([%w%._:]+)%s*;")
    file:close()
    return moduleName
end

local function orderedModuleFiles()
    local moduleFiles = os.matchfiles(_MAIN_SCRIPT_DIR .. "/ElevateEngine/Source/**/*.ixx")
    local modules = {}
    local dependencies = {}
    local headerImports = {}
    local sourceRoot = _MAIN_SCRIPT_DIR .. "/ElevateEngine/Source/"

    local function collectImports(content)
        local imports = {}
        for importedModule in content:gmatch("import%s+([%w%._:]+)%s*;") do
            table.insert(imports, importedModule)
        end
        for partition in content:gmatch("import%s+:([%w_]+)%s*;") do
            table.insert(imports, ":" .. partition)
        end
        return imports
    end

    for _, pattern in ipairs({"**/*.h", "**/*.hpp", "**/*.inl"}) do
        for _, filename in ipairs(os.matchfiles(sourceRoot .. pattern)) do
            local file = io.open(filename, "r")
            if file then
                headerImports[normalizePath(filename)] = collectImports(file:read("*all"))
                file:close()
            end
        end
    end

    for _, filename in ipairs(moduleFiles) do
        local file = io.open(filename, "r")
        if file then
            local content = file:read("*all")
            file:close()

            local moduleName = content:match("export%s+module%s+([%w%._:]+)%s*;")
            if moduleName then
                modules[moduleName] = filename
                dependencies[moduleName] = {}
                local moduleBase = moduleName:match("^([^:]+)")
                for _, importedModule in ipairs(collectImports(content)) do
                    if importedModule:sub(1, 1) == ":" then
                        table.insert(dependencies[moduleName], moduleBase .. importedModule)
                    else
                        table.insert(dependencies[moduleName], importedModule)
                    end
                end

                for includePath in content:gmatch("#include%s+[<\"]([^>\"]+)[>\"]") do
                    for headerFilename, imports in pairs(headerImports) do
                        if headerFilename:sub(-#includePath) == includePath then
                            for _, importedModule in ipairs(imports) do
                                if importedModule:sub(1, 1) == ":" then
                                    table.insert(dependencies[moduleName], moduleBase .. importedModule)
                                else
                                    table.insert(dependencies[moduleName], importedModule)
                                end
                            end
                        end
                    end
                end
            end
        end
    end

    local ordered = {}
    local visiting = {}
    local visited = {}
    local function visit(moduleName)
        if visited[moduleName] or visiting[moduleName] then
            return
        end

        visiting[moduleName] = true
        for _, dependency in ipairs(dependencies[moduleName] or {}) do
            if modules[dependency] then
                visit(dependency)
            end
        end
        visiting[moduleName] = nil
        visited[moduleName] = true
        table.insert(ordered, modules[moduleName])
    end

    for moduleName in pairs(modules) do
        visit(moduleName)
    end

    return ordered
end

local function moduleBuildCommands()
    local commands = {}
    local moduleDirectory = "%{cfg.objdir}/modules"
    table.insert(commands, "mkdir -p " .. moduleDirectory)
    for _, filename in ipairs(orderedModuleFiles()) do
        local moduleName = nil
        local file = io.open(filename, "r")
        if file then
            moduleName = file:read("*all"):match("export%s+module%s+([%w%._:]+)%s*;")
            file:close()
        end

        if moduleName then
            local relativePath = filename:gsub("^" .. _MAIN_SCRIPT_DIR .. "/", "../")
            local moduleOutputName = moduleName:gsub(":", "-")
            table.insert(commands,
                "$(CXX) $(ALL_CXXFLAGS) --precompile -x c++-module " .. relativePath .. " -o " .. moduleDirectory .. "/" ..
                    moduleOutputName .. ".pcm")
        end
    end
    return commands
end

local function gccModuleBuildCommands()
    local commands = {}
    table.insert(commands, "rm -rf gcm.cache")
    table.insert(commands, "mkdir -p gcm.cache")
    for _, filename in ipairs(orderedModuleFiles()) do
        local relativePath = filename:gsub("^" .. _MAIN_SCRIPT_DIR .. "/", "../")
        table.insert(commands,
            "$(CXX) $(ALL_CXXFLAGS) -g0 -fmodule-only -x c++ -c " .. relativePath)
    end
    return commands
end

local function webModuleBuildCommands()
    local commands = {}
    local moduleDirectory = _MAIN_SCRIPT_DIR .. "/Build/bin-int/" .. outputdir .. "/ElevateEngine/modules"
    local includeFlags = "-I\"%~dp0..\\ElevateEngine\\Source\" -I\"%~dp0..\\ElevateEngine\\Vendor\" " ..
                             "-I\"%~dp0..\\ElevateEngine\\Vendor\\glm\" -I\"%~dp0..\\ElevateEngine\\Vendor\\spdlog\\include\" " ..
                             "-I\"%~dp0..\\ElevateEngine\\Vendor\\rapidjson\\include\" " ..
                             "-I\"%~dp0..\\ElevateEngine\\Vendor\\entt\\include\" " ..
                             "-I\"%~dp0..\\ElevateEngine\\Vendor\\stduuid\\include\" " ..
                             "-I\"%~dp0..\\ElevateEngine\\Vendor\\assimp\\include\" " ..
                             "-I\"%~dp0..\\ElevateEngine\\Vendor\\JoltPhysics\""
    local scriptPath = _MAIN_SCRIPT_DIR .. "/Build/precompile_web_modules.bat"
    local script = io.open(scriptPath, "w")

    if not script then
        error("Unable to create Emscripten module prebuild script: " .. scriptPath)
    end

    script:write("@echo off\r\n")
    script:write("set \"MODULE_DIR=%~f1\"\r\n")
    script:write("if not exist \"%MODULE_DIR%\" mkdir \"%MODULE_DIR%\"\r\n")
    script:write("set \"MODULE_FLAGS=%MODULE_DIR%\\module-flags.rsp\"\r\n")
    script:write("if exist \"%MODULE_FLAGS%\" del /q \"%MODULE_FLAGS%\"\r\n")
    script:write("copy /y NUL \"%MODULE_FLAGS%\" >NUL\r\n")
    script:write("if not exist \"%MODULE_FLAGS%\" exit /b 1\r\n")
    for _, filename in ipairs(orderedModuleFiles()) do
        local file = io.open(filename, "r")
        local moduleName = nil
        if file then
            moduleName = file:read("*all"):match("export%s+module%s+([%w%._:]+)%s*;")
            file:close()
        end

        if moduleName then
            local moduleOutputName = moduleName:gsub(":", "-")
            local scriptOutput = "%MODULE_DIR%/" .. moduleOutputName .. ".pcm"
            local scriptSource = normalizePath(filename):gsub("/", "\\")
            script:write("em++ @\"%MODULE_FLAGS%\" -std=c++20 -DSPDLOG_COMPILED_LIB -DEE_ENGINE_BUILD -DEE_PLATFORM_WEB -DEE_EDITOR_BUILD " ..
                             includeFlags .. " -x c++-module -fprebuilt-module-path=\"%MODULE_DIR%\" " ..
                             "--precompile \"" .. scriptSource .. "\" -o \"" .. scriptOutput .. "\"\r\n")
            script:write("if errorlevel 1 exit /b %errorlevel%\r\n")
            script:write(">>\"%MODULE_FLAGS%\" echo -fmodule-file=" .. moduleName .. "=%MODULE_DIR%/" ..
                             moduleOutputName .. ".pcm\r\n")
        end
    end
    script:write("if not exist \"%MODULE_FLAGS%\" exit /b 1\r\n")
    script:write("exit /b 0\r\n")
    script:close()

    local windowsScriptPath = scriptPath:gsub("/", "\\")
    table.insert(commands, "cmd.exe /d /c \"\"" .. windowsScriptPath .. "\" \"" .. moduleDirectory .. "\"\"")
    return commands
end

project "ElevateEngine"
location "./Build"
kind "StaticLib"
language "C++"
cppdialect "C++20"
staticruntime "on"
systemversion "latest"

targetdir("%{wks.location}/Build/bin/" .. outputdir .. "/%{prj.name}")
objdir("%{wks.location}/Build/bin-int/" .. outputdir .. "/%{prj.name}")

IncludeDir = {}
IncludeDir["Vendors"] = "Vendor"
IncludeDir["GLFW"] = "Vendor/GLFW/include"
IncludeDir["Glad"] = "Vendor/Glad/include"
IncludeDir["ImGui"] = "Vendor/ImGui/"
IncludeDir["glm"] = "Vendor/glm/"
IncludeDir["rapidjson"] = "Vendor/rapidjson/include"
IncludeDir["rapidxml"] = "Vendor/rapidxml/include"
IncludeDir["stb"] = "Vendor/stb/"
IncludeDir["spdlog"] = "Vendor/spdlog/include"
IncludeDir["assimp"] = "Vendor/assimp/include"
IncludeDir["tinyfiledialogs"] = "Vendor/tinyfiledialogs"
IncludeDir["ImGuizmo"] = "Vendor/ImGuizmo/src"
IncludeDir["entt"] = "Vendor/entt/include"
IncludeDir["stduuid"] = "Vendor/stduuid/include"
IncludeDir["Jolt"] = "Vendor/JoltPhysics"

files {"Source/**.ixx", "Source/**.cpp", "Source/**.h", "Source/**.inl", "Vendor/ImGuizmo/src/ImGuizmo.cpp",
       "Vendor/ImGuiFileDialog/**.cpp", "Vendor/tinyfiledialogs/tinyfiledialogs.c",
       "Vendor/spdlog/src/*.cpp"}

defines {"EE_RESOURCE_DIR=\"" .. path.getabsolute("./Resources/") .. "\"", "EE_ENGINE_BUILD", "_CRT_SECURE_NO_WARNINGS",
         "IMGUI_DEFINE_MATH_OPERATORS", "GLM_ENABLE_EXPERIMENTAL", "SPDLOG_COMPILED_LIB"}

includedirs {"Source", values(IncludeDir)}

links {"ImGui", "assimp", "Jolt"}

Wwise.SetupEngine()
filter "not system:emscripten"
links {"GLFW"}
files {"Vendor/Glad/src/glad.c"}
filter {}

BuildPlatform.SetPlatformDefines()

filter "system:windows"
links {"opengl32.lib"}

filter {"system:linux", "language:C++"}
links {"GL", "X11", "pthread", "dl"}
if (_OPTIONS and _OPTIONS["compiler"]) == "gcc" then
    buildoptions {"-fmodules-ts", "-fno-module-lazy"}
else
    buildoptions {"-fprebuilt-module-path=%{cfg.objdir}/modules"}
end

filter "system:linux"
if (_OPTIONS and _OPTIONS["compiler"]) == "gcc" then
    prebuildcommands(gccModuleBuildCommands())
else
    prebuildcommands(moduleBuildCommands())
end

filter "system:emscripten"
local webModuleDirectory = _MAIN_SCRIPT_DIR .. "/Build/bin-int/" .. outputdir .. "/ElevateEngine/modules"
prebuildcommands(webModuleBuildCommands())
buildoptions {"-fprebuilt-module-path=" .. webModuleDirectory,
              "@\"" .. webModuleDirectory .. "/module-flags.rsp\""}

filter {"system:emscripten", "files:**.ixx"}
buildoptions {"-x", "c++-module"}

filter "configurations:Editor_Debug"
defines {"EE_DEBUG", "EE_EDITOR_BUILD"}
runtime "Debug"
symbols "on"

filter "configurations:Editor_Release"
defines {"EE_RELEASE", "EE_EDITOR_BUILD"}
runtime "Release"
optimize "on"

filter "configurations:Debug"
defines "EE_DEBUG"
runtime "Debug"
symbols "on"
links {"ws2_32"}

filter "configurations:Release"
defines "EE_RELEASE"
runtime "Release"
optimize "on"

filter "configurations:Dist"
defines "EE_DIST"
runtime "Release"
optimize "on"

filter {}

print("Finished Generating Engine Solution.\n")
