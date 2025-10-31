#include "eepch.h"

#include <filesystem>

#include "WwiseSoundEngine.h"

#include <ElevateEngine/Core/GameObject.h>

#include <glm/vec3.hpp>

// Sound Engine
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>	
#include <AK/SoundEngine/Common/AkMemoryMgrModule.h>

// Spatial audio
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>

// Wwise Communication
#ifndef AK_OPTIMIZED // Keep the following header out in release mode
#include <AK/Comm/AkCommunication.h>
#endif // AK_OPTIMIZED

// Decoders
#if EE_AUDIO_VORBIS
#include <AK/Plugin/AkVorbisDecoderFactory.h> // This is necessary to be able to play vorbis files
#endif // EE_AUDIO_VORBIS
#if EE_AUDIO_OPUS
#include <AK/Plugin/AkOpusDecoderFactory.h>   // This is necessary to be able to play opus files
#endif // EE_AUDIO_OPUS

// TODO REMOVE
// For Wwise integration testing
#define BANKNAME_INIT L"Init.bnk"
#define BANKNAME_CAR L"Car.bnk"
#define BANKNAME_HUMAN L"Human.bnk"
#define BANKNAME_MARKERTEST L"MarkerTest.bnk"

#ifdef EE_EDITOR_BUILD
#include <ElevateEngine/Audio/Wwise/DataSources/WwiseFileDataSource.h>
#endif

namespace Elevate
{
	bool WwiseSoundEngine::InitImpl()
	{
		EE_CORE_INFO("Initializing Wwise Sound Engine...");

		// Creating the memory manager
		AkMemSettings memorySettings;
		AK::MemoryMgr::GetDefaultSettings(memorySettings);
		if (AK::MemoryMgr::Init(&memorySettings) != AK_Success)
		{
			EE_CORE_ASSERT(false, "ERROR: Failed to initialize the Wwise memory manager.");
			return false;
		}
		else
		{
			EE_CORE_TRACE("Wwise memory manager initialized!");
		}

		// Creating the stream manager
		AkStreamMgrSettings streamSettings;
		AK::StreamMgr::GetDefaultSettings(streamSettings);
		if (!AK::StreamMgr::Create(streamSettings))
		{
			EE_CORE_ASSERT(false, "ERROR: Failed to create the Wwise stream manager.");
			return false;
		}
		else
		{
			EE_CORE_TRACE("Wwise stream manager initialized!");
		}

		// Creating a streaming device
		AkDeviceSettings deviceSettings;
		AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

		m_lowLevelIO = std::make_unique<CAkFilePackageLowLevelIODeferred>();
		if (m_lowLevelIO->Init(deviceSettings) != AK_Success)
		{
			EE_CORE_ASSERT(false, "ERROR: Failed to create the Wwise streaming device and low-level I/O system.");
			return false;
		}
		else
		{
			EE_CORE_TRACE("Wwise streaming device and low-level I/O initialized!");
		}

		// Initializing the sound engine itself
		AkInitSettings initSettings;
		AK::SoundEngine::GetDefaultInitSettings(initSettings);

		AkPlatformInitSettings platformInitSettings;
		AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

		if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
		{
			EE_CORE_ASSERT(false, "ERROR: Failded to initialize the Sound Engine.");
			return false;
		}
		else
		{
			EE_CORE_TRACE("Wwise SoundEngine initialized!");
		}

		// Initializing the interactive music engine
		AkSpatialAudioInitSettings spatialAudioSettings;
		if (AK::SpatialAudio::Init(spatialAudioSettings) != AK_Success)
		{
			EE_CORE_ASSERT(false, "ERROR: Failded to initialize Spatial Audio.");
			return false;
		}
		else
		{
			EE_CORE_TRACE("Wwise Spatial Audio initialized!");
		}

#ifndef AK_OPTIMIZED
		AkCommSettings communicationSettings;
		AK::Comm::GetDefaultInitSettings(communicationSettings);
		if (AK::Comm::Init(communicationSettings) != AK_Success)
		{
			EE_CORE_ASSERT(false, "ERROR: Failded to initialize Wwise communication.");
			return false;
		}
		else
		{
			EE_CORE_TRACE("Wwise communication initialized!");
		}
#endif // AK_OPTIMIZED

		EE_CORE_INFO("Wwise Initialized!");

		PrepareAudio();

#ifdef EE_EDITOR_BUILD
		// this is our test impl to check and discover wwise elemetns from the file browser
		std::string wwiseProjectPath = "./WwiseProject"; // todo get from the user
		m_fileDataSource.reset(new WwiseFileDataSource(wwiseProjectPath));;
		m_fileDataSource->InitializeSource();
#endif

		return true;
	}

