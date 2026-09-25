#pragma once
#include "GameObject.h"

#include <format>

#include <entt/entt.hpp>

#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Core/Assert.h>
#include <ElevateEngine/Scene/Scene.h>
#include <ElevateEngine/Scene/ScenePrivate.h>

namespace Elevate
{
	class Component;
}

#define EE_VALIDATE_COMPONENT_TYPE() EE_ASSERT((std::is_base_of<Component, T>::value), "EE_VALIDATE_COMPONENT_TYPE() %s : Type specifier must be a child of the Component class.", m_name);

// TODO REPLACE ALL EE_LOGS TO EE_CORE_LOGS (they are in the engine so they should use this feature)
namespace Elevate
{
	template<typename T, typename... Args>
	T& GameObject::AddComponent(Args&&... args)
	{
		EE_VALIDATE_COMPONENT_TYPE();

		// We can't add a second component of the same type
		if (GetRegistryMap()[m_scene->m_registryId]->all_of<T>(entt::entity(m_entityId)))
		{
			EE_ERROR("Error: Tried to add an already existing component to the %s GameObject", m_name);
			return GetRegistryMap()[m_scene->m_registryId]->get<T>(entt::entity(m_entityId));
		}
	
		auto& comp = GetRegistryMap()[m_scene->m_registryId]->emplace<T>(entt::entity(m_entityId), std::forward<Args>(args)...);
		comp.gameObject = this;
		comp.Init();

		return comp;
	}

	template <typename T>
	T* GameObject::GetComponent(bool onlyReturnActive)
	{
		EE_VALIDATE_COMPONENT_TYPE();

		EE_INFO("Getting component type: {}", typeid(T).name());

		if (!m_scene)
			return nullptr;

		auto& registryMap = GetRegistryMap();

		auto it = registryMap.find(m_scene->m_registryId);

		if (it == registryMap.end())
			return nullptr;

		if (!it->second)
			return nullptr;

		entt::registry* registry = it->second.get();

		entt::entity entity = entt::entity(m_entityId);

		bool valid = registry->valid(entity);

		T* component = registry->try_get<T>(entity);

		if (!component)
			return nullptr;

		if (onlyReturnActive && !component->IsActive())
			return nullptr;

		return component;
	}

	template <typename T>
	bool GameObject::HasComponent()
	{
		EE_VALIDATE_COMPONENT_TYPE();

		return GetRegistryMap()[m_scene->m_registryId]->all_of<T>(entt::entity(m_entityId));
	}

	template <typename T>
	void GameObject::RemoveComponent()
	{
		EE_VALIDATE_COMPONENT_TYPE();

		if (HasComponent<T>())
		{
			GetComponent<T>()->Destroy();
			GetRegistryMap()[m_scene->m_registryId]->remove<T>(entt::entity(m_entityId));
		}
		else EE_ERROR("Trying to remove a missing component. You need to add the component before removing it.");
	}
}