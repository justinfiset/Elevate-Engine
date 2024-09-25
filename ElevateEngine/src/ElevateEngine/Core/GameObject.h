#pragma once

#include "ITransformable.h"
#include <vector>
#include <entt/entt.hpp>
#include "ElevateEngine/Scene/Scene.h"
#include "ElevateEngine/Core/ComponentWrapper.h"
#include "ElevateEngine/Core/Component.h"

namespace Elevate
{
	class GameObject : public ITransformable
	{
	public:
		GameObject(std::string name);
		GameObject(std::string name, Scene* scene);
		~GameObject();

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			// Assurer que T est un enfant de Component
			//EE_CORE_ASSERT(std::is_base_of<Component, T>::value, "{0} : AddComponent() type specifier must be a child of the Component class.", m_Name);
			try
			{
				ComponentWrapper& wrapper = ComponentWrapper::Create<T>(std::forward<Args>(args)...);

				if (!wrapper.component) {
					throw std::runtime_error("Component not initialized!");
				}

				// Conservez une référence ou un pointeur vers le composant
				T& component = static_cast<T&>(*wrapper.component);
				wrapper.component->gameObject = this;
				wrapper.component->Init();

				EE_CORE_TRACE(wrapper.component->gameObject->GetName());
				m_Scene->m_Registry.emplace<ComponentWrapper>(m_Entity, std::move(wrapper));

				// Utiliser std::move sur le composant, pas sur le wrapper

				return component;
			}
			catch (const std::runtime_error& e) 
			{ // Catch specific exception type
				EE_CORE_ERROR("{0} : Runtime Exception - {1}", m_Name, e.what());
			}
			catch (const std::exception& e) 
			{ // Catch any other exception type
				EE_CORE_ERROR("{0} : Exception - {1}", m_Name, e.what());
			}
			catch (...) 
			{
				EE_CORE_ERROR("{0} : Caught an unknown excpetion while adding a component.", m_Name);
			}
		}

		template <typename T>
		T& GetComponent()
		{
			//EE_CORE_ASSERT(std::is_base_of<Component, T>::value, "{0} : GetComponent()	 type specifier must be a child of the Component class.", m_Name);
			if (HasComponent<T>())
			{
				return m_Scene->m_Registry.get<T>(m_Entity);
			}
			else 
			{
				std::runtime_error("Trying to get a missing component. You need to add the component before retrieving it.");
			}
		}

		template <typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_Entity);
		}

		template <typename T>
		void RemoveComponent()
		{
			if (HasComponent<T>()) m_Scene->m_Registry.remove<T>(m_Entity);
			else EE_CORE_ERROR("Trying to remove a missing component. You need to add the component before removing it.");
		}

		inline std::string& GetName() { return m_Name; }
		inline void SetName(std::string newName) { m_Name = newName; }
		
		inline const bool HasChild() const { return m_Childs.size() > 0; }
		inline std::vector<std::shared_ptr<GameObject>> GetChilds() const { return m_Childs; }

		// TODO AddChild
		//void AddChild(std::shared_ptr<GameObject> newObject);
		
		// TODO getter and setters for components
	private:
		std::string m_Name;

		// Parent and Child
		std::shared_ptr<GameObject> m_Parent;
		std::vector<std::shared_ptr<GameObject>> m_Childs;

		entt::entity m_Entity { entt::null };
		Scene* m_Scene;

		friend class Scene;
	};
}