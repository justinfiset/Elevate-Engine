#pragma once
#include "Core.h"
#include <string>

namespace Elevate {
    class EE_API Log {
    public:
        // Core logging methods
        static void Trace(const char* fmt, ...);
        static void Info(const char* fmt, ...);
        static void Warn(const char* fmt, ...);
        static void Error(const char* fmt, ...);
        static void Fatal(const char* fmt, ...);

        // Client logging methods
        static void ClientTrace(const char* fmt, ...);
        static void ClientInfo(const char* fmt, ...);
        static void ClientWarn(const char* fmt, ...);
        static void ClientError(const char* fmt, ...);
        static void ClientFatal(const char* fmt, ...);
    };
}

// Core log macros
#if defined(EE_ENGINE_BUILD) || defined(EE_ENGINE_INTERNAL)
#define EE_CORE_TRACE(...)     ::Elevate::Log::Trace(__VA_ARGS__)
#define EE_CORE_INFO(...)      ::Elevate::Log::Info(__VA_ARGS__)
#define EE_CORE_WARN(...)      ::Elevate::Log::Warn(__VA_ARGS__)
#define EE_CORE_ERROR(...)     ::Elevate::Log::Error(__VA_ARGS__)
#define EE_CORE_FATAL(...)     ::Elevate::Log::Fatal(__VA_ARGS__)

// Conditional logging
#define EE_CORE_CTRACE(condition, ...)  if(condition) { EE_CORE_TRACE(__VA_ARGS__); }
#define EE_CORE_CINFO(condition, ...)   if(condition) { EE_CORE_INFO(__VA_ARGS__);  }
#define EE_CORE_CWARN(condition, ...)   if(condition) { EE_CORE_WARN(__VA_ARGS__);  }
#define EE_CORE_CERROR(condition, ...)  if(condition) { EE_CORE_ERROR(__VA_ARGS__); }
#define EE_CORE_CFATAL(condition, ...)  if(condition) { EE_CORE_FATAL(__VA_ARGS__); }
#endif

// Client log macros
#define EE_TRACE(...)          ::Elevate::Log::ClientTrace(__VA_ARGS__)
#define EE_INFO(...)           ::Elevate::Log::ClientInfo(__VA_ARGS__)
#define EE_WARN(...)           ::Elevate::Log::ClientWarn(__VA_ARGS__)
#define EE_ERROR(...)          ::Elevate::Log::ClientError(__VA_ARGS__)
#define EE_FATAL(...)          ::Elevate::Log::ClientFatal(__VA_ARGS__)

// Conditional logging
#define EE_CTRACE(condition, ...)  if(condition) { EE_TRACE(__VA_ARGS__); }
#define EE_CINFO(condition, ...)   if(condition) { EE_INFO(__VA_ARGS__);  }
#define EE_CWARN(condition, ...)   if(condition) { EE_WARN(__VA_ARGS__);  }
#define EE_CERROR(condition, ...)  if(condition) { EE_ERROR(__VA_ARGS__); }
#define EE_CFATAL(condition, ...)  if(condition) { EE_FATAL(__VA_ARGS__); }