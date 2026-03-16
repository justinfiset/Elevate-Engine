#pragma once

#include <cstdint>

namespace Elevate
{
	class GameObject;

	class SoundEngine
	{
	public:
		static void SetImplementation(SoundEngine* implementation)
		{
			Impl = implementation;
		}

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
		static void PostEvent(uint32_t eventId, GameObject* object);
		static void PostEvent(const char* eventName);
		static void PostEvent(uint32_t eventId);

		static void Wakeup();
		static void Suspend(bool renderAnyway = false, bool fadeOut = true);

		static SoundEngine* GetImpl();

	protected:
		virtual bool InitImpl() = 0;
		virtual void RenderAudioImpl() = 0;
		virtual void TerminateImpl() = 0;

		virtual void WakeUpImpl() = 0;
		virtual void SuspendImpl(bool renderAnyway, bool fadeOut) = 0;

		virtual void SetDefaultListenerImpl(GameObject* obj) = 0;
		virtual void SetDistanceProbeImpl(GameObject* obj) = 0;
		virtual void UnsetDistanceProbeImpl() = 0;

		virtual void RegisterGameObjectImpl(GameObject* obj) = 0;
		virtual void UnregisterGameObjectImpl(GameObject* obj) = 0;
		virtual void UpdateObjectPositionImpl(GameObject* obj) = 0;

		virtual void PostEventImpl(const char* eventName, GameObject* object) = 0;
		virtual void PostEventImpl(uint32_t eventName, GameObject* object) = 0;
		virtual void PostEventImpl(const char* eventName) = 0;
		virtual void PostEventImpl(uint32_t eventName) = 0;

	private:
		static SoundEngine* Impl;
	};
}