    #pragma once

// Soundengine
// todo fix this asap
#ifndef EE_NO_SOUNDENGINE
	#define EE_NO_SOUNDENGINE 0 // todo change back once managed by the lua
#endif

#define EE_API

#if defined(EE_PLATFORM_WINDOWS) || defined(EE_PLATFORM_LINUX) || defined(EE_PLATFORM_WEB)
    #ifdef EE_DIST
        #ifdef _WIN32
            #pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
        #endif
    #endif
#else
    #error ElevateEngine is currently only supported on Windows, Linux and Web.
#endif

#ifdef EE_PLATFORM_WEB
    #define EE_SHADER_VERSION_STRING "#version 300"
    #define EE_SHADER_PROFILE_STRING "es"
    #define EE_SHADER_PRECISION_STRING "precision highp float;\n"
#else
    #define EE_SHADER_VERSION_STRING "#version 410"
    #define EE_SHADER_PROFILE_STRING "core"
    #define EE_SHADER_PRECISION_STRING "" 
#endif

#define EE_SHADER_VERSION_HEADER EE_SHADER_VERSION_STRING " " EE_SHADER_PROFILE_STRING
#define EE_SHADER_HEADER EE_SHADER_VERSION_HEADER "\n" EE_SHADER_PRECISION_STRING

#ifndef EE_PLATFORM_WEB
    #define EE_ENGINE_DIR EE_RESOURCE_DIR "/Engine/"
    #define EE_EDITOR_DIR EE_RESOURCE_DIR "/Editor/"
#else
    #define EE_ENGINE_DIR "/Engine/"
    #define EE_EDITOR_DIR "/Editor/"
#endif
#define EE_CONTENT_ROOT "./Content/"

#define BIT(x) (1 << x)

#define EE_BIND_EVNT_FN(fn) std::bind(&fn, this, std::placeholders::_1)