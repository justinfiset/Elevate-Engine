module;

#include <cstdint>

export module Elevate.Audio.SoundEngine;

import Elevate.Audio.Object;

export namespace Elevate
{
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

		static void SetDefaultListener(IAudioObject* obj);
		static void SetDistanceProbe(IAudioObject* obj);
		static void UnsetDistanceProbe();

		static void RegisterGameObject(IAudioObject* obj);
		static void UnregisterGameObject(IAudioObject* obj);
		static void UpdatePosition(IAudioObject* obj);

		static void PostEvent(const char* eventName, IAudioObject* object);
		static void PostEvent(uint32_t eventId, IAudioObject* object);
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

		virtual void SetDefaultListenerImpl(IAudioObject* obj) = 0;
		virtual void SetDistanceProbeImpl(IAudioObject* obj) = 0;
		virtual void UnsetDistanceProbeImpl() = 0;

		virtual void RegisterGameObjectImpl(IAudioObject* obj) = 0;
		virtual void UnregisterGameObjectImpl(IAudioObject* obj) = 0;
		virtual void UpdateObjectPositionImpl(IAudioObject* obj) = 0;

		virtual void PostEventImpl(const char* eventName, IAudioObject* object) = 0;
		virtual void PostEventImpl(uint32_t eventName, IAudioObject* object) = 0;
		virtual void PostEventImpl(const char* eventName) = 0;
		virtual void PostEventImpl(uint32_t eventName) = 0;

	private:
		static SoundEngine* Impl;
	};
}