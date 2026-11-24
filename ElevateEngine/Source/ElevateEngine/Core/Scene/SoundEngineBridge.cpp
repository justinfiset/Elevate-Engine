#include <ElevateEngine/Core/Scene/SoundEngineBridge.h>

import Elevate.Audio.SoundEngine;
import Elevate.Core.Scene;

namespace Elevate::Internal
{
	namespace
	{
		GameObject* AsGameObject(std::uintptr_t objectAddress)
		{
			return reinterpret_cast<GameObject*>(objectAddress);
		}
	}

	void RegisterSoundObject(std::uintptr_t objectAddress)
	{
		SoundEngine::RegisterGameObject(AsGameObject(objectAddress));
	}

	void UnregisterSoundObject(std::uintptr_t objectAddress)
	{
		SoundEngine::UnregisterGameObject(AsGameObject(objectAddress));
	}

	void UpdateSoundObjectPosition(std::uintptr_t objectAddress)
	{
		SoundEngine::UpdatePosition(AsGameObject(objectAddress));
	}
}
