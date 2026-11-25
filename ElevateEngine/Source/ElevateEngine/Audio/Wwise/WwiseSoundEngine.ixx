module;

#include <string>
#include <memory>

#ifdef EE_USES_WWISE

#define EE_AUDIO_VORBIS 1
#define EE_AUDIO_OPUS 1

#include <ElevateEngine/Audio/Ak/Common/AkFilePackageLowLevelIODeferred.h>

#ifdef EE_EDITOR_BUILD
#include <ElevateEngine/Audio/Wwise/DataSources/WwiseMergedDataSource.h>
#endif

#endif // #ifdef EE_USES_WWISE

export module Elevate.Audio.Wwise.SoundEngine;

#ifdef EE_USES_WWISE

import Elevate.Audio.Object;
import Elevate.Audio.SoundEngine;

export namespace Elevate
{
	class WwiseSoundEngine : public SoundEngine
	{
	public:
		static WwiseSoundEngine* Get();

		bool LoadBank(const wchar_t* bankName);
		bool LoadBank(const std::wstring& bankName);
		bool LoadBank(const char* bankName);
		bool LoadBank(std::string& bankName);

#ifdef EE_EDITOR_BUILD
		virtual std::shared_ptr<WwiseDataSource> GetDataSource();
#endif

	protected:
		bool InitImpl() override;
		void RenderAudioImpl() override;
		void TerminateImpl() override;
		void WakeUpImpl() override;
		void SuspendImpl(bool renderAnyway, bool fadeOut) override;

		void SetDefaultListenerImpl(IAudioObject* obj) override;
		void SetDistanceProbeImpl(IAudioObject* obj) override;
		void UnsetDistanceProbeImpl() override;

		void RegisterGameObjectImpl(IAudioObject* obj) override;
		void UnregisterGameObjectImpl(IAudioObject* obj) override;
		void UpdateObjectPositionImpl(IAudioObject* obj) override;

		void PostEventImpl(const char* eventName, IAudioObject* object) override;
		void PostEventImpl(uint32_t eventId, IAudioObject* object) override;
		void PostEventImpl(const char* eventName) override;
		void PostEventImpl(uint32_t eventId) override;
	private:
		void PrepareAudio();

	private:
		AkGameObjectID m_currentListenerID;
		std::unique_ptr<CAkFilePackageLowLevelIODeferred> m_lowLevelIO;

#ifdef EE_EDITOR_BUILD
		std::shared_ptr<WwiseMergedDataSource> m_mergedDataSource;
#endif
	};
}

#endif // #ifdef EE_USES_WWISE
