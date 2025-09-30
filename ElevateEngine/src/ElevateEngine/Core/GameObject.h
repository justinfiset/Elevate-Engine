#pragma once

#include "ITransformable.h"
#include <vector>
#include <set>

#include <memory>
#include <string>

namespace Elevate
{
	class Event;
	class Scene;
	class Component;

	class GameObject : public ITransformable, public std::enable_shared_from_this<GameObject>
	{
	public:
		GameObject(std::string name, std::shared_ptr<Scene> scene, std::shared_ptr<GameObject> parent = nullptr);
		~GameObject();

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args);

		template <typename T>
		T* GetComponent();

		std::vector<Component*> GetComponents() const;
			
		template <typename T>
		bool HasComponent();

		template <typename T>
		void RemoveComponent();

		inline std::string& GetName() { return m_name; }
		inline void SetName(std::string newName) { m_name = newName; }
		
		void SetParent(std::shared_ptr<GameObject> newParent);
		void Destroy();

		void RemoveChild(std::shared_ptr<GameObject> child);

		inline const bool HasChild() const { return m_childs.size() > 0; }
		inline std::set<std::shared_ptr<GameObject>> GetChilds() const { return m_childs; }

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
		std::string m_name;

		// Parent and Child
		std::shared_ptr<GameObject> m_parent;
		std::set<std::shared_ptr<GameObject>> m_childs;

		std::uint32_t m_entityId;

		Scene* m_scene;
		friend class Scene;
	};
}

#include "GameObject.inl"