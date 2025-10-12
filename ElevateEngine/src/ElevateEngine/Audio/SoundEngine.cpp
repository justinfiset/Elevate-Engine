#include "eepch.h"
#include "SoundEngine.h"

#include <ElevateEngine/Core/GameObject.h>

// TODO : REMOVE THIS FROM HERE, THIS SHOULD BE IN AN IMPLEMENTATION
#include <AK/SoundEngine/Common/AkSoundEngine.h>

void Elevate::SoundEngine::SetDefaultListener(GameObject* obj)
{
	// todo move in an impl. This is a simple workaround to have the expected result as fast as possible
	// TODO: IMPL A WAY TO MAKE SURE THE OBJECT WAS ALREADY REGISTERED VIA THE SOUND ENGINE
	AkGameObjectID listenerId = obj->GetEntityId();
	AK::SoundEngine::SetDefaultListeners(&listenerId, 1);
}

void Elevate::SoundEngine::RegisterGameObject(GameObject* obj)
{
	// todo move in an impl. This is a simple workaround to have the expected result as fast as possible
	if (obj)
	{
		AkGameObjectID id = obj->GetEntityId();
		AK::SoundEngine::RegisterGameObj(id, obj->GetName().c_str());
		EE_CORE_TRACE("SoundEngine registed gO => (name : %d), (id : %s)", obj->GetEntityId(), obj->GetName().c_str());
	}
}

void Elevate::SoundEngine::UnregisterGameObject(GameObject* obj)
{
	if (obj)
	{
		AkGameObjectID id = obj->GetEntityId();
		AK::SoundEngine::UnregisterGameObj(id);
	}
}
