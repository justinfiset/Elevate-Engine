#pragma once
#include "Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Elevate { 
	class EE_API Log
	{
	public:
		static void Init();
	
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define EE_CORE_TRACE(...)     ::Elevate::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EE_CORE_INFO(...)      ::Elevate::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EE_CORE_WARN(...)      ::Elevate::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EE_CORE_ERROR(...)     ::Elevate::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EE_CORE_FATAL(...)     ::Elevate::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define EE_TRACE(...)          ::Elevate::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RH_INFO(...)           ::Elevate::Log::GetClientLogger()->info(__VA_ARGS__)
#define RH_WARN(...)           ::Elevate::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RH_ERROR(...)          ::Elevate::Log::GetClientLogger()->error(__VA_ARGS__)
#define RH_FATAL(...)          ::Elevate::Log::GetClientLogger()->fatal(__VA_ARGS__)