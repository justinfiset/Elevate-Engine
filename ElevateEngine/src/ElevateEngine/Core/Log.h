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
#endif

// Client log macros
#define EE_TRACE(...)          ::Elevate::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EE_INFO(...)           ::Elevate::Log::GetClientLogger()->info(__VA_ARGS__)
#define EE_WARN(...)           ::Elevate::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EE_ERROR(...)          ::Elevate::Log::GetClientLogger()->error(__VA_ARGS__)
#define EE_FATAL(...)          ::Elevate::Log::GetClientLogger()->fatal(__VA_ARGS__)