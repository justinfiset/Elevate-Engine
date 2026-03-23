#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING

#include "Log.h"
#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Elevate
{
	LogImpl* Log::GetCoreLogger()
	{
		static LogImpl* logger = new LogImpl([]() {
			auto spdlogger = spdlog::stdout_color_mt("ELEVATE");
			spdlogger->set_level(spdlog::level::trace);
			spdlogger->set_pattern("%^[%T] [%n] %v%$");
			return spdlogger;
		}());
		return logger;
	}

	LogImpl* Log::GetClientLogger()
	{
		static LogImpl* logger = new LogImpl([]() {
			auto spdlogger = spdlog::stdout_color_mt("APP");
			spdlogger->set_level(spdlog::level::trace);
			spdlogger->set_pattern("%^[%T] [%n] %v%$");
			return spdlogger;
		}());
		return logger;
	}
}