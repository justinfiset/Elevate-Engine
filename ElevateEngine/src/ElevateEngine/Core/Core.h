#pragma once

#ifdef EE_PLATFORM_WINDOWS
	#ifdef EE_DYNAMIC_LINK
		#ifdef EE_BUILD_DLL
			#define EE_API __declspec(dllexport)
		#else 
			#define EE_API __declspec(dllimport)
		#endif // EE_BUILD_DLL
	#else
		#define EE_API
	#endif
#else
	#error ElevateEngine is currently only supported on Windows.
#endif // EE_PLATFORM_WINDOWS

#define BIT(x) (1 << x)

#define RH_BIND_EVNT_FN(fn) std::bind(&fn, this, std::placeholders::_1)