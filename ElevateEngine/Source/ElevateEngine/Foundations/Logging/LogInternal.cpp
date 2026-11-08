module;

#include <algorithm>

#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING

#include <spdlog/logger.h>
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h> // iwyu: keep

module Elevate.Foundations.Logger.Internal;

namespace Elevate::Internal
{
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

	LogImpl::LogImpl(std::shared_ptr<spdlog::logger> logger)
		: m_logger(logger), m_nextCallbackId(1)
	{
	}

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

	CallbackID LogImpl::AddCallback(LogCallback callback)
	{
		const auto callbackId = m_nextCallbackId++;
		m_callbacks.push_back({ callbackId, callback });
		return callbackId;
	}

	void LogImpl::RemoveCallback(CallbackID callbackId)
	{
		const auto it = std::find_if(m_callbacks.begin(), m_callbacks.end(), [callbackId](const LogCallbackEntry& entry)
			{
				return entry.Id == callbackId;
			});

		if (it != m_callbacks.end())
		{
			m_callbacks.erase(it);
		}
	}

	void LogImpl::NotifyCallbacks(LogLevel level, const std::string& msg)
	{
		for (auto& entry : m_callbacks)
		{
			entry.Callback(level, msg);
		}
	}
}
