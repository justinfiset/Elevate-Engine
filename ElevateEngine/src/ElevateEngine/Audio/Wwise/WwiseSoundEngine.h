#pragma once

#define EE_AUDIO_VORBIS 1
#define EE_AUDIO_OPUS 1

#include <ElevateEngine/Audio/SoundEngine.h>
#include <ElevateEngine/Audio/Ak/Common/AkFilePackageLowLevelIODeferred.h>

namespace Elevate
{
    class GameObject;
    
    class WwiseSoundEngine : public SoundEngine
    {
    protected:
        virtual bool InitImpl() override;
        virtual void RenderAudioImpl() override;
        virtual void TerminateImpl() override;

        virtual void SetDefaultListenerImpl(GameObject* obj) override;
        virtual void SetDistanceProbeImpl(GameObject* obj) override;
        virtual void UnsetDistanceProbeImpl() override;

        virtual void RegisterGameObjectImpl(GameObject* obj) override;
        virtual void UnregisterGameObjectImpl(GameObject* obj) override;
        virtual void UpdateObjectPositionImpl(GameObject* obj) override;

        virtual void PostEventImpl(const char* eventName, GameObject* object) override;

    private:
        void PrepareAudio();

    private:
        AkGameObjectID m_currentListenerID;
        std::unique_ptr<CAkFilePackageLowLevelIODeferred> m_lowLevelIO;
    };
}