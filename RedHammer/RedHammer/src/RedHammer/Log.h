#pragma once
#include "Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Hammer { 
	class RH_API Log
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
#define RH_CORE_TRACE(...)     ::Hammer::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RH_CORE_INFO(...)      ::Hammer::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RH_CORE_WARN(...)      ::Hammer::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RH_CORE_ERROR(...)     ::Hammer::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RH_CORE_FATAL(...)     ::Hammer::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define RH_TRACE(...)          ::Hammer::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RH_INFO(...)           ::Hammer::Log::GetClientLogger()->info(__VA_ARGS__)
#define RH_WARN(...)           ::Hammer::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RH_ERROR(...)          ::Hammer::Log::GetClientLogger()->error(__VA_ARGS__)
#define RH_FATAL(...)          ::Hammer::Log::GetClientLogger()->fatal(__VA_ARGS__)