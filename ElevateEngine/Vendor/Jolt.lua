project "Jolt"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    staticruntime "on"

    targetdir ("%{wks.location}/bin/%{cfg.buildcfg}/%{prj.name}")
    objdir ("%{wks.location}/bin-int/%{cfg.buildcfg}/%{prj.name}")

    -- =========================================================================
    -- Source files
    -- =========================================================================

    files
    {
        "JoltPhysics/Jolt/**.cpp",
        "JoltPhysics/Jolt/**.h",
        "JoltPhysics/Jolt/**.inl"
    }

    -- =========================================================================
    -- Exclude optional compute backends
    -- =========================================================================

    removefiles
    {
        "JoltPhysics/Jolt/Compute/CPU/**",
        "JoltPhysics/Jolt/Compute/DX12/**",
        "JoltPhysics/Jolt/Compute/VK/**",
        "JoltPhysics/Jolt/Compute/MTL/**"
    }

    -- =========================================================================
    -- ObjectStream
    -- =========================================================================

    removefiles
    {
        "JoltPhysics/Jolt/ObjectStream/**.cpp"
    }

    files
    {
        "JoltPhysics/Jolt/ObjectStream/SerializableObject.cpp"
    }

    -- =========================================================================
    -- Include directories
    -- =========================================================================

    includedirs
    {
        "JoltPhysics"
    }

    -- =========================================================================
    -- Debug
    -- =========================================================================

    filter "configurations:Debug or Editor_Debug"
        runtime "Debug"
        symbols "On"

        defines
        {
            "JPH_ENABLE_ASSERTS"
        }
        
    -- =========================================================================
    -- Release
    -- =========================================================================

    filter "configurations:Release or Editor_Release"
        runtime "Release"
        optimize "On"

        defines
        {
            "NDEBUG"
        }

    -- =========================================================================
    -- Distribution
    -- =========================================================================

    filter "configurations:Dist"
        runtime "Release"
        optimize "Full"

        defines
        {
            "NDEBUG"
        }

    -- =========================================================================
    -- Reset filters
    -- =========================================================================

    filter {}