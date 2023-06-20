#pragma once

#ifdef EE_DEBUG
	#define RH_ASSERTS_ENABLED 1
#else 
	#define RH_ASSERTS_ENABLED 0
#endif

#if RH_ASSERTS_ENABLED
	#define RH_ASSERT(x, ...) { if(!(x)) { RH_ERROR("Assertions Failed: {0}", __VA_ARGS__); __debugbreak(); } } 
	#define EE_CORE_ASSERT(x, ...) { if(!(x)) { EE_CORE_ERROR("Assertions Failed: {0}", __VA_ARGS__); __debugbreak(); } } 
#else
	#define RH_ASSERT(x, ...)
	#define EE_CORE_ASSERT(x, ...)
#endif