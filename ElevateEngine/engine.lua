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
        for importedModule in content:gmatch("import%s+([:%w%._]+)%s*;") do
            table.insert(imports, importedModule)
        end
        return imports
    end

    table.sort(moduleFiles)

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

                local function addDependency(importedModule)
                    if importedModule:sub(1, 1) == ":" then
                        importedModule = moduleBase .. importedModule
                    end

                    table.insert(dependencies[moduleName], importedModule)
                end

                for _, importedModule in ipairs(collectImports(content)) do
                    addDependency(importedModule)
                end

                for includePath in content:gmatch('#include%s+[<"]([^>"]+)[>"]') do
                    for headerFilename, imports in pairs(headerImports) do
                        if headerFilename:sub(-#includePath) == includePath then
                            for _, importedModule in ipairs(imports) do
                                addDependency(importedModule)
                            end
                        end
                    end
                end
            end
        end
    end

    -- Topological sorting and dependency level calculation.
    local ordered = {}
    local visiting = {}
    local visited = {}
    local levels = {}

    local function visit(moduleName)
        if visited[moduleName] then
            return levels[moduleName]
        end

        if visiting[moduleName] then
            error("Circular C++ module dependency involving " .. moduleName)
        end

        visiting[moduleName] = true

        local level = 0

        for _, dependency in ipairs(dependencies[moduleName] or {}) do
            if modules[dependency] then
                level = math.max(level, visit(dependency) + 1)
            end
        end

        visiting[moduleName] = nil
        visited[moduleName] = true

        levels[moduleName] = level
        table.insert(ordered, modules[moduleName])

        return level
    end

    local moduleNames = {}

    for moduleName in pairs(modules) do
        table.insert(moduleNames, moduleName)
    end

    table.sort(moduleNames)

    for _, moduleName in ipairs(moduleNames) do
        visit(moduleName)
    end

    -- Map source files to their dependency levels.
    local levelByFile = {}

    for moduleName, filename in pairs(modules) do
        levelByFile[filename] = levels[moduleName]
    end

    return ordered, levelByFile
end

function UseEngineModules()
    filter "system:linux"
    if (_OPTIONS and _OPTIONS["compiler"]) == "gcc" then
        buildoptions {"-fmodule-mapper=" .. normalizePath(_MAIN_SCRIPT_DIR .. "/ElevateEngine/Build/module.map")}
    else
        buildoptions {"-fprebuilt-module-path=%{wks.location}/Build/bin-int/" .. outputdir .. "/ElevateEngine/modules"}
    end
    filter "system:emscripten"
    buildoptions {"-fprebuilt-module-path=" .. _MAIN_SCRIPT_DIR .. "/Build/bin-int/" .. outputdir ..
        "/ElevateEngine/modules", "-msimd128", "-msse4.1"}
    filter {}
end

local function moduleBuildCommands()
    local commands = {}
    local moduleDirectory = "%{cfg.objdir}/modules"
    local compiler = "$(CXX)"
    local moduleFlags = "$(ALL_CXXFLAGS)"

    local buildDirectory = normalizePath(_MAIN_SCRIPT_DIR .. "/ElevateEngine/Build")

    os.mkdir(buildDirectory)

    local ordered, levelByFile = orderedModuleFiles()
    local levels = {}
    local maxLevel = -1

    -- Group modules by dependency level.
    for _, filename in ipairs(ordered) do
        local moduleName = moduleNameForFile(filename)

        if moduleName then
            local level = levelByFile[filename] or 0
            local outputName = moduleName:gsub(":", "-")

            levels[level] = levels[level] or {}

            table.insert(levels[level], {
                source = normalizePath(filename),
                output = outputName
            })

            maxLevel = math.max(maxLevel, level)
        end
    end

    table.insert(commands, 'mkdir -p "' .. moduleDirectory .. '"')

    -- Precompile modules in parallel, respecting dependency levels.
    local objectListPath = buildDirectory .. "/clang-module-objects.list"
    local objectListFile = io.open(objectListPath, "w")

    if not objectListFile then
        error("Unable to create Clang module object list at " .. objectListPath)
    end

    for level = 0, maxLevel do
        local modulesAtLevel = levels[level]

        if modulesAtLevel then
            local listPath = buildDirectory .. "/clang-modules-level" .. level .. ".list"
            local listFile = io.open(listPath, "w")

            if not listFile then
                error("Unable to create Clang module list at " .. listPath)
            end

            for _, entry in ipairs(modulesAtLevel) do
                listFile:write(entry.source, "\n")
                listFile:write(entry.output, "\n")

                objectListFile:write(entry.output, "\n")
                objectListFile:write(entry.output, "\n")
            end

            listFile:close()

            local compileCommand = "xargs -d '\\n' -n 2 -P \"$$(nproc)\" sh -c '" .. compiler .. " " .. moduleFlags ..
                                       " --precompile -x c++-module \"$$1\" " .. "-o \"" .. moduleDirectory ..
                                       "/$$2.pcm\"' _ " .. "< \"" .. listPath .. "\" || exit 1"

            table.insert(commands, compileCommand)
        end
    end

    objectListFile:close()

    -- Compile all generated PCMs into object files in parallel.
    local objectCommand =
        "xargs -d '\\n' -n 2 -P \"$$(nproc)\" sh -c '" .. compiler .. " " .. moduleFlags .. " -c \"" .. moduleDirectory ..
            "/$$1.pcm\" " .. "-o \"" .. moduleDirectory .. "/$$2.o\"' _ " .. "< \"" .. objectListPath .. "\" || exit 1"

    table.insert(commands, objectCommand)

    return commands
end

local function webModuleBuildCommands()
    local commands = {}
    local moduleDirectory = _MAIN_SCRIPT_DIR .. "/Build/bin-int/" .. outputdir .. "/ElevateEngine/modules"
    local wwiseDefine = Wwise.IsInstalled() and " -DEE_USES_WWISE" or ""
    local wwiseIncludeFlags = ""
    if Wwise.IsInstalled() then
        wwiseIncludeFlags = " -I\"" .. wwiseSDK .. "/include\" " .. "-I\"" .. wwiseSDK .. "/samples\" " .. "-I\"" ..
                                wwiseSDK .. "/samples/SoundEngine/Common\" " .. "-I\"" .. wwiseSDK ..
                                "/samples/SoundEngine/POSIX\" " .. "-I\"" .. wwiseSDK ..
                                "/samples/3rdParty/subprojects/rapidjson/include\" " .. "-I\"" .. wwiseSDK ..
                                "/samples/WwiseProjectDatabase/WwiseProjectDatabase\""
    end
    local includeFlags = "-I\"%~dp0..\\ElevateEngine\\Source\" -I\"%~dp0..\\ElevateEngine\\Vendor\" " ..
                             "-I\"%~dp0..\\ElevateEngine\\Vendor\\_config_headers\" " ..
                             "-I\"%~dp0..\\ElevateEngine\\Vendor\\_config_headers\\assimp\" " ..
                             "-I\"%~dp0..\\ElevateEngine\\Vendor\\ImGui\" " ..
                             "-I\"%~dp0..\\ElevateEngine\\Vendor\\glm\" " ..
                             "-I\"%~dp0..\\ElevateEngine\\Vendor\\spdlog\\include\" " ..
                             "-I\"%~dp0..\\ElevateEngine\\Vendor\\rapidjson\\include\" " ..
                             "-I\"%~dp0..\\ElevateEngine\\Vendor\\entt\\include\" " ..
                             "-I\"%~dp0..\\ElevateEngine\\Vendor\\stduuid\\include\" " ..
                             "-I\"%~dp0..\\ElevateEngine\\Vendor\\assimp\\include\" " ..
                             "-I\"%~dp0..\\ElevateEngine\\Vendor\\JoltPhysics\"" .. wwiseIncludeFlags
    local scriptPath = _MAIN_SCRIPT_DIR .. "/Build/precompile_web_modules.bat"
    local script = io.open(scriptPath, "w")

    if not script then
        error("Unable to create Emscripten module prebuild script: " .. scriptPath)
    end

    script:write("@echo off\r\n")
    script:write("set \"MODULE_DIR=%~f1\"\r\n")
    script:write("set \"MODULE_DIR=%MODULE_DIR:\\=/%\"\r\n")
    script:write("if not exist \"%MODULE_DIR%\" mkdir \"%MODULE_DIR%\"\r\n")
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
            script:write(
                "call em++ -std=c++20 -DSPDLOG_COMPILED_LIB -DEE_ENGINE_BUILD -DEE_PLATFORM_WEB -DEE_EDITOR_BUILD" ..
                    wwiseDefine .. " " .. "-pthread -matomics -mbulk-memory -msimd128 -msse4.1 " ..
                    "-Wno-deprecated-literal-operator -Wno-invalid-offsetof -Wno-deprecated-pragma " .. includeFlags ..
                    " -x c++-module -fprebuilt-module-path=\"%MODULE_DIR%\" " .. "--precompile \"" .. scriptSource ..
                    "\" -o \"" .. scriptOutput .. "\"\r\n")
            script:write("if errorlevel 1 exit /b %errorlevel%\r\n")
        end
    end
    script:write("exit /b 0\r\n")
    script:close()

    local buildScriptPath = "Build/precompile_web_modules.bat"
    local buildModuleDirectory = "Build/bin-int/" .. outputdir .. "/ElevateEngine/modules"
    table.insert(commands, "cmd.exe //d //c \"call " .. buildScriptPath .. " " .. buildModuleDirectory .. "\"")
    return commands
end

local function moduleObjectFiles()
    local objectFiles = {}
    local ordered = orderedModuleFiles()

    for _, filename in ipairs(ordered) do
        local moduleName = moduleNameForFile(filename)

        if moduleName then
            local outputName = moduleName:gsub(":", "-")

            table.insert(objectFiles, "%{cfg.objdir}/modules/" .. outputName .. ".o")
        end
    end

    return objectFiles
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
       "Vendor/ImGuiFileDialog/**.cpp", "Vendor/tinyfiledialogs/tinyfiledialogs.c", "Vendor/spdlog/src/*.cpp"}

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
    buildoptions {"-fmodules-ts",
                  "-fmodule-mapper=" .. normalizePath(_MAIN_SCRIPT_DIR .. "/ElevateEngine/Build/module.map")}
