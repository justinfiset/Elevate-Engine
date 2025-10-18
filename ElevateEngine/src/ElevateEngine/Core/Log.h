#pragma once
#include "Core.h"
#include <string>
#include <format>



namespace Elevate {
    class LogImpl;

    class EE_API Log
    {
    public:
        static void Trace(LogImpl* logger, std::string fmt);
        static void Info(LogImpl* logger, std::string fmt);
        static void Warn(LogImpl* logger, std::string fmt);
        static void Error(LogImpl* logger, std::string fmt);
        static void Fatal(LogImpl* logger, std::string fmt);

        static LogImpl* GetCoreLogger();
        static LogImpl* GetClientLogger();

        // Fonctions templates pour le formatting
        template<typename... Args>
        static void TraceFormat(LogImpl* logger, std::format_string<Args...> fmt, Args&&... args) {
            Trace(logger, std::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void InfoFormat(LogImpl* logger, std::format_string<Args...> fmt, Args&&... args) {
            Info(logger, std::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void WarnFormat(LogImpl* logger, std::format_string<Args...> fmt, Args&&... args) {
            Warn(logger, std::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void ErrorFormat(LogImpl* logger, std::format_string<Args...> fmt, Args&&... args) {
            Error(logger, std::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void FatalFormat(LogImpl* logger, std::format_string<Args...> fmt, Args&&... args) {

            Fatal(logger, std::format(fmt, std::forward<Args>(args)...));
        }
    };
}

// Core log macros avec formatting
#if defined(EE_ENGINE_BUILD) || defined(EE_ENGINE_INTERNAL)
#define EE_CORE_TRACE(...)     ::Elevate::Log::TraceFormat(::Elevate::Log::GetCoreLogger(), __VA_ARGS__)
#define EE_CORE_INFO(...)      ::Elevate::Log::InfoFormat(::Elevate::Log::GetCoreLogger(), __VA_ARGS__)
#define EE_CORE_WARN(...)      ::Elevate::Log::WarnFormat(::Elevate::Log::GetCoreLogger(), __VA_ARGS__)
#define EE_CORE_ERROR(...)     ::Elevate::Log::ErrorFormat(::Elevate::Log::GetCoreLogger(), __VA_ARGS__)
#define EE_CORE_FATAL(...)     ::Elevate::Log::FatalFormat(::Elevate::Log::GetCoreLogger(), __VA_ARGS__)

// Conditional logging
#define EE_CORE_CTRACE(condition, ...)  if(condition) { EE_CORE_TRACE(__VA_ARGS__); }
#define EE_CORE_CINFO(condition, ...)   if(condition) { EE_CORE_INFO(__VA_ARGS__);  }
#define EE_CORE_CWARN(condition, ...)   if(condition) { EE_CORE_WARN(__VA_ARGS__);  }
#define EE_CORE_CERROR(condition, ...)  if(condition) { EE_CORE_ERROR(__VA_ARGS__); }
#define EE_CORE_CFATAL(condition, ...)  if(condition) { EE_CORE_FATAL(__VA_ARGS__); }
#endif

// Client log macros
#define EE_TRACE(...)          ::Elevate::Log::TraceFormat(::Elevate::Log::GetClientLogger(), __VA_ARGS__)
#define EE_INFO(...)           ::Elevate::Log::InfoFormat(::Elevate::Log::GetClientLogger(), __VA_ARGS__)
#define EE_WARN(...)           ::Elevate::Log::WarnFormat(::Elevate::Log::GetClientLogger(), __VA_ARGS__)
#define EE_ERROR(...)          ::Elevate::Log::ErrorFormat(::Elevate::Log::GetClientLogger(), __VA_ARGS__)
#define EE_FATAL(...)          ::Elevate::Log::FatalFormat(::Elevate::Log::GetClientLogger(), __VA_ARGS__)

// Conditional logging
#define EE_CTRACE(condition, ...)  if(condition) { EE_TRACE(__VA_ARGS__); }
#define EE_CINFO(condition, ...)   if(condition) { EE_INFO(__VA_ARGS__);  }
#define EE_CWARN(condition, ...)   if(condition) { EE_WARN(__VA_ARGS__);  }
#define EE_CERROR(condition, ...)  if(condition) { EE_ERROR(__VA_ARGS__); }
#define EE_CFATAL(condition, ...)  if(condition) { EE_FATAL(__VA_ARGS__); }