module;

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <entt/entt.hpp>

module Elevate.Core.Scene;

import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Assert;

namespace Elevate
{
	struct ComponentRegistry::Impl
	{
		std::unordered_map<uint32_t, std::unique_ptr<entt::registry>> Registries;

		entt::registry* TryGet(uint32_t registryId)
		{
			auto it = Registries.find(registryId);

			if (it == Registries.end())
			{
				return nullptr;
			}

			return it->second.get();
		}
	};

	// Special members are defined here, where Impl is a complete type.
	ComponentRegistry::ComponentRegistry()
		: m_Impl(std::make_unique<Impl>())
	{
	}

	ComponentRegistry::~ComponentRegistry()
	{
		// m_Impl is null after a move, so only log for the instance that owns the registries.
		if (m_Impl)
		{
			CoreLogger::Trace("Destroying ComponentRegistry");
		}
	}

	ComponentRegistry::ComponentRegistry(ComponentRegistry&&) noexcept = default;
	ComponentRegistry& ComponentRegistry::operator=(ComponentRegistry&&) noexcept = default;

	void ComponentRegistry::CreateRegistry(uint32_t registryId)
	{
		Assert::That(!m_Impl->TryGet(registryId), "Registry {} already exists.", registryId);

		m_Impl->Registries.emplace(registryId, std::make_unique<entt::registry>());
	}

	void ComponentRegistry::DestroyRegistry(uint32_t registryId)
	{
		auto it = m_Impl->Registries.find(registryId);

		if (it == m_Impl->Registries.end())
		{
			Assert::That(false, "Registry {} does not exist.", registryId);
			return;
		}

		m_Impl->Registries.erase(it);
	}

	bool ComponentRegistry::HasRegistry(uint32_t registryId) const
	{
		return m_Impl->Registries.contains(registryId);
	}

	uint32_t ComponentRegistry::CreateEntity(uint32_t registryId)
	{
		auto* registry = m_Impl->TryGet(registryId);

		if (!registry)
		{
			Assert::That(false, "Registry {} does not exist.", registryId);
			return InvalidEntity;
		}

		return static_cast<uint32_t>(registry->create());
	}

	void ComponentRegistry::DestroyEntity(uint32_t registryId, uint32_t entityId)
	{
		auto* registry = m_Impl->TryGet(registryId);

		if (!registry || entityId == InvalidEntity)
		{
			return;
		}

		const auto entity = entt::entity(entityId);

		if (registry->valid(entity))
		{
			registry->destroy(entity);
		}
	}

	entt::registry* ComponentRegistry::Native(uint32_t registryId)
	{
		return m_Impl->TryGet(registryId);
	}
}