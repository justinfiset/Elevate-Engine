#pragma once

#include "ITransformable.h"
#include <vector>
#include <entt/entt.hpp>
#include "ElevateEngine/Scene/Scene.h"
#include <ElevateEngine/Core/Component.h>
#include "ElevateEngine/Core/ComponentWrapper.h"

#define EE_VALIDATE_COMPONENT_TYPE() EE_CORE_ASSERT((std::is_base_of<Component, T>::value), "{0} : Type specifier must be a child of the Component class.", m_Name);

namespace Elevate
{

	class GameObject : public ITransformable, public std::enable_shared_from_this<GameObject>
	{
	public:
		GameObject(std::string name, ScenePtr scene, GameObjectPtr parent = nullptr);
		~GameObject();

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			EE_VALIDATE_COMPONENT_TYPE();

		    ComponentWrapper& emplacedWrapper = m_Scene->m_Registry.emplace<ComponentWrapper>(m_Entity);
			emplacedWrapper.SetComponent<T>(std::forward<Args>(args)...);
			emplacedWrapper.SetGameObject(this);
			emplacedWrapper.Init();

			T& component = static_cast<T&>(*emplacedWrapper.component.get());
			return component;
		}

		template <typename T>
		T* GetComponent()
		{
			EE_VALIDATE_COMPONENT_TYPE();

			T* foundComponent = nullptr;
			m_Scene->m_Registry.view<ComponentWrapper>().each([&foundComponent](auto& wrapper)
			{
				if (wrapper.component && typeid(*wrapper.component) == typeid(T))
				{
					foundComponent = static_cast<T*>(wrapper.component.get());
				}
			});

			if (!foundComponent)
			{
				EE_CORE_TRACE("{} : Trying to get a missing component. You need to add the component before retrieving it.", m_Name);
			}

			return foundComponent;
		}

		std::vector<std::weak_ptr<Component>> GetComponents() const
		{
			std::vector<std::weak_ptr<Component>> components;

			if (m_Scene && m_Scene->m_Registry.valid(m_Entity))
			{
				auto view = m_Scene->m_Registry.view<ComponentWrapper>();
				if (view.contains(m_Entity))
				{
					const auto& wrapper = view.get<ComponentWrapper>(m_Entity);
					if (wrapper.component)
					{
						components.push_back(wrapper.component);
					}
				}
			}

			return components;
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
		
		void SetParent(GameObjectPtr newParent);
		void Destroy();

		void RemoveChild(GameObjectPtr child);

		inline const bool HasChild() const { return m_Childs.size() > 0; }
		inline std::set<GameObjectPtr> GetChilds() const { return m_Childs; }

		static GameObjectPtr Create(std::string name, ScenePtr scene, GameObjectPtr parent = nullptr);

		Scene* GetScene() { return m_Scene; }

		glm::mat4 GenGlobalMatrix() const;
		void SetFromGlobalMatrix(const glm::mat4& newWorld);
		glm::vec3 GetGlobalPosition();

	protected:
		// This method is protected as the main entry point to modify the parent should be SetParent()
		void AddChild(GameObjectPtr child);
	private:
		void Initialize(); // Internal function to use just after constructor

		// TODO MAYBE REMOVE AND FIND A BETTER STRUCTURE
		// Transforms callbakcs
		virtual void OnSetPosition() override;
	private:
		std::string m_Name;

		// Parent and Child
		GameObjectPtr m_Parent;
		std::set<GameObjectPtr> m_Childs;

		entt::entity m_Entity { entt::null };
		Scene* m_Scene;

		friend class Scene;
	};
}