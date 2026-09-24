#include "PhysicsSystem.h"

#include <ElevateEngine/Core/Asset.h>

namespace Elevate
{
	PhysicsSystem* PhysicsSystem::s_Instance = nullptr;

	PhysicsSystem& PhysicsSystem::Get()
	{
		EE_CORE_ASSERT(s_Instance, "PhysicsEngine has not been initialized!");
		return *s_Instance;
	}

	bool PhysicsSystem::IsInitialized()
	{
		return s_Instance != nullptr;
	}
}
