#include "AudioDistanceProbe.h"

#include <ElevateEngine/Audio/SoundEngine.h>

void Elevate::AudioDistanceProbe::Init()
{
	SoundEngine::SetDistanceProbe(gameObject);
}

void Elevate::AudioDistanceProbe::Destroy()
{
	SoundEngine::UnsetDistanceProbe();
}