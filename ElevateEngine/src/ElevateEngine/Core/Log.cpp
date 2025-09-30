#include "eepch.h"

#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Elevate {
    class SpdLogger : public ILogger {
    private:
        std::shared_ptr<spdlog::logger> m_logger;

    public:
        SpdLogger(const std::string& name, const std::string& pattern = "%^[%T] [%n] %v%$") {
            m_logger = spdlog::stdout_color_mt(name);
            m_logger->set_level(spdlog::level::trace);
            m_logger->set_pattern(pattern);
        }

        ~SpdLogger() {
            spdlog::drop(m_logger->name());
        }

    protected:
        // Implémentation avec templates pour le formatage spdlog
        template<typename... Args>
        void log_template(spdlog::level::level_enum level, const char* fmt, Args&&... args) {
            m_logger->log(level, fmt, std::forward<Args>(args)...);
        }

        void trace_impl(const char* fmt, ...) override {
            va_list args;
            va_start(args, fmt);
            // Fallback pour la compatibilité variadique
            char buffer[4096];
            vsnprintf(buffer, sizeof(buffer), fmt, args);
            m_logger->trace(buffer);
            va_end(args);
        }

        void info_impl(const char* fmt, ...) override {
            va_list args;
            va_start(args, fmt);
            char buffer[4096];
            vsnprintf(buffer, sizeof(buffer), fmt, args);
            m_logger->info(buffer);
            va_end(args);
        }

        void warn_impl(const char* fmt, ...) override {
            va_list args;
            va_start(args, fmt);
            char buffer[4096];
            vsnprintf(buffer, sizeof(buffer), fmt, args);
            m_logger->warn(buffer);
            va_end(args);
        }

        void error_impl(const char* fmt, ...) override {
            va_list args;
            va_start(args, fmt);
            char buffer[4096];
            vsnprintf(buffer, sizeof(buffer), fmt, args);
            m_logger->error(buffer);
            va_end(args);
        }

        void fatal_impl(const char* fmt, ...) override {
            va_list args;
            va_start(args, fmt);
            char buffer[4096];
            vsnprintf(buffer, sizeof(buffer), fmt, args);
            m_logger->critical(buffer);
            va_end(args);
        }
    };

    ILogger* Log::s_coreLogger = nullptr;
    ILogger* Log::s_clientLogger = nullptr;
	bool Log::s_loggerInitialized = false;

	void Log::Init() {
		bool created = false;
		if (!s_coreLogger)
		{
            s_coreLogger = new SpdLogger("ELEVATE");
			created = true;
		}
		
		if (!s_clientLogger)
		{
            s_clientLogger = new SpdLogger("APP");
			created = true;
		}

		if (created)
		{
			EE_CORE_TRACE("Logger Initialized.");
		}

		s_loggerInitialized;
	}

    ILogger* Log::GetCoreLogger()
	{
		if (!s_coreLogger)
		{
			Init();
		}
		return s_coreLogger;
	}

    ILogger* Log::GetClientLogger()
	{
		if (!s_clientLogger)
		{
			Init();
		}
		return s_clientLogger;
	}
}
