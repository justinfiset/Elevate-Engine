project "ElevateEngine"
	location "./Build"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("../Build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../Build/bin-int/" .. outputdir .. "/%{prj.name}")

	flags { "NoPCH" }

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
		"EE_USES_WWISE", -- We still include the Wwise synmbolds in the engine even if the user doesn't use Wwise, as it doesn't cause any issues and allows users to easily add Wwise support if they want to.
		"_CRT_SECURE_NO_WARNINGS",
		"IMGUI_DEFINE_MATH_OPERATORS",
	}

	includedirs
	{
		"src",

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

	Wwise.SetupEngine()

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