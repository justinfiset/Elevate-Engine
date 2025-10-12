#include "eepch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#include <cstdarg>
#include <cstdio>
#include <vector>

#define EE_LOGGER_FORMAT(adr) \
    va_list args; \
    va_start(args, fmt); \
    adr = Internal::FormatString(fmt, args); \
    va_end(args) \

namespace Elevate {
    namespace Internal {
        static std::shared_ptr<spdlog::logger> GetCoreLogger() {
            static auto logger = []() {
                auto logger = spdlog::stdout_color_mt("ELEVATE");
                logger->set_level(spdlog::level::trace);
                logger->set_pattern("%^[%T] [%n] %v%$");
                return logger;
            }();
            return logger;
        }

        static std::shared_ptr<spdlog::logger> GetClientLogger() {
            static auto logger = []() {
                auto logger = spdlog::stdout_color_mt("APP");
                logger->set_level(spdlog::level::trace);
                logger->set_pattern("%^[%T] [%n] %v%$");
                return logger;
            }();
            return logger;
        }

        static std::string FormatString(const char* fmt, va_list args) {
            if (!fmt) {
                return "Error: null format string";
            }

            va_list args_copy;
            va_copy(args_copy, args);

            int size = vsnprintf(nullptr, 0, fmt, args_copy);
            va_end(args_copy);

            if (size < 0) {
                return "Format error";
            }

            std::vector<char> buffer(size + 1);
            vsnprintf(buffer.data(), buffer.size(), fmt, args);

            return std::string(buffer.data(), size);
        }
    }

    void Log::Trace(const char* fmt, ...) {
        std::string formatted;
        EE_LOGGER_FORMAT(formatted);
        Internal::GetCoreLogger()->trace("{}", formatted);
    }

    void Log::Info(const char* fmt, ...) {
        std::string formatted;
        EE_LOGGER_FORMAT(formatted);
        Internal::GetCoreLogger()->info("{}", formatted);
    }

    void Log::Warn(const char* fmt, ...) {
        std::string formatted;
        EE_LOGGER_FORMAT(formatted);
        Internal::GetCoreLogger()->warn("{}", formatted);
    }

    void Log::Error(const char* fmt, ...) {
        std::string formatted;
        EE_LOGGER_FORMAT(formatted);
        Internal::GetCoreLogger()->error("{}", formatted);
    }

    void Log::Fatal(const char* fmt, ...) {
        std::string formatted;
        EE_LOGGER_FORMAT(formatted);
        Internal::GetCoreLogger()->critical("{}", formatted);
    }

    // Client versions
    void Log::ClientTrace(const char* fmt, ...) {
        std::string formatted;
        EE_LOGGER_FORMAT(formatted);
        Internal::GetClientLogger()->trace("{}", formatted);
    }

    void Log::ClientInfo(const char* fmt, ...) {
        std::string formatted;
        EE_LOGGER_FORMAT(formatted);
        Internal::GetClientLogger()->info("{}", formatted);
    }

    void Log::ClientWarn(const char* fmt, ...) {
        std::string formatted;
        EE_LOGGER_FORMAT(formatted);
        Internal::GetClientLogger()->warn("{}", formatted);
    }

    void Log::ClientError(const char* fmt, ...) {
        std::string formatted;
        EE_LOGGER_FORMAT(formatted);
        Internal::GetClientLogger()->error("{}", formatted);
    }

    void Log::ClientFatal(const char* fmt, ...) {
        std::string formatted;
        EE_LOGGER_FORMAT(formatted);
        Internal::GetClientLogger()->critical("{}", formatted);
    }
}