	void WwiseSoundEngine::PrepareAudio()
	{
		m_lowLevelIO->SetBasePath(AKTEXT("./WwiseProject/GeneratedSoundBanks/Windows/"));
		AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

		// TODO TRANSFORM IN A FUNCITON AND HAVE A BETTER ERROR LOGGING IN CASE OF ACCIDENT
		// + RETURN TRUE / FALSE IN LOADED OR NO
		// Load banks synchronously (from file name).
		AkBankID bankID; // Not used. These banks can be unloaded with their file name.
		AKRESULT eResult = AK::SoundEngine::LoadBank(BANKNAME_INIT, bankID);
		EE_CORE_CERROR(eResult != AK_Success, "ERROR: Failed to load SoundBank.");
		eResult = AK::SoundEngine::LoadBank(BANKNAME_CAR, bankID);
		EE_CORE_CERROR(eResult != AK_Success, "ERROR: Failed to load SoundBank.");
		eResult = AK::SoundEngine::LoadBank(BANKNAME_HUMAN, bankID);
		EE_CORE_CERROR(eResult != AK_Success, "ERROR: Failed to load SoundBank.");
		eResult = AK::SoundEngine::LoadBank(BANKNAME_MARKERTEST, bankID);
		EE_CORE_CERROR(eResult != AK_Success, "ERROR: Failed to load SoundBank.");
	}

	void WwiseSoundEngine::RenderAudioImpl()
	{
		AK::SoundEngine::RenderAudio();
	}

	void WwiseSoundEngine::TerminateImpl()
	{
#ifndef AK_OPTIMIZED
		AK::Comm::Term();
#endif // AK_OPTIMIZED

		AK::SoundEngine::Term();

		m_lowLevelIO->Term();

		if (AK::IAkStreamMgr::Get())
		{
			AK::IAkStreamMgr::Get()->Destroy();
		}

		AK::MemoryMgr::Term();
	}

	void WwiseSoundEngine::SetDefaultListenerImpl(GameObject* obj)
	{
		if (obj)
		{
			m_currentListenerID = obj->GetObjectId();
			AK::SoundEngine::SetDefaultListeners(&m_currentListenerID, 1);
		}
	}

	void WwiseSoundEngine::SetDistanceProbeImpl(GameObject* obj)
	{
		if (obj)
		{
			AK::SoundEngine::SetDistanceProbe(m_currentListenerID, obj->GetObjectId());
		}
	}

	void WwiseSoundEngine::UnsetDistanceProbeImpl()
	{
		AK::SoundEngine::SetDistanceProbe(m_currentListenerID, m_currentListenerID);
	}

	void WwiseSoundEngine::RegisterGameObjectImpl(GameObject* obj)
	{
		if (obj)
		{
			AkGameObjectID id = obj->GetObjectId();
			if (AK::SoundEngine::RegisterGameObj(id, obj->GetName().c_str()) != AK_Success)
			{
				EE_CORE_ERROR("SoundEngine Error : Unable to register gO => (name : %d), (id : %s)", obj->GetObjectId(), obj->GetName().c_str());
			}
			else
			{
				EE_CORE_TRACE("SoundEngine registed gO => (name : %d), (id : %s)", obj->GetObjectId(), obj->GetName().c_str());
			}
		}
	}

	void WwiseSoundEngine::UnregisterGameObjectImpl(GameObject* obj)
	{
		if (obj)
		{
			AkGameObjectID id = obj->GetObjectId();
			AK::SoundEngine::UnregisterGameObj(id);
		}
	}

	void WwiseSoundEngine::UpdateObjectPositionImpl(GameObject* obj)
	{
		if (obj)
		{
			const glm::vec3 objectPosition = obj->GetPosition();
			AkGameObjectID id = obj->GetObjectId();

			const Transform& transform = obj->GetTransform();
			glm::vec3 top = glm::normalize(transform.GetUp());
			glm::vec3 front = glm::normalize(transform.GetForward());

			AkSoundPosition soundPosition;
			soundPosition.SetPosition(objectPosition.x, objectPosition.y, objectPosition.z);
			soundPosition.SetOrientation(front.x, front.y, front.z, top.x, top.y, top.z);

			if (AK::SoundEngine::SetPosition(id, soundPosition) == AK_InvalidParameter)
			{
				EE_CORE_ERROR("SoundEngine Error : could not set the transform values for {} with values [{}, {}, {}]", obj->GetName(), objectPosition.x, objectPosition.y, objectPosition.z);
			}
		}
	}

	void WwiseSoundEngine::PostEventImpl(const char* eventName, GameObject* obj)
	{
		if (obj)	
		{
			AkGameObjectID id = obj->GetObjectId();
			AK::SoundEngine::PostEvent(eventName, id);
		}
		else
		{
			EE_CORE_ERROR("Error (WwiseSoundEngine::PostEventImpl) : Cannot play an event on a null GameObject.");
		}
	}

	void WwiseSoundEngine::PostEventImpl(uint16_t eventId, GameObject* obj)
	{
		if (obj)
		{
			AkGameObjectID id = obj->GetObjectId();
			AK::SoundEngine::PostEvent(eventId, id);
		}
		else
		{
			EE_CORE_ERROR("Error (WwiseSoundEngine::PostEventImpl) : Cannot play an event on a null GameObject.");
		}
	}

	void WwiseSoundEngine::PostEventImpl(const char* eventName)
	{
		AK::SoundEngine::PostEvent(eventName, m_currentListenerID);
	}

	void WwiseSoundEngine::PostEventImpl(uint16_t eventId)
	{
		AK::SoundEngine::PostEvent(eventId, m_currentListenerID);
	}

#ifdef EE_EDITOR_BUILD
	std::shared_ptr<WwiseFileDataSource> WwiseSoundEngine::GetFileDataSource()
	{
		return m_fileDataSource;
	}
#endif
}