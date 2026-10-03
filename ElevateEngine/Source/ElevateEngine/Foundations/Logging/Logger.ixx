module;

#include <format> // iwyu: keep
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <string_view>

#include <spdlog/fmt/bundled/base.h>
#include <spdlog/logger.h>

export module Elevate.Foundations.Logger;

export namespace Elevate
{
	enum class LogLevel
	{
		Trace,
		Info,
		Warning,
		Error,
		Fatal
	};

	using LogCallback = std::function<void(LogLevel, std::string_view)>;

	// todo move in a private module
	namespace Internal
	{
		class LogImpl
		{
		public:
			LogImpl(std::shared_ptr<spdlog::logger> logger) : m_logger(logger) {}
			void Trace(const std::string& text);
			void Info(const std::string& text);
			void Warn(const std::string& text);
			void Error(const std::string& text);
			void Fatal(const std::string& text);

			void AddCallback(LogCallback callback);

		private:
			void NotifyCallbacks(LogLevel level, const std::string& msg);

		private:
			std::vector<LogCallback> m_callbacks;
			std::shared_ptr<spdlog::logger> m_logger;
		};

		class LogDispatcher
		{
		public:
			static inline void Trace(Internal::LogImpl* logger, const std::string& text) { if (logger) logger->Trace(text); }
			static inline void Info(Internal::LogImpl* logger, const std::string& text) { if (logger) logger->Info(text); }
			static inline void Warn(Internal::LogImpl* logger, const std::string& text) { if (logger) logger->Warn(text); }
			static inline void Error(Internal::LogImpl* logger, const std::string& text) { if (logger) logger->Error(text); }
			static inline void Fatal(Internal::LogImpl* logger, const std::string& text) { if (logger) logger->Fatal(text); }
		};
	}
	
	class Logger
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