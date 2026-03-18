#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING

#include "Log.h"

#include <memory>

#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Elevate
{
	class LogImpl
	{
	public:
		LogImpl(std::shared_ptr<spdlog::logger> logger) : m_logger(logger) { }

		void Trace(std::string text) { m_logger->trace(text);    }
		void Info(std::string text)  { m_logger->info(text);     }
		void Warn(std::string text)  { m_logger->warn(text);     }
		void Error(std::string text) { m_logger->error(text);    }
		void Fatal(std::string text) { m_logger->critical(text); }

	private:
		std::shared_ptr<spdlog::logger> m_logger;
	};

	void Log::Trace(LogImpl* logger, std::string fmt) { logger->Trace(fmt); }
	void Log::Info(LogImpl* logger, std::string fmt)  { logger->Info(fmt);  }
	void Log::Warn(LogImpl* logger, std::string fmt)  { logger->Warn(fmt);  }
	void Log::Error(LogImpl* logger, std::string fmt) { logger->Error(fmt); }
	void Log::Fatal(LogImpl* logger, std::string fmt) { logger->Fatal(fmt); }

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