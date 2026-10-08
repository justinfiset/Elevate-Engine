module;

#include <ElevateEngine/Audio/SoundEngine.h>

module Elevate.Audio.AudioDistanceProbe;

namespace Elevate
{
	void AudioDistanceProbe::Init()
	{
		SoundEngine::SetDistanceProbe(gameObject);
	}

	void AudioDistanceProbe::Destroy()
	{
		SoundEngine::UnsetDistanceProbe();
	}
}
