#pragma once

#include <string>
#include <memory>

namespace AK
{
	namespace WwiseAuthoringAPI
	{
		class Client;
	}
}

namespace Elevate
{
	class WAAPIClient
	{
	public:
		virtual ~WAAPIClient();

		inline static WAAPIClient& Get()
		{
			static WAAPIClient Instance;
			return Instance;
		}

		static bool IsConnected();

		// prevent duplication of the singleton
		WAAPIClient(const WAAPIClient&) = delete;
		WAAPIClient& operator=(const WAAPIClient&) = delete;

		bool Connect();
		void Disconnect();

	private:
		WAAPIClient();

		static void LoggerCallback(const char* logMessage);

	protected:
		bool m_isConnected = false;

		// todo move in a setting file somehwere
		std::string m_ip = "127.0.0.1";
		uint16_t m_port = 8080;

		AK::WwiseAuthoringAPI::Client* m_client;
	};
}