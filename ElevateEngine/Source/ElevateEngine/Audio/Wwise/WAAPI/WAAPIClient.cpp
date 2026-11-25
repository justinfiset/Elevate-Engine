module;

#if defined(EE_USES_WWISE) && !defined(EE_PLATFORM_WEB)
#include <AK/WwiseAuthoringAPI/AkAutobahn/Client.h>
#include <AK/WwiseAuthoringAPI/AkAutobahn/Logger.h>
#include <AK/WwiseAuthoringAPI/waapi.h>
#define EE_WAAPI_AVAILABLE 1
#else
#define EE_WAAPI_AVAILABLE 0
#endif

module Elevate.Audio.Wwise.WAAPI.Client;

import Elevate.Foundations.CoreLogger;

namespace Elevate
{
	bool WAAPIClient::Connect()
	{
#if EE_WAAPI_AVAILABLE
		m_isConnected = m_client->Connect(m_ip.c_str(), m_port);
		if (!m_isConnected)
		{
			CoreLogger::Error("Could not connect to WAAPI ({}:{}).", m_ip, m_port);
		}
		return m_isConnected;
#else
		return false;
#endif
	}

	void WAAPIClient::Disconnect()
	{
#if EE_WAAPI_AVAILABLE
		if (m_client)
		{
			m_client->Disconnect();
		}
#endif
	}

	WAAPIClient::WAAPIClient()
	{
#if EE_WAAPI_AVAILABLE
		m_client = new AK::WwiseAuthoringAPI::Client();

		AK::WwiseAuthoringAPI::Logger::Get()->SetLoggerFunction(LoggerCallback);

		Connect();
#endif
	}

	WAAPIClient::~WAAPIClient()
	{
#if EE_WAAPI_AVAILABLE
		delete m_client;
#endif
	}

	void WAAPIClient::LoggerCallback([[maybe_unused]] const char* logMessage)
	{
#if EE_WAAPI_AVAILABLE
		CoreLogger::Trace("[WAAPIClient] : {}", logMessage);
#endif
	}

	bool WAAPIClient::IsConnected()
	{
#if EE_WAAPI_AVAILABLE
		return Get().m_isConnected;
#else
		return false;
#endif
	}
}