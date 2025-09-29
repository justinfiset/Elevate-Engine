project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin/temps")

    files 
    {
        "src/**.h",
        "src/**.cpp",
        "src/**.vert",
        "src/**.frag",
    }

    includedirs
    {
        "../ElevateEngine/vendor/spdlog/include",
        "../ElevateEngine/vendor/entt/include",
        "../ElevateEngine/vendor/glm/",
        "../ElevateEngine/src"
    }

    links 
    {
        "ElevateEngine"
    }

    filter "system:windows"
        systemversion "latest"
        defines { "EE_PLATFORM_WINDOWS" }

    filter "system:linux"
        systemversion "latest"
        defines { "EE_PLATFORM_LINUX" }

        links
        {
            "GL", "GLU",
            "X11", "Xrandr", "Xinerama", "Xcursor", "Xi",
            "dl", "pthread"
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
        defines "EE_DIST"
        runtime "Release"
        optimize "on"
