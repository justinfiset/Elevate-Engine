#include "eepch.h"
#include "SoundEngine.h"

#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Core/Assert.h>

#include <ElevateEngine/Audio/Wwise/WwiseSoundEngine.h>
Elevate::SoundEngine* Elevate::SoundEngine::Impl = new Elevate::WwiseSoundEngine();

bool Elevate::SoundEngine::Init()
{
	EE_CORE_ASSERT(Impl, "Error : No valid SoundEngine impl. found.");
	bool result = Impl->InitImpl();
	EE_CORE_CERROR(!result, "Error (SoundEngine::Init) : Could not initialize the sound engine.");
	return result;
}

void Elevate::SoundEngine::RenderAudio()
{
	Impl->RenderAudioImpl();
}

void Elevate::SoundEngine::Terminate()
{
	Impl->TerminateImpl();
}

void Elevate::SoundEngine::SetDefaultListener(GameObject* obj)
{
	Impl->SetDefaultListenerImpl(obj);
}

void Elevate::SoundEngine::SetDistanceProbe(GameObject* obj)
{
	Impl->SetDistanceProbeImpl(obj);
}

void Elevate::SoundEngine::UnsetDistanceProbe()
{
	Impl->UnsetDistanceProbeImpl();
}

void Elevate::SoundEngine::RegisterGameObject(GameObject* obj)
{
	Impl->RegisterGameObjectImpl(obj);
}

void Elevate::SoundEngine::UnregisterGameObject(GameObject* obj)
{
	Impl->UnregisterGameObjectImpl(obj);
}

void Elevate::SoundEngine::UpdatePosition(GameObject* obj)
{
	Impl->UpdateObjectPositionImpl(obj);
}

void Elevate::SoundEngine::PostEvent(const char* eventName, GameObject* object)
{
	Impl->PostEventImpl(eventName, object);
}