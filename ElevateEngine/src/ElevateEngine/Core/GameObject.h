#pragma once

#include "ITransformable.h"
#include <vector>
#include <entt/entt.hpp>
#include <ElevateEngine/Core/Component.h>
#include <ElevateEngine/Scene/Scene.h>

#define EE_VALIDATE_COMPONENT_TYPE() EE_ASSERT((std::is_base_of<Component, T>::value), "{0} : Type specifier must be a child of the Component class.", m_Name);

namespace Elevate {
	class Scene;
	class Component;
}

namespace Elevate
{
	class GameObject : public ITransformable, public std::enable_shared_from_this<GameObject>
	{
	public:
		GameObject(std::string name, std::shared_ptr<Scene> scene, std::shared_ptr<GameObject> parent = nullptr);
		~GameObject();

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			EE_VALIDATE_COMPONENT_TYPE();

			// We can't add a second component of the same type
			if (m_scene->m_Registry.all_of<T>(m_Entity))
			{
				EE_ERROR("Error: Tried to add an already existing component to the {} GameObject", m_Name);
				return m_scene->m_Registry.get<T>(m_Entity);
			}

			auto& comp = m_scene->m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);
			comp.gameObject = this;
			comp.Init();

			return comp;
		}

		template <typename T>
		T* GetComponent()
		{
			EE_VALIDATE_COMPONENT_TYPE();

			return m_scene->m_Registry.try_get<T>(m_Entity);
		}

		std::vector<Component*> GetComponents() const;
			
		template <typename T>
		bool HasComponent()
		{
			EE_VALIDATE_COMPONENT_TYPE();

			return m_scene->m_Registry.all_of<T>(m_Entity);
		}

		template <typename T>
		void RemoveComponent()
		{
			EE_VALIDATE_COMPONENT_TYPE();

			if (HasComponent<T>()) m_scene->m_Registry.remove<T>(m_Entity);
			else EE_ERROR("Trying to remove a missing component. You need to add the component before removing it.");
		}

		inline std::string& GetName() { return m_Name; }
		inline void SetName(std::string newName) { m_Name = newName; }
		
		void SetParent(std::shared_ptr<GameObject> newParent);
		void Destroy();

		void RemoveChild(std::shared_ptr<GameObject> child);

		inline const bool HasChild() const { return m_Childs.size() > 0; }
		inline std::set<std::shared_ptr<GameObject>> GetChilds() const { return m_Childs; }

		static std::shared_ptr<GameObject> Create(std::string name, std::shared_ptr<Scene> scene, std::shared_ptr<GameObject> parent = nullptr);

		Scene* GetScene() { return m_scene; }

		glm::mat4 GenGlobalMatrix() const;
		void SetFromGlobalMatrix(const glm::mat4& newWorld);
		glm::vec3 GetGlobalPosition();

	protected:
		void Update();
		void PreRender();
		void Render();
		void RenderInEditor();
		void Notify(Event& event);

		// This method is protected as the main entry point to modify the parent should be SetParent()
		void AddChild(std::shared_ptr<GameObject> child);
	private:
		void Initialize(); // Internal function to use just after constructor

		// TODO MAYBE REMOVE AND FIND A BETTER STRUCTURE
		// Transforms callbakcs
		virtual void OnSetPosition() override;
	private:
		std::string m_Name;

		// Parent and Child
		std::shared_ptr<GameObject> m_Parent;
		std::set<std::shared_ptr<GameObject>> m_Childs;

		entt::entity m_Entity { entt::null };
		Scene* m_scene;
		friend class Scene;
	};
}