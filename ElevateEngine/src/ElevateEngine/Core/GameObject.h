#pragma once

#include "ITransformable.h"
#include <vector>
#include <entt/entt.hpp>
#include "ElevateEngine/Scene/Scene.h"

namespace Elevate
{
	class GameObject : public ITransformable
	{
	public:
		GameObject(std::string name);
		GameObject(std::string name, Scene* scene);
		~GameObject();

		template <typename T>
		T& AddComponent(T component)
		{
			m_Scene->m_Registry.emplace(m_Entity, component);
			return component;
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			T& component = m_Scene->m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);
			//m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template <typename T>
		T& GetComponent()
		{
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