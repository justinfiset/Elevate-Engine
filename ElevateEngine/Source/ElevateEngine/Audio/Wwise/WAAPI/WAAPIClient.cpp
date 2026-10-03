#include "WAAPIClient.h"

#if defined(EE_USES_WWISE) && !defined(EE_PLATFORM_WEB)
	import Elevate.Foundations;
	#include <AK/WwiseAuthoringAPI/AkAutobahn/Client.h>
	#include <AK/WwiseAuthoringAPI/AkAutobahn/Logger.h>
	#include <AK/WwiseAuthoringAPI/waapi.h>
	#define EE_WAAPI_AVAILABLE 1
#else
    #define EE_WAAPI_AVAILABLE 0
#endif

namespace Elevate
{
	bool WAAPIClient::Connect()
	{
#if EE_WAAPI_AVAILABLE
		m_isConnected = m_client->Connect(m_ip.c_str(), m_port);
		if (!m_isConnected)
		{
			CoreLogger::Error("Could not connect to WAAPI.");
		}

		// todo remove this test
		using namespace AK::WwiseAuthoringAPI;
		AkJson wwiseInfoJson;
		if (!m_client->Call(ak::wwise::core::getInfo, AkJson(AkJson::Type::Map), AkJson(AkJson::Type::Map), wwiseInfoJson, 10))
		{
			CoreLogger::Error("Failed to obtain Wwise Info within 10ms: {}", std::string(wwiseInfoJson["message"].GetVariant()));
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
		// todo create an async system to try and reconnect if not working
		// 1s -> 2s -> 4s -> 8s -> 16s mult. tiime by 2 each time.
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