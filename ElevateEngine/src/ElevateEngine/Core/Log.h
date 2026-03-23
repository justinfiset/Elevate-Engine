#pragma once

#include <format>
#include <string>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Core.h"

namespace Elevate {
    class LogImpl {
    public:
        LogImpl(std::shared_ptr<spdlog::logger> logger) : m_logger(logger) { }
        // Utilise const std::string& ici
        inline void Trace(const std::string& text) { m_logger->trace(text);    }
        inline void Info(const std::string& text)  { m_logger->info(text);     }
        inline void Warn(const std::string& text)  { m_logger->warn(text);     }
        inline void Error(const std::string& text) { m_logger->error(text);    }
        inline void Fatal(const std::string& text) { m_logger->critical(text); }
    private:
        std::shared_ptr<spdlog::logger> m_logger;
    };

    class Log {
    public:
        // Utilise const std::string& ici aussi
        static inline void Trace(LogImpl* logger, const std::string& text) { if(logger) logger->Trace(text); }
        static inline void Info(LogImpl* logger, const std::string& text)  { if(logger) logger->Info(text);  }
        static inline void Warn(LogImpl* logger, const std::string& text)  { if(logger) logger->Warn(text);  }
        static inline void Error(LogImpl* logger, const std::string& text) { if(logger) logger->Error(text); }
        static inline void Fatal(LogImpl* logger, const std::string& text) { if(logger) logger->Fatal(text); }

        static LogImpl* GetCoreLogger();
        static LogImpl* GetClientLogger();
    };
}

// Macros Core : Le std::format est appelé ici, côté appelant.
#if defined(EE_ENGINE_BUILD) || defined(EE_ENGINE_INTERNAL)
#define EE_CORE_TRACE(...)     ::Elevate::Log::Trace(::Elevate::Log::GetCoreLogger(), std::format(__VA_ARGS__))
#define EE_CORE_INFO(...)      ::Elevate::Log::Info(::Elevate::Log::GetCoreLogger(),  std::format(__VA_ARGS__))
#define EE_CORE_WARN(...)      ::Elevate::Log::Warn(::Elevate::Log::GetCoreLogger(),  std::format(__VA_ARGS__))
#define EE_CORE_ERROR(...)     ::Elevate::Log::Error(::Elevate::Log::GetCoreLogger(), std::format(__VA_ARGS__))
#define EE_CORE_FATAL(...)     ::Elevate::Log::Fatal(::Elevate::Log::GetCoreLogger(), std::format(__VA_ARGS__))

#define EE_CORE_CTRACE(condition, ...)  if(condition) { EE_CORE_TRACE(__VA_ARGS__); }
#define EE_CORE_CINFO(condition, ...)   if(condition) { EE_CORE_INFO(__VA_ARGS__);  }
#define EE_CORE_CWARN(condition, ...)   if(condition) { EE_CORE_WARN(__VA_ARGS__);  }
#define EE_CORE_CERROR(condition, ...)  if(condition) { EE_CORE_ERROR(__VA_ARGS__); }
#define EE_CORE_CFATAL(condition, ...)  if(condition) { EE_CORE_FATAL(__VA_ARGS__); }
#endif

// Macros Client
#define EE_TRACE(...)          ::Elevate::Log::Trace(::Elevate::Log::GetClientLogger(), std::format(__VA_ARGS__))
#define EE_INFO(...)           ::Elevate::Log::Info(::Elevate::Log::GetClientLogger(),  std::format(__VA_ARGS__))
#define EE_WARN(...)           ::Elevate::Log::Warn(::Elevate::Log::GetClientLogger(),  std::format(__VA_ARGS__))
#define EE_ERROR(...)          ::Elevate::Log::Error(::Elevate::Log::GetClientLogger(), std::format(__VA_ARGS__))
#define EE_FATAL(...)          ::Elevate::Log::Fatal(::Elevate::Log::GetClientLogger(), std::format(__VA_ARGS__))

#define EE_CTRACE(condition, ...)  if(condition) { EE_TRACE(__VA_ARGS__); }
#define EE_CINFO(condition, ...)   if(condition) { EE_INFO(__VA_ARGS__);  }
#define EE_CWARN(condition, ...)   if(condition) { EE_WARN(__VA_ARGS__);  }
#define EE_CERROR(condition, ...)  if(condition) { EE_ERROR(__VA_ARGS__); }
#define EE_CFATAL(condition, ...)  if(condition) { EE_FATAL(__VA_ARGS__); }