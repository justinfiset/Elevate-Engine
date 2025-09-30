#pragma once
#include "GameObject.h"

#include <entt/entt.hpp>

#include <ElevateEngine/Core/Component.h>
#include <ElevateEngine/Scene/Scene.h>

#define EE_VALIDATE_COMPONENT_TYPE() EE_ASSERT((std::is_base_of<Component, T>::value), "{0} : Type specifier must be a child of the Component class.", m_name);

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
			EE_ERROR("Error: Tried to add an already existing component to the {} GameObject", m_name);
			return GetRegistryMap()[m_scene->m_registryId]->get<T>(entt::entity(m_entityId));
		}

		auto& comp = GetRegistryMap()[m_scene->m_registryId]->emplace<T>(entt::entity(m_entityId), std::forward<Args>(args)...);
		comp.gameObject = this;
		comp.Init();

		return comp;
	}

	template <typename T>
	T* GameObject::GetComponent()
	{
		EE_VALIDATE_COMPONENT_TYPE();

		return GetRegistryMap()[m_scene->m_registryId]->try_get<T>(entt::entity(m_entityId));
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

		if (HasComponent<T>()) GetRegistryMap()[m_scene->m_registryId]->remove<T>(entt::entity(m_entityId));
		else EE_ERROR("Trying to remove a missing component. You need to add the component before removing it.");
	}
}