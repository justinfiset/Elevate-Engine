#pragma once

#include "ITransformable.h"
#include <vector>
#include <entt/entt.hpp>
#include "ElevateEngine/Scene/Scene.h"
#include "ElevateEngine/Core/ComponentWrapper.h"
#include "ElevateEngine/Core/Component.h"

#define EE_VALIDATE_COMPONENT_TYPE() EE_CORE_ASSERT((std::is_base_of<Component, T>::value), "{0} : Type specifier must be a child of the Component class.", m_Name);

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
			EE_VALIDATE_COMPONENT_TYPE();

			T& component = m_Scene->m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);

			Component& baseComponent = static_cast<Component&>(component);
			baseComponent.gameObject = this;
			baseComponent.Init();

			return component;

			/*try
			{
				ComponentWrapper& tempWrapper = ComponentWrapper::Create<T>(std::forward<Args>(args)...);

				tempWrapper.SetGameObject(this);
				tempWrapper.Init();
				
				ComponentWrapper& emplacedWrapper = m_Scene->m_Registry.emplace_or_replace<ComponentWrapper>(m_Entity, std::move(tempWrapper));
				T& component = static_cast<T&>(*emplacedWrapper.component);

				EE_CORE_TRACE(m_Scene->GetName());
				return component;
			}
			catch (...) 
			{
				EE_CORE_ERROR("{0} : Caught an unknown excpetion while adding a component.", m_Name);
			}*/
		}

		template <typename T>
		T& GetComponent()
		{
			EE_VALIDATE_COMPONENT_TYPE();

			if(!HasComponent<T>())
				EE_CORE_ERROR("{0} : Entity does not have component!", m_Name);
			return m_Scene->m_Registry.get<T>(m_Entity);
			//// todo reactiver cette ligne
			////EE_CORE_ASSERT(std::is_base_of<Component, T>::value, "T must be a child of the Component class");
			//auto view = m_Scene->m_Registry.view<ComponentWrapper>();
			//EE_CORE_TRACE(view.size());
			//view.each([&](auto& wrapper)
			//{
			//		if (wrapper.IsComponentType<T>())
			//		{
			//			return dynamic_cast<T*>(wrapper.component.get());
			//			//return wrapper.Get<T>();
			//		}
			//});

			//EE_CORE_TRACE("Componenet not found.");
			//std::runtime_error("Trying to get a missing component. You need to add the component before retrieving it.");
		}

		template <typename T>
		bool HasComponent()
		{
			EE_VALIDATE_COMPONENT_TYPE();

			return m_Scene->m_Registry.all_of<T>(m_Entity);
		}

		template <typename T>
		void RemoveComponent()
		{
			EE_VALIDATE_COMPONENT_TYPE();

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