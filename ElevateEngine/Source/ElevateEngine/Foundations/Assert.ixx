module;

#include <format>

#include "Core.h"

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

export module Elevate.Foundations.Assert;

import Elevate.Foundations.CoreLogger;

export namespace Elevate
{
	class Assert
	{
	public:
		template<typename T, typename... Args>
		static void That(T&& condition, std::format_string<Args...> fmt, Args&&... args)
		{
			if (!static_cast<bool>(std::forward<T>(condition)))
			{
				CoreLogger::Error(fmt, std::forward<Args>(args)...);
#ifdef EE_ASSERTS_ENABLED
				DEBUG_BREAK();
#endif
			}
		}
	};
}