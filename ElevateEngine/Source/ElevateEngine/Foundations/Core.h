#pragma once

#if defined(EE_PLATFORM_WINDOWS) || defined(EE_PLATFORM_LINUX) || defined(EE_PLATFORM_WEB)
#ifdef EE_DIST
#ifdef _WIN32
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif
#endif
#else
#error ElevateEngine is currently only supported on Windows, Linux and Web.
#endif