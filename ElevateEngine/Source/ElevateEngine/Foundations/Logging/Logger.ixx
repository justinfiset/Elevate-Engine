module;

// GCC 14 cannot instantiate std::format's friend-based argument store across
// module BMI boundaries; use the bundled fmt library (same syntax) instead.
#include <spdlog/fmt/fmt.h>
#include <functional>
#include <string_view>

export module Elevate.Foundations.Logger;

export import Elevate.Foundations.Callbacks;
export import Elevate.Foundations.Logger.LogLevel;
export import Elevate.Foundations.Logger.LogCallback;
import Elevate.Foundations.Logger.Internal;

export namespace Elevate
{
	class Logger
	{
	public:
		static Internal::LogImpl* GetImpl();

		static CallbackID AddCallback(LogCallback callback)
		{
			return GetImpl()->AddCallback(callback);
		}

		static void RemoveCallback(CallbackID callbackId)
		{
			GetImpl()->RemoveCallback(callbackId);
		}

		template<typename... Args>
		static void Trace(fmt::format_string<Args...> format, Args&&... args)
		{
			Internal::LogDispatcher::Trace(GetImpl(), fmt::format(format, std::forward<Args>(args)...));
		}

		template<typename... Args>
		static void Info(fmt::format_string<Args...> format, Args&&... args)
		{
			Internal::LogDispatcher::Info(GetImpl(), fmt::format(format, std::forward<Args>(args)...));
		}

		template<typename... Args>
		static void Warn(fmt::format_string<Args...> format, Args&&... args)
		{
			Internal::LogDispatcher::Warn(GetImpl(), fmt::format(format, std::forward<Args>(args)...));
		}

		template<typename... Args>
		static void Error(fmt::format_string<Args...> format, Args&&... args)
		{
			Internal::LogDispatcher::Error(GetImpl(), fmt::format(format, std::forward<Args>(args)...));
		}

		template<typename... Args>
		static void Fatal(fmt::format_string<Args...> format, Args&&... args)
		{
			Internal::LogDispatcher::Fatal(GetImpl(), fmt::format(format, std::forward<Args>(args)...));
		}

		// Conditional Logging

		template<typename... Args>
		static void CTrace(bool condition, fmt::format_string<Args...> format, Args&&... args)
		{
			if (!condition)
			{
				Trace(format, std::forward<Args>(args)...);
			}
		}

		template<typename... Args>
		static void CInfo(bool condition, fmt::format_string<Args...> format, Args&&... args)
		{
			if (!condition)
			{
				Info(format, std::forward<Args>(args)...);
			}
		}

		template<typename... Args>
		static void CWarn(bool condition, fmt::format_string<Args...> format, Args&&... args)
		{
			if (!condition)
			{
				Warn(format, std::forward<Args>(args)...);
			}
		}

		template<typename... Args>
		static void CError(bool condition, fmt::format_string<Args...> format, Args&&... args)
		{
			if (!condition)
			{
				Error(format, std::forward<Args>(args)...);
			}
		}

		template<typename... Args>
		static void CFatal(bool condition, fmt::format_string<Args...> format, Args&&... args)
		{
			if (!condition)
			{
				Fatal(format, std::forward<Args>(args)...);
			}
		}
	};
}