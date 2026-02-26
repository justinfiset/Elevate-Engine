local wwiseSDK = os.getenv("WWISESDK")
local wwiseIncludePath = wwiseSDK .. "/include"

project "ElevateEngine"
	location "Build"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin/temps")

	flags { "NoPCH" }
	-- pchheader "eepch.h"
	-- pchsource "src/eepch.cpp"

	IncludeDir = {}
	IncludeDir["Vendors"] = "vendor"
	IncludeDir["GLFW"] = "vendor/GLFW/include" 
	IncludeDir["Glad"] = "vendor/Glad/include"
	IncludeDir["ImGui"] = "vendor/ImGui/"
	IncludeDir["glm"] = "vendor/glm/"
	IncludeDir["rapidjson"] = "vendor/rapidjson/include"
	IncludeDir["rapidxml"] = "vendor/rapidxml/include"
	IncludeDir["stb"] = "vendor/stb/"
	IncludeDir["spdlog"] = "vendor/spdlog/include"
	IncludeDir["assimp"] = "vendor/assimp/include"
	IncludeDir["tinyfiledialogs"] = "vendor/tinyfiledialogs"
	IncludeDir["ImGuizmo"] = "vendor/ImGuizmo"
	IncludeDir["entt"] = "vendor/entt/include"

	print("Wwise SDK Path : "..wwiseSDK)
	if not os.isdir(wwiseSDK) then
		error("ERROR : Wwise SDK folder, no such folder exists.")
	end
	print("Wwise Include Path : "..wwiseIncludePath)
	if not os.isdir(wwiseIncludePath) then
		error("ERROR : Wwise Include Folder, no such folder exists.")
	end
	-- TODO MAKE THIS PATH DYNAMIC AND NOT HARD CODED - LIKE THIS FOR TEST AND LEARNING PURPOSES
	local wwiseLibLinkPath = wwiseSDK .. "/x64_vc170/Debug(StaticCRT)/lib"
	local wwiseBinLinkPath = wwiseSDK .. "/x64_vc170/Debug(StaticCRT)/bin"

	-- SoundEngine samples
	local wwiseSDKSoundEngineSamplesSrc = path.getabsolute(wwiseSDK.."/samples/SoundEngine")
	local wwiseSDKSoundEngineSampleDest = path.getabsolute("src/ElevateEngine/Audio/Ak")

	-- WAAPI samples
	-- local wwiseSDKWAAPISampleSrc = path.getabsolute(wwiseSDK.."/samples/WwiseAuthoringAPI/cpp/SampleClient/AkAutobahn")
	-- local wwiseSDKWAAPISampleDest = path.getabsolute("src/ElevateEngine/Audio/Ak/AkAutobahn")
	
	local samplesPlatform
	if os.istarget("Windows") then
		samplesPlatform = "Win32"
	else
		samplesPlatform = "POSX"
	end

	files 
	{
		"src/**.h",
		"src/**.inl",
		"src/**.cpp",

		"src/**.vert",
		"src/**.frag",

		"vendor/ImGuizmo/ImGuizmo.cpp",
		"vendor/Glad/src/glad.c",
		"vendor/tinyfiledialogs/tinyfiledialogs.cpp",
	}

	defines 
	{
		"EE_ENGINE_BUILD",
		"_CRT_SECURE_NO_WARNINGS",
		"IMGUI_DEFINE_MATH_OPERATORS",
	}

	includedirs
	{
		"src",

		-- todo add conditions to make sure we really are using wwise.
		wwiseIncludePath, -- include the Ak include path
		wwiseSDKSoundEngineSampleDest.."/"..samplesPlatform, -- include the Ak sample/SoundEngine include path
		wwiseSDK.."/samples",
		wwiseSDK.."/samples/3rdParty/subprojects",
		wwiseSDK.."/samples/WwiseProjectDatabase/WwiseProjectDatabase",

		"%{IncludeDir.Vendors}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.rapidjson}",
		"%{IncludeDir.rapidxml}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.tinyfiledialogs}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.entt}"
	}

	-- todo : add a condition before doing so, make sure we want to compile with the soundengine
	print("Build Commands Summary :")
	print(" + Copying Wwise SoundEngine Samples from "..wwiseSDKSoundEngineSamplesSrc.." to "..wwiseSDKSoundEngineSampleDest)
	print("    + Copying /Common")
	os.execute("{COPYDIR} "..wwiseSDKSoundEngineSamplesSrc.."/Common "..wwiseSDKSoundEngineSampleDest.."/Common") -- Copy the Soundengine samples from Wwise)
	print("    > DONE")
	print("    + Copying /"..samplesPlatform)
	os.execute("{COPYDIR} "..wwiseSDKSoundEngineSamplesSrc.."/"..samplesPlatform.." "..wwiseSDKSoundEngineSampleDest.."/"..samplesPlatform) -- Copy the Soundengine samples from Wwise)
	print("    > DONE")

	-- todo : add a condition before doing so, make sure we want to compile with waapi
	-- print(" + Copying Wwise WAAPI Sample from "..wwiseSDKWAAPISampleSrc.." to "..wwiseSDKWAAPISampleDest)
	-- os.execute("{COPYDIR} "..wwiseSDKWAAPISampleSrc.." "..wwiseSDKWAAPISampleDest) -- Copy the WAAPI samples from Wwise)
	-- print("    > DONE")

	libdirs
	{
		wwiseLibLinkPath,
		wwiseBinLinkPath,
	}

	links
	{
		"GLFW",
		"ImGui",
		"assimp",

		"AkSoundEngine",
		"AkMemoryMgr",
		"AkStreamMgr",
		"AkSpatialAudio",
		"CommunicationCentral", -- Not needed for release config
		"AkVorbisDecoder",
		"AkOpusDecoder",
	}

	filter "system:windows"
		systemversion "latest"
		buildoptions
		{
			"/Zc:wchar_t",
			"/Zc:preprocessor",
			"/Zc:__cplusplus"
		}

		defines
		{
			"EE_PLATFORM_WINDOWS",
		}

		links
		{
			"opengl32.lib",
		}

	-- All of the debug configs on Windows

	filter "system:linux"
		systemversion "latest"

		defines
		{
			"EE_PLATFORM_LINUX",
			"STBI_NO_SIMD"
		}
		
		links
		{
			"GL",
			"GLU",
			"X11",
			"dl",
			"pthread",
		}

	filter "configurations:Editor Debug"
		defines
		{
			"EE_DEBUG",
			"EE_EDITOR_BUILD"
		}

		links
		{
			"AkAutobahn",
			"ws2_32"
		}

		runtime "Debug"
		symbols "on"

	 filter "configurations:Editor Release"
		defines 
		{
			"EE_RELEASE",
			"EE_EDITOR_BUILD"
		}
		
		links
		{
			"AkAutobahn",
			"ws2_32"
		}

		runtime "Release"
		optimize "on"

	filter "configurations:Debug"
		defines "EE_DEBUG"
		runtime "Debug"
		symbols "on"

		links
		{
			"ws2_32"
		}

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