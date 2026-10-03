module;

#include <format>
#include <string_view>
#include <utility>

export module Elevate.Foundations.CoreLogger;

import Elevate.Foundations.Logger;

export namespace Elevate
{
	class CoreLogger
	{
	public:
		static Internal::LogImpl* GetImpl();

		static void AddCallback(LogCallback callback)
		{
			GetImpl()->AddCallback(callback);
		}

		template<typename... Args>
		static void Trace(std::format_string<Args...> fmt, Args&&... args)
		{
			Internal::LogDispatcher::Trace(GetImpl(), std::format(fmt, std::forward<Args>(args)...));
		}

		template<typename... Args>
		static void Info(std::format_string<Args...> fmt, Args&&... args)
		{
			Internal::LogDispatcher::Info(GetImpl(), std::format(fmt, std::forward<Args>(args)...));
		}

		template<typename... Args>
		static void Warn(std::format_string<Args...> fmt, Args&&... args)
		{
			Internal::LogDispatcher::Warn(GetImpl(), std::format(fmt, std::forward<Args>(args)...));
		}

		template<typename... Args>
		static void Error(std::format_string<Args...> fmt, Args&&... args)
		{
			Internal::LogDispatcher::Error(GetImpl(), std::format(fmt, std::forward<Args>(args)...));
		}

		template<typename... Args>
		static void Fatal(std::format_string<Args...> fmt, Args&&... args)
		{
			Internal::LogDispatcher::Fatal(GetImpl(), std::format(fmt, std::forward<Args>(args)...));
		}

		// Conditional Logging

		template<typename... Args>
		static void CTrace(bool condition, std::format_string<Args...> fmt, Args&&... args)
		{
			if (!condition)
			{
				Trace(fmt, std::forward<Args>(args)...);
			}
		}

		template<typename... Args>
		static void CInfo(bool condition, std::format_string<Args...> fmt, Args&&... args)
		{
			if (!condition)
			{
				Info(fmt, std::forward<Args>(args)...);
			}
		}

		template<typename... Args>
		static void CWarn(bool condition, std::format_string<Args...> fmt, Args&&... args)
		{
			if (!condition)
			{
				Warn(fmt, std::forward<Args>(args)...);
			}
		}

		template<typename... Args>
		static void CError(bool condition, std::format_string<Args...> fmt, Args&&... args)
		{
			if (!condition)
			{
				Error(fmt, std::forward<Args>(args)...);
			}
		}

		template<typename... Args>
		static void CFatal(bool condition, std::format_string<Args...> fmt, Args&&... args)
		{
			if (!condition)
			{
				Fatal(fmt, std::forward<Args>(args)...);
			}
		}
	};
}
