module;

#include <unordered_map>
#include <memory>
#include <cstdint>
#include <entt/entt.hpp>

export module Elevate.Core.Scene.ComponentRegistry;

import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Assert;

export namespace Elevate
{
	struct RegistryMap
	{
		std::unordered_map<uint32_t, std::unique_ptr<entt::registry>> map;

		~RegistryMap()
		{
			CoreLogger::Trace("Destroying Registry Map");
		}
	};

	class ComponentRegistry
	{
	public:
		// Manipulation
		static void CreateRegistry(uint32_t registryId)
		{
			auto* registry = TryGetRegistry(registryId);

			Assert::That(!registry, "Registry {} already exists.", registryId);

			s_Instance.m_RegistryMap.map.emplace(registryId, std::make_unique<entt::registry>());
		}

		static void DestroyRegistry(uint32_t registryId)
		{
			auto it = s_Instance.m_RegistryMap.map.find(registryId);

			if (it == s_Instance.m_RegistryMap.map.end())
			{
				Assert::That(false, "Registry {} does not exist.", registryId);
				return;
			}

			s_Instance.m_RegistryMap.map.erase(it);
		}

	private:
		static entt::registry* TryGetRegistry(uint32_t registryId)
		{
			auto it = s_Instance.m_RegistryMap.map.find(registryId);

			if (it == s_Instance.m_RegistryMap.map.end())
			{
				return nullptr;
			}

			return it->second.get();
		}

		template<typename T>
		static T* TryGetFromRegistry(uint32_t registryId, entt::entity entityId)
		{
			auto* registry = TryGetRegistry(registryId);

			if (!registry)
			{
				return nullptr;
			}

			return registry->try_get<T>(entityId);
		}

	private:
		static ComponentRegistry s_Instance;
		RegistryMap m_RegistryMap;
	};

	ComponentRegistry ComponentRegistry::s_Instance;
}
