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
	IncludeDir["GLFW"] = "vendor/GLFW/include" 
	IncludeDir["Glad"] = "vendor/Glad/include"
	IncludeDir["ImGui"] = "vendor/ImGui/"
	IncludeDir["glm"] = "vendor/glm/"
	IncludeDir["rapidjson"] = "vendor/rapidjson/include"
	IncludeDir["stb"] = "vendor/stb/"
	IncludeDir["spdlog"] = "vendor/spdlog/include"
	IncludeDir["assimp"] = "vendor/assimp/include"
	IncludeDir["tinyfiledialogs"] = "vendor/tinyfiledialogs"
	IncludeDir["ImGuizmo"] = "vendor/ImGuizmo"
	IncludeDir["entt"] = "vendor/entt/include"

	local wwiseSDK = os.getenv("WWISESDK")
	local wwiseIncludePath = wwiseSDK .. "/include"
	print("Wwise SDK Path : "..wwiseSDK)
	if not os.isdir(wwiseSDK) then
		error("ERROR : Wwise SDK folder, no such folder exists.")
	end
	print("Wwise Include Path : "..wwiseIncludePath)
	if not os.isdir(wwiseIncludePath) then
		error("ERROR : Wwise Include Folder, no such folder exists.")
	end
	-- TODO MAKE THIS PATH DYNAMIC AND NOT HARD CODED - LIKE THIS FOR TEST AND LEARNING PURPOSES
	local wwiseLinkPath = wwiseSDK .. "/x64_vc170/Debug(StaticCRT)/lib"

	local wwiseSDKSoundEngineSamplesSrc = path.getabsolute(wwiseSDK.."/samples/SoundEngine")
	local wwiseSDKSoundEngineSampleDest = path.getabsolute("src/ElevateEngine/Audio/Ak")
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

		wwiseIncludePath, -- include the Ak include path
		wwiseSDKSoundEngineSampleDest.."/"..samplesPlatform, -- include the Ak sample/SoundEngine include path

		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.rapidjson}",
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
	print(" + Copy of Wwise SoundEngine Samples from "..wwiseSDKSoundEngineSamplesSrc.." to "..wwiseSDKSoundEngineSampleDest)
	print("    + Copying /Common")
	os.execute("{COPYDIR} "..wwiseSDKSoundEngineSamplesSrc.."/Common "..wwiseSDKSoundEngineSampleDest.."/Common") -- Copy the Soundengine samples from Wwise)
	print("    > DONE")
	print("    + Copying /"..samplesPlatform)
	os.execute("{COPYDIR} "..wwiseSDKSoundEngineSamplesSrc.."/"..samplesPlatform.." "..wwiseSDKSoundEngineSampleDest.."/"..samplesPlatform) -- Copy the Soundengine samples from Wwise)
	print("    > DONE")


	libdirs
	{
		wwiseLinkPath
	}

	links
	{
		-- "Glad", NOT A PROJECT ANYMORE
		"GLFW",
		"ImGui",
		"assimp",
		-- "tinyfiledialogs", NOT A PROJECT ANYMORE

		"AkSoundEngine",
		"AkMemoryMgr",
		"AkStreamMgr",
		"AkSpatialAudio",
		"CommunicationCentral", -- Not needed for release config
		"AkVorbisDecoder",
		"AkOpusDecoder"
	}

	-- buildoptions { "/translateInclude" } 
	
	-- filter "files:**.ixx or **.cppm"
	--	   flags { "NoPCH" }

	-- filter { "files:vendor/**.c" }
	--		   flags { "NoPCH" }

	-- filter { "files:vendor/**.cpp" }
	--	   flags { "NoPCH" } 

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
			"ws2_32" -- For Wwise Communication WARNING NOT NEEDED IN RELEASE BUT STILL INCLUDED FOR THE MOMENT
		}

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
		runtime "Debug"
		symbols "on"

	 filter "configurations:Editor Release"
		defines 
		{
			"EE_RELEASE",
			"EE_EDITOR_BUILD"
		}
		runtime "Release"
		optimize "on"

	filter "configurations:Debug"
		defines "EE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EE_DIST"
		runtime "Release"
		optimize "on"

print("Finished Generating Engine Solution.\n")