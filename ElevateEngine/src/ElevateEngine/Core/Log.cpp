#include "eepch.h"

#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Elevate {
	std::shared_ptr<spdlog::logger> Log::s_coreLogger;
	std::shared_ptr<spdlog::logger> Log::s_clientLogger;

	bool Log::s_loggerInitialized;

	void Log::Init() {
		spdlog::set_pattern("%^[%T] [%n] %v%$");

		bool created = false;
		if (!s_coreLogger)
		{
			s_coreLogger = spdlog::stdout_color_mt("ELEVATE");
			s_coreLogger->set_level(spdlog::level::trace);
			created = true;
		}
		
		if (!s_clientLogger)
		{
			s_clientLogger = spdlog::stdout_color_mt("APP");
			s_clientLogger->set_level(spdlog::level::trace);
			created = true;
		}

		if (created)
		{
			EE_CORE_TRACE("Logger Initialized.");
		}

		s_loggerInitialized;
	}
}
