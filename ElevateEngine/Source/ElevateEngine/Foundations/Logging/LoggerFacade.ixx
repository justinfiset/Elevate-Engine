module;

// Use bundled fmt (same syntax as std::format) so module interfaces do not
// depend on standard-library header-unit support.
#include <spdlog/fmt/fmt.h>
#include <utility>

export module Elevate.Foundations.Logger.Facade;

export import Elevate.Foundations.Callbacks;
export import Elevate.Foundations.Logger.LogLevel;
export import Elevate.Foundations.Logger.LogCallback;
import Elevate.Foundations.Logger.Internal;

export namespace Elevate
{
	namespace Internal
	{
		template<typename TLogger>
		class LoggerFacade
		{
		public:
			static CallbackID AddCallback(LogCallback callback)
			{
				return TLogger::GetImpl()->AddCallback(callback);
			}

			static void RemoveCallback(CallbackID callbackId)
			{
				TLogger::GetImpl()->RemoveCallback(callbackId);
			}

			template<typename... Args>
			static void Trace(fmt::format_string<Args...> format, Args&&... args)
			{
				if (auto* impl = TLogger::GetImpl())
				{
					impl->Trace(fmt::format(format, std::forward<Args>(args)...));
				}
			}

			template<typename... Args>
			static void Info(fmt::format_string<Args...> format, Args&&... args)
			{
				if (auto* impl = TLogger::GetImpl())
				{
					impl->Info(fmt::format(format, std::forward<Args>(args)...));
				}
			}

			template<typename... Args>
			static void Warn(fmt::format_string<Args...> format, Args&&... args)
			{
				if (auto* impl = TLogger::GetImpl())
				{
					impl->Warn(fmt::format(format, std::forward<Args>(args)...));
				}
			}

			template<typename... Args>
			static void Error(fmt::format_string<Args...> format, Args&&... args)
			{
				if (auto* impl = TLogger::GetImpl())
				{
					impl->Error(fmt::format(format, std::forward<Args>(args)...));
				}
			}

			template<typename... Args>
			static void Fatal(fmt::format_string<Args...> format, Args&&... args)
			{
				if (auto* impl = TLogger::GetImpl())
				{
					impl->Fatal(fmt::format(format, std::forward<Args>(args)...));
				}
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
}
