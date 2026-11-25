module;

module Elevate.Audio.AudioDistanceProbe;

import Elevate.Audio.SoundEngine;

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
