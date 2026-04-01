project "ImGui"
	kind "StaticLib"
	language "C"
	architecture "x86_64"

	targetdir ("imgui/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("imgui/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs { "ImGui/", "Glad/include", "GLFW/include/" }

	files
	{
		"ImGui/*.cpp",
		"ImGui/backends/imgui_impl_glfw.cpp",
		"ImGui/backends/imgui_impl_opengl3.cpp",
		"ImGui/misc/cpp/imgui_stdlib.cpp"
	}

	defines 
	{
		"IMGUI_IMPL_OPENGL_LOADER_GLAD"
	}
    
	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"

		defines
		{
			"_IMGUI_X11"
		}

	filter "system:emscripten"
		architecture "wasm32"
		systemversion "latest"

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		defines 
		{ 
			"_IMGUI_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug or configurations:Editor_Debug"
		runtime "Debug"
		optimize "Off"
		symbols "on"

	filter "configurations:Release or configurations:Editor_Release or configurations:Dist"
		runtime "Release"
		optimize "Speed"
		optimize "on"