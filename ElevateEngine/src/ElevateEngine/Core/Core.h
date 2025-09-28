#pragma once

#if defined(EE_PLATFORM_WINDOWS) || defined(EE_PLATFORM_LINUX)
	#ifdef EE_DIST // Hide the console if in dist
		#ifdef _WIN32
			#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
		#endif
	#endif

	#ifdef EE_DYNAMIC_LINK
		#ifdef EE_BUILD_DLL
			#define EE_API __declspec(dllexport)
		#else 
			#define EE_API __declspec(dllimport)
		#endif // EE_BUILD_DLL
	#else
		#define EE_API // does absolutly nothing if we are doing static linking and compile
	#endif
#else
	#error ElevateEngine is currently only supported on Windows.
#endif // EE_PLATFORM_WINDOWS or EE_PLATFORM_LINUX

#define BIT(x) (1 << x)

#define EE_BIND_EVNT_FN(fn) std::bind(&fn, this, std::placeholders::_1)