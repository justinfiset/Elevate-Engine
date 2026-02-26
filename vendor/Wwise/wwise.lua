local wwiseSDK = os.getenv("WWISESDK")

Wwise = {}
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

        print(" > Finished Generating WwiseProjectDatabase Solution.\n")
end