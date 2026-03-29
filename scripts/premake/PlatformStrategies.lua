include "WebBuild.lua"

local currentPlatform = nil

WindowsStrategy = {
    ConfigureWorkspace = function(prj)
        architecture "x64"
        buildoptions
		{
			"/Zc:wchar_t",
			"/Zc:preprocessor",
			"/Zc:__cplusplus",
			"/utf-8"
		}
    end,

    SetPlatformDefines = function()
        defines { "EE_PLATFORM_WINDOWS" }
    end,
}

LinuxStrategy = {
    ConfigureWorkspace = function()
        architecture "x64"
    end,

    SetPlatformDefines = function()
        defines { "EE_PLATFORM_LINUX", "STBI_NO_SIMD" }
    end,
}

WebStrategy = {
    ConfigureWorkspace = function()
        architecture "wasm32"
        toolset "clang"
        WebBuild.ApplyCompilerSettings()
    end,

    ConfigureWorkspaceAction = function()
        WebBuild.CleanupBuildFiles()
    end,

    SetPlatformDefines = function()
        defines { "EE_PLATFORM_WEB" }
    end,

    ConfigureProject = function(directory, project)
        for _, configName in ipairs(ElevateConfigs) do
            local projectSafeName = CommonProject.GetSafeProjectName(project.name)
            local targetPath = _MAIN_SCRIPT_DIR .. "/Build/bin/" .. configName .. "-emscripten-wasm32/" .. projectSafeName
            os.mkdir(targetPath)
            WebBuild.GenerateHTML(project, targetPath)
        end
    end,
}

BuildPlatform = {}
function BuildPlatform.Init()
    if os.target() == "windows" then
        currentPlatform = WindowsStrategy
    elseif os.target() == "emscripten" then
        currentPlatform = WebStrategy
    elseif os.target() == "linux" then
        currentPlatform = LinuxStrategy
    end
end

function BuildPlatform.ConfigureWorkspace()
    BuildPlatform.Init()
    if currentPlatform and currentPlatform.ConfigureWorkspace then
        currentPlatform.ConfigureWorkspace()
    end
end

function BuildPlatform.ConfigureWorkspaceAction()
    if currentPlatform and currentPlatform.ConfigureWorkspaceAction then
        currentPlatform.ConfigureWorkspaceAction()
    end
end

function BuildPlatform.SetPlatformDefines()
    if currentPlatform and currentPlatform.SetPlatformDefines then
        currentPlatform.SetPlatformDefines()
    end
end

function BuildPlatform.ConfigureProject(directory, project)
    if currentPlatform and currentPlatform.ConfigureProject then
        currentPlatform.ConfigureProject(directory, project)
    end
end