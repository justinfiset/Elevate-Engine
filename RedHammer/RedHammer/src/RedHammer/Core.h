#pragma once

#ifdef RH_PLATFORM_WINDOWS
	#ifdef RH_BUILD_DLL
		#define RH_API __declspec(dllexport)
	#else 
		#define RH_API __declspec(dllimport)
	#endif // RH_BUILD_DLL
#else
	#error RedHammer is currently only supported on Windows.
#endif // RH_PLATFORM_WINDOWS

#define BIT(x) (1 << x)

#define RH_BIND_EVNT_FN(fn) std::bind(&fn, this, std::placeholders::_1)