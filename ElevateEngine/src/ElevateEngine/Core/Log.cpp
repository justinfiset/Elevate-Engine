#include "eepch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
//static std::shared_ptr<spdlog::logger> GetCoreLogger() {
//    static auto logger = []() {
//        auto logger = spdlog::stdout_color_mt("ELEVATE");
//        logger->set_level(spdlog::level::trace);
//        logger->set_pattern("%^[%T] [%n] %v%$");
//        return logger;
//        }();
//    return logger;
//}
//
//static std::shared_ptr<spdlog::logger> GetClientLogger() {
//    static auto logger = []() {
//        auto logger = spdlog::stdout_color_mt("APP");
//        logger->set_level(spdlog::level::trace);
//        logger->set_pattern("%^[%T] [%n] %v%$");
//        return logger;
//        }();
//    return logger;
//}
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