else
    buildoptions {"-fprebuilt-module-path=%{cfg.objdir}/modules"}
end

filter "system:linux"
linkoptions {"-fuse-ld=lld"}

if (_OPTIONS and _OPTIONS["compiler"]) ~= "gcc" then
    files(moduleObjectFiles())
end

if (_OPTIONS and _OPTIONS["compiler"]) == "gcc" then
    prebuildcommands(gccModuleBuildCommands())
else
    prebuildcommands(moduleBuildCommands())
end

filter {"system:linux", "files:**.ixx"}

if (_OPTIONS and _OPTIONS["compiler"]) == "gcc" then
    buildaction "Compile"
    compileas "C++"
    buildoptions {"-x", "c++"}
else
    -- Clang compiles the generated PCM files into object files.
    buildaction "None"
end

filter {}

filter "system:emscripten"
local webModuleBuildDirectory = "Build/bin-int/" .. outputdir .. "/ElevateEngine/modules"
prebuildcommands(webModuleBuildCommands())
buildoptions {"-fprebuilt-module-path=" .. webModuleBuildDirectory, "-msimd128", "-msse4.1"}

filter {"system:emscripten", "files:**.ixx"}
buildoptions {"-x", "c++-module"}

filter "configurations:Editor_Debug"
defines {"EE_DEBUG", "EE_EDITOR_BUILD", "JPH_ENABLE_ASSERTS"}
runtime "Debug"
symbols "on"

filter "configurations:Editor_Release"
defines {"EE_RELEASE", "EE_EDITOR_BUILD", "NDEBUG"}
runtime "Release"
optimize "on"

filter "configurations:Debug"
defines {"EE_DEBUG", "JPH_ENABLE_ASSERTS"}
runtime "Debug"
symbols "on"
links {"ws2_32"}

filter "configurations:Release"
defines {"EE_RELEASE", "NDEBUG"}
runtime "Release"
optimize "on"

filter "configurations:Dist"
defines {"EE_DIST", "NDEBUG"}
runtime "Release"
optimize "on"

filter {}

print("Finished Generating Engine Solution.\n")
