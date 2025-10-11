#pragma once
#include "Core.h"

namespace Elevate { 
	class EE_API ILogger {
	public:
		virtual ~ILogger() = default;

        template<typename... Args>
        void trace(const char* fmt, const Args&... args) {
            trace_impl(fmt, args...);
        }

        template<typename... Args>
        void info(const char* fmt, const Args&... args) {
            info_impl(fmt, args...);
        }

        template<typename... Args>
        void warn(const char* fmt, const Args&... args) {
            warn_impl(fmt, args...);
        }

        template<typename... Args>
        void error(const char* fmt, const Args&... args) {
            error_impl(fmt, args...);
        }

        template<typename... Args>
        void fatal(const char* fmt, const Args&... args) {
            fatal_impl(fmt, args...);
        }

        void trace(const std::string& message) {
            trace_impl(message.c_str());
        }

        void info(const std::string& message) {
            info_impl(message.c_str());
        }

        void warn(const std::string& message) {
            warn_impl(message.c_str());
        }

        void error(const std::string& message) {
            error_impl(message.c_str());
        }

        void fatal(const std::string& message) {
            fatal_impl(message.c_str());
        }

	protected:
		virtual void trace_impl(const char* fmt, ...) = 0;
		virtual void info_impl(const char* fmt, ...) = 0;
		virtual void warn_impl(const char* fmt, ...) = 0;
		virtual void error_impl(const char* fmt, ...) = 0;
		virtual void fatal_impl(const char* fmt, ...) = 0;
	};

	class EE_API Log
	{
	public:
		static void Init();
	
		static inline bool IsInitalized() { return s_loggerInitialized; }

		static ILogger* GetCoreLogger();
		static ILogger* GetClientLogger();

	private:
		static ILogger* s_coreLogger;
		static ILogger* s_clientLogger;

		static bool s_loggerInitialized;
	};
}

// Core log macros : can only be used by the engine
#if defined(EE_ENGINE_BUILD) || defined(EE_ENGINE_INTERNAL)
	#define EE_CORE_TRACE(...)     ::Elevate::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define EE_CORE_INFO(...)      ::Elevate::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define EE_CORE_WARN(...)      ::Elevate::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define EE_CORE_ERROR(...)     ::Elevate::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define EE_CORE_FATAL(...)     ::Elevate::Log::GetCoreLogger()->fatal(__VA_ARGS__)

	// Conditional logging
	#define EE_CORE_CTRACE(condition, ...)  if(condition) { EE_CORE_TRACE(__VA_ARGS__); }
	#define EE_CORE_CINFO(condition, ...)   if(condition) { EE_CORE_INFO(__VA_ARGS__);  }
	#define EE_CORE_CWARN(condition, ...)   if(condition) { EE_CORE_WARN(__VA_ARGS__);  }
	#define EE_CORE_CERROR(condition, ...)  if(condition) { EE_CORE_ERROR(__VA_ARGS__); }
	#define EE_CORE_CFATAL(condition, ...)  if(condition) { EE_CORE_FATAL(__VA_ARGS__); }
#endif

// Client log macros
#define EE_TRACE(...)          ::Elevate::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EE_INFO(...)           ::Elevate::Log::GetClientLogger()->info(__VA_ARGS__)
#define EE_WARN(...)           ::Elevate::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EE_ERROR(...)          ::Elevate::Log::GetClientLogger()->error(__VA_ARGS__)
#define EE_FATAL(...)          ::Elevate::Log::GetClientLogger()->fatal(__VA_ARGS__)

// Conditional logging
#define EE_CTRACE(condition, ...)  if(condition) { EE_TRACE(__VA_ARGS__); }
#define EE_CINFO(condition, ...)   if(condition) { EE_INFO(__VA_ARGS__);  }
#define EE_CWARN(condition, ...)   if(condition) { EE_WARN(__VA_ARGS__);  }
#define EE_CERROR(condition, ...)  if(condition) { EE_ERROR(__VA_ARGS__); }
#define EE_CFATAL(condition, ...)  if(condition) { EE_FATAL(__VA_ARGS__); }