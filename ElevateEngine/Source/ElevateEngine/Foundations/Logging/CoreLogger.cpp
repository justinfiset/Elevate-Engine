module;

#include <spdlog/logger.h>
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h> // iwyu: keep

module Elevate.Foundations.CoreLogger;

namespace Elevate
{
	Internal::LogImpl* CoreLogger::GetImpl()
	{
		static Internal::LogImpl* logger = new Internal::LogImpl([]() {
			auto spdlogger = spdlog::stdout_color_mt("ELEVATE");
			spdlogger->set_level(spdlog::level::trace);
			spdlogger->set_pattern("%^[%T] [%n] %v%$");
			return spdlogger;
			}());
		return logger;
	}
}
