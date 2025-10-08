#pragma once
#include "Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Elevate { 
	class EE_API Log
	{
	public:
		static void Init();
	
		static inline bool IsInitalized() { return s_loggerInitialized; }

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() 
		{ 
			if(!s_coreLogger)
			{
				Init();
			}
			return s_coreLogger;
		}
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() 
		{ 
			if (!s_clientLogger)
			{
				Init();
			}
			return s_clientLogger; 
		}
	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;

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