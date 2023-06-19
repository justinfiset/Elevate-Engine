#pragma once

#ifdef EE_PLATFORM_WINDOWS
	#ifdef RH_DYNAMIC_LINK
		#ifdef RH_BUILD_DLL
			#define EE_API __declspec(dllexport)
		#else 
			#define EE_API __declspec(dllimport)
		#endif // RH_BUILD_DLL
	#else
		#define EE_API
	#endif
#else
	#error ElevateEngine is currently only supported on Windows.
#endif // EE_PLATFORM_WINDOWS

#define BIT(x) (1 << x)

#define RH_BIND_EVNT_FN(fn) std::bind(&fn, this, std::placeholders::_1)