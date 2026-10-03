#include "ScenePrivate.h"

#include <unordered_map>
#include <memory>
#include <entt/entt.hpp>

import Elevate.Foundations;

namespace Elevate
{
	namespace
	{
		std::unordered_map<uint32_t, std::unique_ptr<entt::registry>> s_RegistryMap;
	}

	entt::registry* TryGetRegistry(uint32_t registryId)
	{
		auto it = s_RegistryMap.find(registryId);

		if (it == s_RegistryMap.end())
		{
			return nullptr;
		}

		return it->second.get();
	}

	void CreateRegistry(uint32_t registryId)
	{
		auto* registry = TryGetRegistry(registryId);

		Assert::That(!registry, "Registry {} already exists.", registryId);

		s_RegistryMap.emplace(registryId, std::make_unique<entt::registry>());
	}

	void DestroyRegistry(uint32_t registryId)
	{
		auto it = s_RegistryMap.find(registryId);

		if (it == s_RegistryMap.end())
		{
			Assert::That(false, "Registry {} does not exist.", registryId);
			return;
		}

		s_RegistryMap.erase(it);
	}
}