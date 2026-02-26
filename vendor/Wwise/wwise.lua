wwiseSDK = os.getenv("WWISESDK")
wwiseIncludePath = wwiseSDK .. "/include"

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

    defines { "TESTING_WWISE_PROJECT_DATABASE" }

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

            targetdir ("bin/" .. outputdir .. "/%{prj.name}")
            objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

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