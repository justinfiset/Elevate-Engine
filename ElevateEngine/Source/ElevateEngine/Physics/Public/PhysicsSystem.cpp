module Elevate.Physics;

import Elevate.Foundations.Assert;

namespace Elevate
{
	PhysicsSystem* PhysicsSystem::s_Instance = nullptr;

	PhysicsSystem& PhysicsSystem::Get()
	{
		Assert::That(s_Instance, "PhysicsEngine has not been initialized!");
		return *s_Instance;
	}

	bool PhysicsSystem::IsInitialized()
	{
		return s_Instance != nullptr;
	}
}
