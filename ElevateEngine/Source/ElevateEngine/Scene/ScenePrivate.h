#pragma once
#include <cstdint>
#include <entt/entt.hpp>

namespace Elevate
{
	// Getter / Creation / Destruction
	extern entt::registry* TryGetRegistry(uint32_t registryId);
	extern void CreateRegistry(uint32_t registryId);
	extern void DestroyRegistry(uint32_t registryId);

	// Manipulation
	template<typename T>
	T* TryGetFromRegistry(uint32_t registryId, entt::entity entityId)
	{
		auto* registry = TryGetRegistry(registryId);

		if (!registry)
		{
			return nullptr;
		}

		return registry->try_get<T>(entityId);
	}
}