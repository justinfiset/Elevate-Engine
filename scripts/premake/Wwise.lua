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

    print("Wwise SDK found at : "..wwiseSDK)
	if not os.isdir(wwiseSDK) then
		error("ERROR : Wwise SDK folder, no such folder exists.")
	end
	print("Wwise Include found at : "..wwiseIncludePath)
	if not os.isdir(wwiseIncludePath) then
		error("ERROR : Wwise Include Folder, no such folder exists.")
	end

    print("")
end

function Wwise.IsInstalled()
    return wwiseSDK ~= nil and os.isdir(wwiseSDK)
end

function Wwise.SetupWorkspace()
    if not Wwise.IsInstalled() then return end

    print("Creating Wwise Solutions...")

    defines
    {
        "TESTING_WWISE_PROJECT_DATABASE",
        "EE_USES_WWISEPROJECTDATABASE"
    }

    group "Dependencies/Wwise"
        project "WwiseProjectDatabase"
            kind "StaticLib"
            language "C++"
            cppdialect "C++20"
            staticruntime "on"

            configmap {
                ["Editor Debug"]   = "Debug(StaticCRT)",
                ["Editor Release"] = "Release(StaticCRT)",
                ["Dist"]           = "Release(StaticCRT)"
            }
            
            local basePath = wwiseSDK .. "/samples/WwiseProjectDatabase"
            local sourcePath = basePath .. "/WwiseProjectDatabase"

            targetdir ("./Build/bin/" .. outputdir .. "/%{prj.name}")
            objdir ("./Build/bin-int/" .. outputdir .. "/%{prj.name}")

            files {
                basePath .. "/*.h",
                basePath .. "/*.cpp",
                basePath .. "/WwiseProjectDatabase/**.h",
                basePath .. "/WwiseProjectDatabase/**.cpp",
            }

            includedirs {
                sourcePath,
                wwiseSDK .. "/include",
                wwiseSDK .. "/samples/3rdParty/subprojects/rapidjson/include"
            }

            defines { 
                "WIN32_LEAN_AND_MEAN", 
                "WWISE_DB_STANDARD_TYPES",
            }

            filter "configurations:Debug*"
                defines { "_DEBUG" }
                runtime "Debug"
                symbols "on"

            filter "configurations:Release*"
                defines { "NDEBUG" }
                runtime "Release"
                optimize "on"
        
        group "" 

        print("Finished Generating WwiseProjectDatabase Solution.\n")
end

function Wwise.SetupEngine()
    if not Wwise.IsInstalled() then return end

    print("Setting up Wwise in Engine...")

    links { "WwiseProjectDatabase" }

    -- If Wwise is installed, we compile the engine using the Wwise SDK and SoundEngine
    defines { "EE_USES_WWISE" }

    local wwiseSDKSoundEngineSamplesSrc = path.getabsolute(wwiseSDK.."/samples/SoundEngine")
	local wwiseSDKSoundEngineSampleDest = path.getabsolute("src/ElevateEngine/Audio/Ak")

    local samplesPlatform
	if os.istarget("Windows") then
		samplesPlatform = "Win32"
	else
		samplesPlatform = "POSX"
	end

	includedirs
	{
		wwiseIncludePath, -- include the Ak include path
		wwiseSDKSoundEngineSampleDest.."/"..samplesPlatform, -- include the Ak sample/SoundEngine include path
		wwiseSDK.."/samples",
		wwiseSDK.."/samples/3rdParty/subprojects",
		wwiseSDK.."/samples/WwiseProjectDatabase/WwiseProjectDatabase"
    }

    print("Build Commands Summary :")
	print(" + Copying Wwise SoundEngine Samples from "..wwiseSDKSoundEngineSamplesSrc.." to "..wwiseSDKSoundEngineSampleDest)
	print("    + Copying /Common")
	os.execute("{COPYDIR} "..wwiseSDKSoundEngineSamplesSrc.."/Common "..wwiseSDKSoundEngineSampleDest.."/Common") -- Copy the Soundengine samples from Wwise)
	print("    > DONE")
	print("    + Copying /"..samplesPlatform)
	os.execute("{COPYDIR} "..wwiseSDKSoundEngineSamplesSrc.."/"..samplesPlatform.." "..wwiseSDKSoundEngineSampleDest.."/"..samplesPlatform) -- Copy the Soundengine samples from Wwise)
	print("    > DONE")

	Wwise.SetupLibDirs()

    print("Finished setting up Wwise in Engine.\n")
end

function Wwise.SetupLibDirs()
    if not Wwise.IsInstalled() then return end

    print("Setting up Wwise library directories...")

    filter "configurations:*Debug*"
        libdirs { wwiseSDK .. "/x64_vc170/Debug(StaticCRT)/lib" }
        libdirs { wwiseSDK .. "/x64_vc170/Debug(StaticCRT)/bin" }
    filter "configurations:*Release*"
        libdirs { wwiseSDK .. "/x64_vc170/Release(StaticCRT)/lib" }
        libdirs { wwiseSDK .. "/x64_vc170/Release(StaticCRT)/bin" }
    filter {}

    print("Finished setting up Wwise library directories.\n")
end

function Wwise.SetupProject()
    if (not Wwise.IsInstalled()) then 
        return 
    end

    dependson { "WwiseProjectDatabase" }

    libdirs { "./Build/bin/" .. outputdir .. "/WwiseProjectDatabase" }

    Wwise.SetupLibDirs()

    includedirs
    {
        wwiseIncludePath,
        wwiseSDK .. "/samples",
        wwiseSDK .. "/samples/SoundEngine/Common/",
        wwiseSDK .. "/samples/SoundEngine/Win32/",
		wwiseSDK .. "/samples/3rdParty/subprojects/rapidjson/include",
		wwiseSDK .. "/samples/WwiseProjectDatabase/WwiseProjectDatabase/",
    }

    defines { "EE_USES_WWISE" }

    links
    {
        "WwiseProjectDatabase", 
        "AkSoundEngine",
        "AkMemoryMgr",
        "AkStreamMgr",
        "AkSpatialAudio",
        "CommunicationCentral", -- Not needed for release config -- TODO CHANGE THIS
        "AkVorbisDecoder"
    }
end