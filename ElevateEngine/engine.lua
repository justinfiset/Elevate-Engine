local function values(t)
    local res = {}
    for _, v in pairs(t) do table.insert(res, v) end
    return res
end

project "ElevateEngine"
	location "./Build"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	systemversion "latest"
	
	targetdir ("%{wks.location}/Build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Build/bin-int/" .. outputdir .. "/%{prj.name}")

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
		"vendor/ImGuiFileDialog/**.cpp",
		"vendor/tinyfiledialogs/tinyfiledialogs.cpp",
	}

	defines 
	{
		"EE_RESOURCE_DIR=\""..path.getabsolute("./Resources/").."\"",
		"EE_ENGINE_BUILD",
		"_CRT_SECURE_NO_WARNINGS",
		"IMGUI_DEFINE_MATH_OPERATORS",
		"GLM_ENABLE_EXPERIMENTAL",
	}

	includedirs { "src", values(IncludeDir) }

	links { "ImGui", "assimp" }

	Wwise.SetupEngine()
	filter "not system:emscripten"
		links { "GLFW" }
        files { "vendor/Glad/src/glad.c" }
    filter {}

	BuildPlatform.SetPlatformDefines()

	filter "system:windows"
		links { "opengl32.lib" }
		
	filter "system:linux"
		links { "GL", "X11", "pthread", "dl" }

	filter "configurations:Editor_Debug"
		defines { "EE_DEBUG", "EE_EDITOR_BUILD" }
		runtime "Debug"
		symbols "on"

	 filter "configurations:Editor_Release"
		defines { "EE_RELEASE", "EE_EDITOR_BUILD" }
		runtime "Release"
		optimize "on"

	filter "configurations:Debug"
		defines "EE_DEBUG"
		runtime "Debug"
		symbols "on"
		links { "ws2_32" }

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
