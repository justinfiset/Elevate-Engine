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
	IncludeDir["Vendors"] = "Vendor"
	IncludeDir["GLFW"] = "Vendor/GLFW/include" 
	IncludeDir["Glad"] = "Vendor/Glad/include"
	IncludeDir["ImGui"] = "Vendor/ImGui/"
	IncludeDir["glm"] = "Vendor/glm/"
	IncludeDir["rapidjson"] = "Vendor/rapidjson/include"
	IncludeDir["rapidxml"] = "Vendor/rapidxml/include"
	IncludeDir["stb"] = "Vendor/stb/"
	IncludeDir["spdlog"] = "Vendor/spdlog/include"
	IncludeDir["assimp"] = "Vendor/assimp/include"
	IncludeDir["tinyfiledialogs"] = "Vendor/tinyfiledialogs"
	IncludeDir["ImGuizmo"] = "Vendor/ImGuizmo"
	IncludeDir["entt"] = "Vendor/entt/include"
	IncludeDir["stduuid"] = "Vendor/stduuid/include"
	IncludeDir["Jolt"] = "Vendor/JoltPhysics"

	files
	{
		"Source/**.h",
		"Source/**.inl",
		"Source/**.cpp",

		"Source/**.vert",
		"Source/**.frag",

		"Vendor/ImGuizmo/ImGuizmo.cpp",
		"Vendor/ImGuiFileDialog/**.cpp",
		"Vendor/tinyfiledialogs/tinyfiledialogs.c",
	}

	defines 
	{
		"EE_RESOURCE_DIR=\""..path.getabsolute("./Resources/").."\"",
		"EE_ENGINE_BUILD",
		"_CRT_SECURE_NO_WARNINGS",
		"IMGUI_DEFINE_MATH_OPERATORS",
		"GLM_ENABLE_EXPERIMENTAL",
	}

	includedirs { "Source", values(IncludeDir) }

	links { "ImGui", "assimp", "Jolt" }

	Wwise.SetupEngine()
	filter "not system:emscripten"
		links { "GLFW" }
        files { "Vendor/Glad/src/glad.c" }
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
