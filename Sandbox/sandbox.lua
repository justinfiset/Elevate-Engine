project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin/temps")

    IncludeDir = {}
    IncludeDir["GLFW"] = "../ElevateEngine/vendor/GLFW/include" 
    IncludeDir["Glad"] = "../ElevateEngine/vendor/Glad/include"
    IncludeDir["ImGui"] = "../ElevateEngine/vendor/ImGui/"
    IncludeDir["glm"] = "../ElevateEngine/vendor/glm/"
    IncludeDir["rapidjson"] = "../ElevateEngine/vendor/rapidjson/include"
    IncludeDir["stb"] = "../ElevateEngine/vendor/stb/"
    IncludeDir["spdlog"] = "../ElevateEngine/vendor/spdlog/include"
    IncludeDir["assimp"] = "../ElevateEngine/vendor/assimp/include"
    IncludeDir["tinyfiledialogs"] = "../ElevateEngine/vendor/tinyfiledialogs"
    IncludeDir["ImGuizmo"] = "../ElevateEngine/vendor/ImGuizmo"
    IncludeDir["entt"] = "../ElevateEngine/vendor/entt/include"

    debugdir("./")

    files 
    {
        "src/**.h",
        "src/**.cpp",

        "src/**.vert",
        "src/**.frag",
    }

    includedirs
    {
        "%{IncludeDir.GLFW}",
	    "%{IncludeDir.Glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.rapidjson}",
        "%{IncludeDir.assimp}",
        "%{IncludeDir.tinyfiledialogs}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.entt}",
        
        "vendor/include",
        "../ElevateEngine/vendor/",
        "../ElevateEngine/src"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "EE_PLATFORM_WINDOWS",
        }

	links 
	{
	    "ElevateEngine",
	}

    filter "system:linux"
        systemversion "latest"

        defines
        {
            "EE_PLATFORM_LINUX",
	    }

        files 
        {
        	"%{IncludeDir.ImGui}/backends/imgui_impl_glfw.cpp",
            "%{IncludeDir.ImGui}/backends/imgui_impl_opengl3.cpp",
          -- "ElevateEngine/vendor/Glad/src/glad.c",
        }

	links
	{
	    "GL",
	    "GLU",
	    "X11",
	    "Xrandr",
	    "Xinerama",
	    "Xcursor",
	    "Xi",
	    "dl",
	    "pthread",
	    "Glad",
        "GLFW",
        "ImGui",
	    "ElevateEngine",
	    "assimp",
	}
    filter "configurations:Debug"
        defines "EE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "EE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        kind "WindowedApp" -- Hide the console 
        defines "EE_DIST"
        runtime "Release"
        optimize "on"