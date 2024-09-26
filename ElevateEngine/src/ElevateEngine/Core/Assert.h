#pragma once

#include "Log.h"

#ifdef EE_DEBUG
	#define EE_ASSERTS_ENABLED 1
#else 
	#define EE_ASSERTS_ENABLED 0
	#define NDEBUG
#endif

#if EE_ASSERTS_ENABLED
	#define EE_ASSERT(x, ...) { if(!(x)) { EE_ERROR("Assertions Failed: {0}", __VA_ARGS__); __debugbreak(); } } 
	#define EE_CORE_ASSERT(x, ...) { if(!(x)) { EE_CORE_ERROR("Assertions Failed: {0}", __VA_ARGS__); __debugbreak(); } } 
#else
	#define EE_ASSERT(x, ...) { if(!(x)) { EE_ERROR("Assertions Failed: {0}", __VA_ARGS__); } } 
	#define EE_CORE_ASSERT(x, ...) { if(!(x)) { EE_CORE_ERROR("Assertions Failed: {0}", __VA_ARGS__); } } 
#endif