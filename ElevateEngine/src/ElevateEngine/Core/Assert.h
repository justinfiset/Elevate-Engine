#pragma once

#include "Log.h"

#ifdef EE_DEBUG
	#define EE_ASSERTS_ENABLED 1
#else 
	#define EE_ASSERTS_ENABLED 0
	#define NDEBUG
#endif

#ifdef _MSC_VER
	#include <intrin.h>
	#define DEBUG_BREAK() __debugbreak()
#else
	#include <csignal>
	#define DEBUG_BREAK() raise(SIGTRAP)
#endif

#if EE_ASSERTS_ENABLED
	#define EE_ASSERT(x, ...) { if(!(x)) { EE_ERROR("Assertions Failed: {0}", __VA_ARGS__); DEBUG_BREAK(); } } 

	#if defined(EE_ENGINE_BUILD)
		#define EE_CORE_ASSERT(x, ...) { if(!(x)) { EE_CORE_ERROR("Assertions Failed: {0}", __VA_ARGS__); DEBUG_BREAK(); } } 
	#endif
#else
	#define EE_ASSERT(x, ...) { if(!(x)) { EE_ERROR("Assertions Failed: {0}", __VA_ARGS__); } } 
	#if defined(EE_ENGINE_BUILD)
		#define EE_CORE_ASSERT(x, ...) { if(!(x)) { EE_CORE_ERROR("Assertions Failed: {0}", __VA_ARGS__); } } 
	#endif
#endif