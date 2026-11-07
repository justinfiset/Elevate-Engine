#pragma once

// Soundengine
// todo fix this asap
#ifndef EE_NO_SOUNDENGINE
#define EE_NO_SOUNDENGINE 0 // todo change back once managed by the lua
#endif

#if defined(EE_PLATFORM_WINDOWS) || defined(EE_PLATFORM_LINUX) || defined(EE_PLATFORM_WEB)
#ifdef EE_DIST
#ifdef _WIN32
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif
#endif
#else
#error ElevateEngine is currently only supported on Windows, Linux and Web.
#endif

// todo remove
#define EE_BIND_EVNT_FN(fn) std::bind(&fn, this, std::placeholders::_1)