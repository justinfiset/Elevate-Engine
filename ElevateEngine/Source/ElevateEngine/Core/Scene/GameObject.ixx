module;

#include <vector>
#include <set>
#include <memory>
#include <string>
#include <stdint.h>

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <entt/entt.hpp>
#include <ElevateEngine/Core/Reflection/Reflection.h>

#define EE_VALIDATE_COMPONENT_TYPE() Assert::That((std::is_base_of<Component, T>::value), "EE_VALIDATE_COMPONENT_TYPE() {} : Type specifier must be a child of the Component class.", m_name);

export module Elevate.Core.Scene.GameObject;

import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Assert;
import Elevate.Core.Objects.Object;
import Elevate.Core.Objects.ObjectPtr;
import Elevate.Core.Scene.Scene;
import Elevate.Core.Scene.ComponentRegistry;
import Elevate.Core.Transform.ITransformable;
import Elevate.Core.Types.TypeRegistry;

// Forwards
namespace Elevate
{
	class Event;
	class Scene;
	class Component;

	namespace Editor
	{
		class EditorLayer;
	}
}

export namespace Elevate
{
	constexpr uint32_t INVALID_ENTITY_ID = UINT32_MAX;

	class GameObject : public ITransformable, public EEObject
	{
	public:
		BEGIN_OBJECT(GameObject);
		using Super = EEObject;

		GameObject(std::string name, std::shared_ptr<Scene> scene, std::shared_ptr<GameObject> parent = nullptr);
		~GameObject();

		std::shared_ptr<GameObject> GetShared();
		std::weak_ptr<GameObject> GetWeak();
		std::shared_ptr<const GameObject> GetShared() const;
		std::weak_ptr<const GameObject> GetWeak() const;

		std::vector<Component*> GetComponents();
		std::vector<const Component*> GetComponents() const;

		inline void SetName(std::string newName) { m_name = newName; }
		std::string GetName() const override;
		virtual TypeLayout GetLayout() const override;

		void SetParent(const EEObjectPtr<GameObject>& newParent);
		void Destroy();

		void RemoveChild(const EEObjectPtr<GameObject>& child);

		inline uint32_t GetEntityId() { return m_entityId; }
		inline uint32_t GetObjectId() { return m_goId; }

		inline const bool HasChild() const { return m_childs.empty(); }
		inline std::set<EEObjectPtr<GameObject>> GetChilds() const { return m_childs; }

		static std::shared_ptr<GameObject> Create(std::string name, std::shared_ptr<Scene> scene, std::shared_ptr<GameObject> parent = nullptr);

		Scene* GetScene() { return m_scene; }

		glm::mat4 GenGlobalMatrix() const;
		void SetFromGlobalMatrix(const glm::mat4& newWorld);
		glm::vec3 GetGlobalPosition();

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			EE_VALIDATE_COMPONENT_TYPE();

			auto* registry = ComponentRegistry::TryGetRegistry(m_scene->GetRegistryId());

			if (HasComponent<T>())
			{
				CoreLogger::Error("Error: Tried to add an already existing component to the {} GameObject", m_name);
				return *ComponentRegistry::TryGetFromRegistry<T>(m_scene->GetRegistryId(), entt::entity(m_entityId));
			}

			auto& comp = registry->emplace<T>(entt::entity(m_entityId), std::forward<Args>(args)...);
			comp.gameObject = this;
			comp.Init();
			return comp;
		}

		template <typename T>
		T* GetComponent(bool onlyReturnActive = false)
		{
			EE_VALIDATE_COMPONENT_TYPE();

			T* component = ComponentRegistry::TryGetFromRegistry<T>(m_scene->GetRegistryId(), entt::entity(m_entityId));

			if (onlyReturnActive && !component->IsActive())
			{
				return nullptr;
			}

			return component;
		}

		template <typename T>
		const T* GetComponent(bool onlyReturnActive = false) const
		{
			EE_VALIDATE_COMPONENT_TYPE();

			T* component = ComponentRegistry::TryGetFromRegistry<T>(m_scene->GetRegistryId(), entt::entity(m_entityId));

			if (onlyReturnActive && !component->IsActive())
			{
				return nullptr;
			}

			return component;
		}

		template <typename T>
		bool HasComponent()
		{
			EE_VALIDATE_COMPONENT_TYPE();

			if (auto* registry = ComponentRegistry::TryGetRegistry(m_scene->GetRegistryId()))
			{
				return registry->all_of<T>(entt::entity(m_entityId));
			}

			return false;
		}

		template <typename T>
		void RemoveComponent()
		{
			EE_VALIDATE_COMPONENT_TYPE();

			if (HasComponent<T>())
			{
				GetComponent<T>()->Destroy();
				auto* registry = ComponentRegistry::TryGetRegistry(m_scene->GetRegistryId());
				registry->remove<T>(entt::entity(m_entityId));
			}
			else
			{
				CoreLogger::Error("Trying to remove a missing component. You need to add the component before removing it.");
			}
		}

	protected:
		void Awake();
		void Start();
		void Update();
		void Render();
		void Notify(Event& event);

		// ITransformable
		void OnSetPosition() override;
		void OnSetRotation() override;
		void OnSetScale()    override;

		// Editor Rendering
		void RenderInEditor();
		void RenderWhenSelected();

		// This method is protected as the main entry point to modify the parent should be SetParent()
		void AddChild(const EEObjectPtr<GameObject>& child);

	private:
		void Initialize(); // Internal function to use just after constructor

	private:
		std::string m_name;
		PROPERTY(m_name);

		// Parent and Child
		EEObjectPtr<GameObject> m_parent;
		std::set<EEObjectPtr<GameObject>> m_childs;

		// The entt id
		uint32_t m_entityId = INVALID_ENTITY_ID; // Invalid up until the initialization
		// The gameObject static count
		uint32_t m_goId = INVALID_ENTITY_ID; // Invalid up until the initialization
		static uint32_t s_goIdCount;
		bool m_isInitialized = false;

		Scene* m_scene;

		END_OBJECT_CUSTOM();

		friend class Scene;
		friend class TypeRegistry;
		friend class Editor::EditorLayer;
	};
}
