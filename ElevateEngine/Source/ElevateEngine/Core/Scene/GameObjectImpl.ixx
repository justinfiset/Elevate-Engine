//module;
//
//#include <entt/entt.hpp>
//#include <type_traits>
//#include <utility>
//
//export module Elevate.Core.Scene.GameObject:Impl;
//
//import Elevate.Core.Scene:GameObject;
//import Elevate.Core.Scene:Scene;
//import Elevate.Core.Scene:ComponentRegistry;
//import Elevate.Core.Scene:Component;
//import Elevate.Foundations.CoreLogger;
//import Elevate.Foundations.Assert;
//
//#define EE_VALIDATE_COMPONENT_TYPE()                                                                \
//	Assert::That(                                                                                   \
//		(std::is_base_of<Component, T>::value),                                                     \
//		"EE_VALIDATE_COMPONENT_TYPE() {} : Type specifier must be a child of the Component class.", \
//		m_name)
//
//namespace Elevate
//{
//	template <typename T, typename... Args>
//	T& GameObject::AddComponent(Args &&...args)
//	{
//		EE_VALIDATE_COMPONENT_TYPE();
//
//		auto* registry = ComponentRegistry::TryGetRegistry(m_scene->GetRegistryId());
//
//		if (HasComponent<T>())
//		{
//			CoreLogger::Error("Error: Tried to add an already existing component to the {} GameObject", m_name);
//
//			return *ComponentRegistry::TryGetFromRegistry<T>(
//				m_scene->GetRegistryId(),
//				entt::entity(m_entityId)
//			);
//		}
//
//		auto& comp = registry->emplace<T>(
//			entt::entity(m_entityId),
//			std::forward<Args>(args)...);
//
//		comp.gameObject = this;
//		comp.Init();
//
//		return comp;
//	}
//
//	template <typename T>
//	T* GameObject::GetComponent(bool onlyReturnActive)
//	{
//		EE_VALIDATE_COMPONENT_TYPE();
//
//		T* component = ComponentRegistry::TryGetFromRegistry<T>(
//			m_scene->GetRegistryId(),
//			entt::entity(m_entityId));
//
//		if (onlyReturnActive && component && !component->IsActive())
//			return nullptr;
//
//		return component;
//	}
//
//	template <typename T>
//	const T* GameObject::GetComponent(bool onlyReturnActive) const
//	{
//		EE_VALIDATE_COMPONENT_TYPE();
//
//		const T* component = ComponentRegistry::TryGetFromRegistry<T>(
//			m_scene->GetRegistryId(),
//			entt::entity(m_entityId));
//
//		if (onlyReturnActive && component && !component->IsActive())
//			return nullptr;
//
//		return component;
//	}
//
//	template <typename T>
//	bool GameObject::HasComponent()
//	{
//		EE_VALIDATE_COMPONENT_TYPE();
//
//		auto* registry =
//			ComponentRegistry::TryGetRegistry(m_scene->GetRegistryId());
//
//		if (registry)
//			return registry->all_of<T>(entt::entity(m_entityId));
//
//		return false;
//	}
//
//	template <typename T>
//	void GameObject::RemoveComponent()
//	{
//		EE_VALIDATE_COMPONENT_TYPE();
//
//		if (HasComponent<T>())
//		{
//			GetComponent<T>()->Destroy();
//
//			auto* registry =
//				ComponentRegistry::TryGetRegistry(m_scene->GetRegistryId());
//
//			registry->remove<T>(entt::entity(m_entityId));
//		}
//		else
//		{
//			CoreLogger::Error(
//				"Trying to remove a missing component. "
//				"You need to add the component before removing it.");
//		}
//	}
//}
