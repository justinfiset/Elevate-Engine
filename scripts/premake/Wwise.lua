local sdkPath = os.getenv("WWISESDK")

wwiseSDK = nil
if sdkPath then
    wwiseSDK = sdkPath
end

wwiseIncludePath = nil
if wwiseSDK then
    wwiseIncludePath = wwiseSDK .. "/include"
end

Wwise = {}

function Wwise.Initialize()
    if not Wwise.IsInstalled() then
        print("Wwise SDK not found. Skipping Wwise integration.")
        return
    end

    print("Wwise SDK found at : " .. wwiseSDK)
    if not os.isdir(wwiseSDK) then
        error("ERROR : Wwise SDK folder, no such folder exists.")
    end
    print("Wwise Include found at : " .. wwiseIncludePath)
    if not os.isdir(wwiseIncludePath) then
        error("ERROR : Wwise Include Folder, no such folder exists.")
    end

    print("")
end

function Wwise.IsInstalled()
    return wwiseSDK ~= nil and os.isdir(wwiseSDK)
end

function Wwise.SetupWorkspace()
    if not Wwise.IsInstalled() then
        return
    end

    print("Creating Wwise Solutions...")

    defines {"TESTING_WWISE_PROJECT_DATABASE", "EE_USES_WWISEPROJECTDATABASE"}

    group "Dependencies/Wwise"
    project "WwiseProjectDatabase"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"      
    staticruntime "on"

    configmap {
        ["Editor_Debug"] = "DebugStaticCRT",
        ["Editor_Release"] = "ReleaseStaticCRT)",
        ["Dist"] = "ReleaseStaticCRT"
    }

    local basePath = wwiseSDK .. "/samples/WwiseProjectDatabase"
    local sourcePath = basePath .. "/WwiseProjectDatabase"

    targetdir("./Build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("./Build/bin-int/" .. outputdir .. "/%{prj.name}")

    files {basePath .. "/*.h", basePath .. "/*.cpp", basePath .. "/WwiseProjectDatabase/**.h",
           basePath .. "/WwiseProjectDatabase/**.cpp"}

    includedirs {sourcePath, wwiseSDK .. "/include", wwiseSDK .. "/samples/3rdParty/subprojects/rapidjson/include"}

    defines {"WIN32_LEAN_AND_MEAN", "WWISE_DB_STANDARD_TYPES"}

    filter "configurations:Debug*"
    defines {"_DEBUG"}
    runtime "Debug"
    symbols "on"

    filter "configurations:Release*"
    defines {"NDEBUG"}
    runtime "Release"
    optimize "on"

    group ""

    print("Finished Generating WwiseProjectDatabase Solution.\n")
end

function Wwise.GetSamplePlatform()
    local samplesPlatform = nil
    if os.istarget("Windows") then
        samplesPlatform = "Win32"
    elseif os.istarget("Linux") then
        samplesPlatform = "POSX"
    elseif os.istarget("emscripten") then
        samplesPlatform = "POSIX"
    end
    return samplesPlatform
end

function Wwise.SetupBuildOption()
    filter "system:emscripten"
        buildoptions { "-msimd128", "-msse2" }
    filter {}
end

function Wwise.SetupLinks()
    links 
    { 
        "WwiseProjectDatabase",
        "AkSoundEngine",
        "AkMemoryMgr",
        "AkStreamMgr",
        "AkSpatialAudio",
        "AkVorbisDecoder",
        "AkOpusDecoder"
    }

    filter { "configurations:*Debug*" }
        links {"CommunicationCentral"}

    filter { "configurations:*Debug*", "system:windows" }
        links {"ws2_32"}

    filter { "configurations:Editor_Debug", "not system:emscripten" }
        links {"AkAutobahn"}

    filter {}
end

function Wwise.SetupEngine()
    if not Wwise.IsInstalled() then
        return
    end

    print("Setting up Wwise in Engine...")

    -- If Wwise is installed, we compile the engine using the Wwise SDK and SoundEngine
    defines {"EE_USES_WWISE"}

    local wwiseSDKSoundEngineSamplesSrc = path.getabsolute(wwiseSDK .. "/samples/SoundEngine")
    local wwiseSDKSoundEngineSampleDest = path.getabsolute("src/ElevateEngine/Audio/Ak")

    local samplesPlatform = Wwise.GetSamplePlatform()
    if not samplesPlatform then
        error("Unsupported platform for Wwise SoundEngine samples.")
    end

    includedirs
    {
        wwiseIncludePath, -- include the Ak include path
        wwiseSDKSoundEngineSampleDest .. "/" .. samplesPlatform, -- include the Ak sample/SoundEngine include path
        wwiseSDK .. "/samples", wwiseSDK .. "/samples/3rdParty/subprojects",
        wwiseSDK .. "/samples/WwiseProjectDatabase/WwiseProjectDatabase",

        wwiseSDK .. "/samples/SoundEngine/Common/",
        wwiseSDK .. "/samples/SoundEngine/"..samplesPlatform.."/",
    }

    print("Build Commands Summary :")
    FileUtils.SafeDelete(wwiseSDKSoundEngineSampleDest)
    print(" + Copying Wwise SoundEngine Samples from " .. wwiseSDKSoundEngineSamplesSrc .. " to " ..
              wwiseSDKSoundEngineSampleDest)
    print("    + Copying /Common")
    FileUtils.SafeCopy(wwiseSDKSoundEngineSamplesSrc .. "/Common", wwiseSDKSoundEngineSampleDest .. "/Common")
    print("    > DONE")
    print("    + Copying /" .. samplesPlatform)
    FileUtils.SafeCopy(wwiseSDKSoundEngineSamplesSrc .. "/" .. samplesPlatform, wwiseSDKSoundEngineSampleDest .. "/" .. samplesPlatform)
    print("    > DONE")

    filter { "configurations:Editor_Debug", "not system:emscripten" }
        links {"AkAutobahn", "ws2_32"}
    filter { "configurations:Editor_Release", "not system:emscripten" }
        links {"AkAutobahn", "ws2_32"}
    filter {}

    Wwise.SetupLibDirs()
    Wwise.SetupLinks()
    Wwise.SetupBuildOption()

    print("Finished setting up Wwise in Engine.\n")
end

function Wwise.SetupLibDirs()
    if not Wwise.IsInstalled() then return end

    print("Setting up Wwise library directories...")

    -- WINDOWS
    filter { "system:windows", "configurations:*Debug*" }
        libdirs { wwiseSDK .. "/x64_vc170/Debug(StaticCRT)/lib", wwiseSDK .. "/x64_vc170/Debug(StaticCRT)/bin" } 
    filter { "system:windows", "configurations:*Release*" }
        libdirs { wwiseSDK .. "/x64_vc170/Release(StaticCRT)/lib", wwiseSDK .. "/x64_vc170/Release(StaticCRT)/bin" }

    -- LINUX
    filter { "system:linux", "configurations:*Debug*" }
        libdirs { wwiseSDK .. "/Linux_x64/Debug/lib", wwiseSDK .. "/Linux_x64/Debug/bin" }
    filter { "system:linux", "configurations:*Release*" }
        libdirs { wwiseSDK .. "/Linux_x64/Release/lib", wwiseSDK .. "/Linux_x64/Release/bin" }

    -- EMSCRIPTEN (WEB)
    filter { "system:emscripten", "configurations:*Debug*" }
        libdirs { wwiseSDK .. "/Emscripten_mt/Debug/lib", wwiseSDK .. "/Emscripten_mt/Debug/bin" }
    filter { "system:emscripten", "configurations:*Release*" }
        libdirs { wwiseSDK .. "/Emscripten_mt/Release/lib", wwiseSDK .. "/Emscripten_mt/Release/bin" }

    filter {}

    print("Finished setting up Wwise library directories.\n")
end

function Wwise.SetupProject()
    if (not Wwise.IsInstalled()) then
        return
    end

    dependson {"WwiseProjectDatabase"}

    libdirs {"./Build/bin/" .. outputdir .. "/WwiseProjectDatabase"}

    Wwise.SetupLibDirs()

    samplesPlatform = Wwise.GetSamplePlatform()
    if not samplesPlatform then
        error("Unsupported platform for Wwise SoundEngine samples.")
    end

    includedirs 
    {
        wwiseIncludePath,
        wwiseSDK .. "/samples",
        wwiseSDK .. "/samples/SoundEngine/Common/",
        wwiseSDK .. "/samples/SoundEngine/"..samplesPlatform.."/",
        wwiseSDK .. "/samples/3rdParty/subprojects/rapidjson/include",
        wwiseSDK .. "/samples/WwiseProjectDatabase/WwiseProjectDatabase/"
    }

    defines {"EE_USES_WWISE"}

    Wwise.SetupLinks()
    Wwise.SetupBuildOption()
end
