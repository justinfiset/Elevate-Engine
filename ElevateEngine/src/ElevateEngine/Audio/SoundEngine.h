#pragma once

#include <memory>

#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Core/Assert.h>

#define EE_CHECK_SOUNDENGINE(x) if(EE_NO_SOUNDENGINE == 1) { return x; }

namespace Elevate
{
    class GameObject;
    
    class SoundEngine
    {
    public:
        static inline bool Init()
        {
            // todo find a way to put back core assert and logging
            EE_CHECK_SOUNDENGINE(false);
            EE_ASSERT(Impl, "Error : No valid SoundEngine impl. found.");
            bool result = Impl->InitImpl();
            EE_CERROR(!result, "Error (SoundEngine::Init) : Could not initialize the sound engine.");
            return result;
        }

        static inline void RenderAudio()
        {
            EE_CHECK_SOUNDENGINE();
            Impl->RenderAudioImpl();
        }

        static inline void Terminate()
        {
            EE_CHECK_SOUNDENGINE();
            Impl->TerminateImpl();
        }

        static inline void SetDefaultListener(GameObject* obj)
        {
            EE_CHECK_SOUNDENGINE();
            Impl->SetDefaultListenerImpl(obj);
        }

        static inline void SetDistanceProbe(GameObject* obj)
        {
            EE_CHECK_SOUNDENGINE();
            Impl->SetDistanceProbeImpl(obj);
        }

        static inline void UnsetDistanceProbe()
        {
            EE_CHECK_SOUNDENGINE();
            Impl->UnsetDistanceProbeImpl();
        }

        static inline void RegisterGameObject(GameObject* obj)
        {
            EE_CHECK_SOUNDENGINE();
            Impl->RegisterGameObjectImpl(obj);
        }

        static inline void UnregisterGameObject(GameObject* obj)
        {
            EE_CHECK_SOUNDENGINE();
            Impl->UnregisterGameObjectImpl(obj);
        }

        static inline void UpdatePosition(GameObject* obj)
        {
            EE_CHECK_SOUNDENGINE();
            Impl->UpdateObjectPositionImpl(obj);
        }

        static inline void PostEvent(const char* eventName, GameObject* object)
        {
            EE_CHECK_SOUNDENGINE();
            Impl->PostEventImpl(eventName, object);
        }

        static inline void PostEvent(uint16_t eventId, GameObject* object)
        {
            EE_CHECK_SOUNDENGINE();
            Impl->PostEventImpl(eventId, object);
        }

        static inline void PostEvent(const char* eventName)
        {
            EE_CHECK_SOUNDENGINE();
            Impl->PostEventImpl(eventName);
        }

        static inline void PostEvent(uint16_t eventId)
        {
            EE_CHECK_SOUNDENGINE();
            Impl->PostEventImpl(eventId);
        }

        static inline SoundEngine* GetImpl() { return Impl; }

    protected:
        virtual bool InitImpl() = 0;
        virtual void RenderAudioImpl() = 0;
        virtual void TerminateImpl() = 0;

        virtual void SetDefaultListenerImpl(GameObject* obj) = 0;
        virtual void SetDistanceProbeImpl(GameObject* obj) = 0;
        virtual void UnsetDistanceProbeImpl() = 0;

        virtual void RegisterGameObjectImpl(GameObject* obj) = 0;
        virtual void UnregisterGameObjectImpl(GameObject* obj) = 0;
        virtual void UpdateObjectPositionImpl(GameObject* obj) = 0;

        virtual void PostEventImpl(const char* eventName, GameObject* object) = 0;
        virtual void PostEventImpl(uint16_t eventName, GameObject* object) = 0;
        virtual void PostEventImpl(const char* eventName) = 0;
        virtual void PostEventImpl(uint16_t eventName) = 0;
    private:
        static SoundEngine* Impl;
    };
}