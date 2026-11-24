module;


#include <cstdint>

module Elevate.Audio.SoundEngine;

import Elevate.Foundations.CoreLogger;
import Elevate.Audio.Object;

Elevate::SoundEngine* Elevate::SoundEngine::Impl = nullptr;

// Return given arguments if no soundengine is used or no valid implementation is provided.
#if defined(EE_NO_SOUNDENGINE) && (EE_NO_SOUNDENGINE == 1)
#   define EE_CHECK_SOUNDENGINE(...) \
        do { \
            CoreLogger::CWarning(true, "SoundEngine call ignored: EE_NO_SOUNDENGINE is active."); \
            return __VA_ARGS__; \
        } while(0)
#else
#   define EE_CHECK_SOUNDENGINE(...) \
        do { \
            if (!Impl) { \
                CoreLogger::CError(true, "SoundEngine call failed: Impl is nullptr."); \
                return __VA_ARGS__; \
            } \
        } while(0)
#endif

bool Elevate::SoundEngine::Init()
{
	EE_CHECK_SOUNDENGINE(false);
	bool result = Impl->InitImpl();
	CoreLogger::CError(!result, "Error (SoundEngine::Init) : Could not initialize the sound engine.");
	return result;
	// todo register to native window callbacks
}

void Elevate::SoundEngine::RenderAudio()
{
	EE_CHECK_SOUNDENGINE();
	Impl->RenderAudioImpl();
}

void Elevate::SoundEngine::Terminate()
{
	EE_CHECK_SOUNDENGINE();
	Impl->TerminateImpl();
}

void Elevate::SoundEngine::SetDefaultListener(IAudioObject* obj)
{
	EE_CHECK_SOUNDENGINE();
	Impl->SetDefaultListenerImpl(obj);
}

void Elevate::SoundEngine::SetDistanceProbe(IAudioObject* obj)
{
	EE_CHECK_SOUNDENGINE();
	Impl->SetDistanceProbeImpl(obj);
}

void Elevate::SoundEngine::UnsetDistanceProbe()
{
	EE_CHECK_SOUNDENGINE();
	Impl->UnsetDistanceProbeImpl();
}

void Elevate::SoundEngine::RegisterGameObject(IAudioObject* obj)
{
	EE_CHECK_SOUNDENGINE();
	Impl->RegisterGameObjectImpl(obj);
}

void Elevate::SoundEngine::UnregisterGameObject(IAudioObject* obj)
{
	EE_CHECK_SOUNDENGINE();
	Impl->UnregisterGameObjectImpl(obj);
}

void Elevate::SoundEngine::UpdatePosition(IAudioObject* obj)
{
	EE_CHECK_SOUNDENGINE();
	Impl->UpdateObjectPositionImpl(obj);
}

void Elevate::SoundEngine::PostEvent(const char* eventName, IAudioObject* object)
{
	EE_CHECK_SOUNDENGINE();
	Impl->PostEventImpl(eventName, object);
}

void Elevate::SoundEngine::PostEvent(uint32_t eventId, IAudioObject* object)
{
	EE_CHECK_SOUNDENGINE();
	Impl->PostEventImpl(eventId, object);
}

void Elevate::SoundEngine::PostEvent(const char* eventName)
{
	EE_CHECK_SOUNDENGINE();
	Impl->PostEventImpl(eventName);
}

void Elevate::SoundEngine::PostEvent(uint32_t eventId)
{
	EE_CHECK_SOUNDENGINE();
	Impl->PostEventImpl(eventId);
}

void Elevate::SoundEngine::Wakeup()
{
	EE_CHECK_SOUNDENGINE();
	Impl->WakeUpImpl();
}

void Elevate::SoundEngine::Suspend(bool renderAnyway, bool fadeOut)
{
	EE_CHECK_SOUNDENGINE();
	Impl->SuspendImpl(renderAnyway, fadeOut);
}

Elevate::SoundEngine* Elevate::SoundEngine::GetImpl()
{
	return Impl;
}