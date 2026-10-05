#include "ScenePrivate.h"

#include <unordered_map>
#include <memory>
#include <entt/entt.hpp>

import Elevate.Foundations;

namespace Elevate
{
	struct RegistryMap
	{
		std::unordered_map<uint32_t, std::unique_ptr<entt::registry>> map;

		~RegistryMap()
		{
			CoreLogger::Trace("Destroying RegistryMap");
		}
	};

	namespace
	{
		RegistryMap s_RegistryMap;
	}

	entt::registry* TryGetRegistry(uint32_t registryId)
	{
		auto it = s_RegistryMap.map.find(registryId);

		if (it == s_RegistryMap.map.end())
		{
			return nullptr;
		}

		return it->second.get();
	}

	void CreateRegistry(uint32_t registryId)
	{
		auto* registry = TryGetRegistry(registryId);

		Assert::That(!registry, "Registry {} already exists.", registryId);

		s_RegistryMap.map.emplace(registryId, std::make_unique<entt::registry>());
	}

	void DestroyRegistry(uint32_t registryId)
	{
		auto it = s_RegistryMap.map.find(registryId);

		if (it == s_RegistryMap.map.end())
		{
			Assert::That(false, "Registry {} does not exist.", registryId);
			return;
		}

		s_RegistryMap.map.erase(it);
	}
}