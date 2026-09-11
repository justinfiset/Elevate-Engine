#include "SoundEngine.h"

#include <format>
#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Core/Core.h>

Elevate::SoundEngine* Elevate::SoundEngine::Impl = nullptr;

// Return given arguments if no soundengine is used or no valid implementation is provided.
#define EE_CHECK_SOUNDENGINE(...) if(EE_NO_SOUNDENGINE == 1 || !Impl) { return __VA_ARGS__; }

bool Elevate::SoundEngine::Init()
{
	EE_CORE_CWARN(!Impl, "Error : No valid SoundEngine impl. found. Using a null SoundEngine.");
	EE_CHECK_SOUNDENGINE(false);
	bool result = Impl->InitImpl();
	EE_CERROR(!result, "Error (SoundEngine::Init) : Could not initialize the sound engine.");
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

void Elevate::SoundEngine::SetDefaultListener(GameObject* obj)
{
	EE_CHECK_SOUNDENGINE();
	Impl->SetDefaultListenerImpl(obj);
}

void Elevate::SoundEngine::SetDistanceProbe(GameObject* obj)
{
	EE_CHECK_SOUNDENGINE();
	Impl->SetDistanceProbeImpl(obj);
}

void Elevate::SoundEngine::UnsetDistanceProbe()
{
	EE_CHECK_SOUNDENGINE();
	Impl->UnsetDistanceProbeImpl();
}

void Elevate::SoundEngine::RegisterGameObject(GameObject* obj)
{
	EE_CHECK_SOUNDENGINE();
	Impl->RegisterGameObjectImpl(obj);
}

void Elevate::SoundEngine::UnregisterGameObject(GameObject* obj)
{
	EE_CHECK_SOUNDENGINE();
	Impl->UnregisterGameObjectImpl(obj);
}

void Elevate::SoundEngine::UpdatePosition(GameObject* obj)
{
	EE_CHECK_SOUNDENGINE();
	Impl->UpdateObjectPositionImpl(obj);
}

void Elevate::SoundEngine::PostEvent(const char* eventName, GameObject* object)
{
	EE_CHECK_SOUNDENGINE();
	Impl->PostEventImpl(eventName, object);
}

void Elevate::SoundEngine::PostEvent(uint32_t eventId, GameObject* object)
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