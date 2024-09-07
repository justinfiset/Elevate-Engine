project "tinyfiledialogs"
    kind "StaticLib"
    language "C++"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "tinyfiledialogs.cpp",
        "tinyfiledialogs.h"
    }

    filter "system:windows"
        systemversion "latest"

    filter "system:linux"
		pic "on"
		systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
