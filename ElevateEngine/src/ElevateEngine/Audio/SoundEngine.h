#pragma once

namespace Elevate
{
    class GameObject;
    
    class SoundEngine
    {
    public:
        static void SetDefaultListener(GameObject* obj);
        static void RegisterGameObject(GameObject* obj);
        static void UnregisterGameObject(GameObject* obj);
    protected:
        virtual void RegisterGameObjectImpl(GameObject* obj) = 0;
        virtual void UnregisterGameObjectImpl(GameObject* obj) = 0;
    };
}