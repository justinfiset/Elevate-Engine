#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING

module;

#include <spdlog/logger.h>
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h> // iwyu: keep

module Elevate.Foundations.Logger;

#ifdef EE_PLATFORM_WEB
constexpr std::string_view TRACE_PREFIX = "[TRACE] ";
constexpr std::string_view INFO_PREFIX = "[INFO] ";
constexpr std::string_view WARN_PREFIX = "[WARN] ";
constexpr std::string_view ERROR_PREFIX = "[ERROR] ";
constexpr std::string_view FATAL_PREFIX = "[FATAL] ";
#else
constexpr std::string_view TRACE_PREFIX = "";
constexpr std::string_view INFO_PREFIX = "";
constexpr std::string_view WARN_PREFIX = "";
constexpr std::string_view ERROR_PREFIX = "";
constexpr std::string_view FATAL_PREFIX = "";
#endif

namespace Elevate
{
	namespace Internal
	{
		void LogImpl::Trace(const std::string& text)
		{
			m_logger->trace("{}{}", TRACE_PREFIX, text);
			NotifyCallbacks(LogLevel::Trace, text);
		}

		void LogImpl::Info(const std::string& text)
		{
			m_logger->info("{}{}", INFO_PREFIX, text);
			NotifyCallbacks(LogLevel::Info, text);
		}

		void LogImpl::Warn(const std::string& text)
		{
			m_logger->warn("{}{}", WARN_PREFIX, text);
			NotifyCallbacks(LogLevel::Warning, text);
		}

		void LogImpl::Error(const std::string& text)
		{
			m_logger->error("{}{}", ERROR_PREFIX, text);
			NotifyCallbacks(LogLevel::Error, text);
		}

		void LogImpl::Fatal(const std::string& text)
		{
			m_logger->critical("{}{}", FATAL_PREFIX, text);
			NotifyCallbacks(LogLevel::Fatal, text);
		}

		void LogImpl::AddCallback(LogCallback callback)
		{
			m_callbacks.push_back(callback);
		}

		void LogImpl::NotifyCallbacks(LogLevel level, const std::string& msg)
		{
			for (auto& callback : m_callbacks)
			{
				callback(level, msg);
			}
		}
	}
	
	Internal::LogImpl* Logger::GetImpl()
	{
		static Internal::LogImpl* logger = new Internal::LogImpl([]() {
			auto spdlogger = spdlog::stdout_color_mt("APP");
			spdlogger->set_level(spdlog::level::trace);
			spdlogger->set_pattern("%^[%T] [%n] %v%$");
			return spdlogger;
		}());
		return logger;
	}
}