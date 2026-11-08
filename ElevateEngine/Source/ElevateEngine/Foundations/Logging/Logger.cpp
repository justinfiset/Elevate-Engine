module;

#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING

#include <spdlog/logger.h>
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h> // iwyu: keep

module Elevate.Foundations.Logger;


namespace Elevate
{
	Internal::LogImpl* Logger::GetImpl()
	{
		static Internal::LogImpl* logger = new Internal::LogImpl([]()
			{
				auto spdlogger = spdlog::stdout_color_mt("APP");
				spdlogger->set_level(spdlog::level::trace);
				spdlogger->set_pattern("%^[%T] [%n] %v%$");
				return spdlogger;
			}());
		return logger;
	}
}