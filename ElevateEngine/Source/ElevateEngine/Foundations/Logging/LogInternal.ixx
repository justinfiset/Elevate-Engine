module;

#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <cstdint>

#include <spdlog/fmt/bundled/base.h>
#include <spdlog/logger.h>

export module Elevate.Foundations.Logger.Internal;

export import Elevate.Foundations.Callbacks;
export import Elevate.Foundations.Logger.LogCallback;

export namespace Elevate
{
	struct LogCallbackEntry
	{
		CallbackID Id;
		LogCallback Callback;
	};

	namespace Internal
	{
		class LogImpl
		{
		public:
			LogImpl(std::shared_ptr<spdlog::logger> logger);
			void Trace(const std::string& text);
			void Info(const std::string& text);
			void Warn(const std::string& text);
			void Error(const std::string& text);
			void Fatal(const std::string& text);

			CallbackID AddCallback(LogCallback callback);
			void RemoveCallback(CallbackID callbackId);

		private:
			void NotifyCallbacks(LogLevel level, const std::string& msg);

		private:
			std::vector<LogCallbackEntry> m_callbacks;
			std::shared_ptr<spdlog::logger> m_logger;
			CallbackID m_nextCallbackId;
		};

		class LogDispatcher
		{
		public:
			static inline void Trace(Internal::LogImpl* logger, const std::string& text)
			{
				if (logger)
					logger->Trace(text);
			}
			static inline void Info(Internal::LogImpl* logger, const std::string& text)
			{
				if (logger)
					logger->Info(text);
			}
			static inline void Warn(Internal::LogImpl* logger, const std::string& text)
			{
				if (logger)
					logger->Warn(text);
			}
			static inline void Error(Internal::LogImpl* logger, const std::string& text)
			{
				if (logger)
					logger->Error(text);
			}
			static inline void Fatal(Internal::LogImpl* logger, const std::string& text)
			{
				if (logger)
					logger->Fatal(text);
			}
		};
	}
}
