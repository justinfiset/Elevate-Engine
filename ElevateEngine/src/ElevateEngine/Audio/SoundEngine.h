#pragma once

#include <memory>

namespace Elevate
{
    class GameObject;
    
    class SoundEngine
    {
    public:
        static bool Init();
        static void RenderAudio();
        static void Terminate();

        static void SetDefaultListener(GameObject* obj);
        static void SetDistanceProbe(GameObject* obj);
        static void UnsetDistanceProbe();

        static void RegisterGameObject(GameObject* obj);
        static void UnregisterGameObject(GameObject* obj);
        static void UpdatePosition(GameObject* obj);

        static void PostEvent(const char* eventName, GameObject* object);

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

    private:
        static SoundEngine* Impl;
    };
}