#include "WAAPIClient.h"

#include <ElevateEngine/Core/Log.h>

#ifdef EE_USES_WWISE
#include <AK/WwiseAuthoringAPI/AkAutobahn/Client.h>
#include <AK/WwiseAuthoringAPI/AkAutobahn/Logger.h>
#include <AK/WwiseAuthoringAPI/waapi.h>
#endif

namespace Elevate
{
	bool WAAPIClient::Connect()
	{
#ifdef EE_USES_WWISE
		m_isConnected = m_client->Connect(m_ip.c_str(), m_port);
		if (!m_isConnected)
		{
			EE_CORE_ERROR("Could not connect to WAAPI.");
		}

		// todo remove this test
		using namespace AK::WwiseAuthoringAPI;
		AkJson wwiseInfoJson;
		if (!m_client->Call(ak::wwise::core::getInfo, AkJson(AkJson::Type::Map), AkJson(AkJson::Type::Map), wwiseInfoJson, 10))
		{
			EE_CORE_ERROR("Failed to obtain Wwise Info within 10ms: {}", std::string(wwiseInfoJson["message"].GetVariant()));
		}

		// todo remove this test
		// Fetch all of the supported objects via WAAPI
		// use this as the wwise data source
		//const AkJson args( AkJson::Map{
		//	{ "from", AkJson::Map{ { "ofType", AkJson::Array{
		//		AkVariant("Bus"),
		//		AkVariant("Event"),
		//		AkVariant("SoundBank"),
		//		AkVariant("State"),
		//		AkVariant("Switch"),
		//		AkVariant("SwitchGroup"),
		//		AkVariant("StateGroup"),
		//	} } } }
		//});

		//const AkJson options(AkJson::Map{
		//	{ "return", AkJson::Array{
		//		AkVariant("name"),
		//		AkVariant("id"),
		//		AkVariant("shortId"),
		//		AkVariant("type"),
		//		AkVariant("path"),
		//	} }
		//});

		//AkJson res;
		//if (!m_client->Call(ak::wwise::core::object::get, args, options, res, 2000))
		//{
		//	EE_CORE_ERROR("Failed to fetch all of the objects in the Wwise Project.");
		//}
		//else
		//{
		//	if (res.IsMap() && res.HasKey("return"));
		//	{
		//		auto resArr = res["return"];
		//		for (auto obj : resArr.GetArray())
		//		{
		//			std::string name = obj["name"].GetVariant();
		//			std::string guid = obj["id"].GetVariant();
		//			std::string type = obj["type"].GetVariant();
		//			std::string path = obj["path"].GetVariant();
		//			uint64_t shortId = obj["shortId"].GetVariant();
		//		}
		//	}
		//}

		return m_isConnected;
#else
		return false;
#endif
	}

	void WAAPIClient::Disconnect()
	{
#ifdef EE_USES_WWISE
		if (m_client)
		{
			m_client->Disconnect();
		}
#endif
	}

	WAAPIClient::WAAPIClient()
	{
#ifdef EE_USES_WWISE
		m_client = new AK::WwiseAuthoringAPI::Client();

		AK::WwiseAuthoringAPI::Logger::Get()->SetLoggerFunction(LoggerCallback);

		Connect();
		// todo create an async system to try and reconnect if not working
		// 1s -> 2s -> 4s -> 8s -> 16s mult. tiime by 2 each time.
#endif
	}

	WAAPIClient::~WAAPIClient()
	{
#ifdef EE_USES_WWISE
		delete m_client;
#endif
	}

	void WAAPIClient::LoggerCallback(const char* logMessage)
	{
		// todo use a custom logger here to know who prints what
		EE_CORE_TRACE("{}", logMessage);
	}

	bool WAAPIClient::IsConnected()
	{
#ifdef EE_USES_WWISE
		return Get().m_isConnected;
#else
		return false;
#endif
	}
}