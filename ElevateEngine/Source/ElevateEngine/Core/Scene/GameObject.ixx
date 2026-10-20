module;

#include <vector>
#include <set>
#include <memory>
#include <string>
#include <cstdint>
#include <type_traits>
#include <utility>

#include <entt/entt.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <ElevateEngine/Core/Reflection/Reflection.h>

export module Elevate.Core.Scene:GameObject;

import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Assert;
import Elevate.Core.Objects.Object;
import Elevate.Core.Objects.ObjectPtr;
import Elevate.Core.Scene:Component;
import Elevate.Core.Scene:ComponentRegistry;
import Elevate.Core.Transform.ITransformable;
import Elevate.Core.Types.TypeRegistry;

// Forwards
namespace Elevate
{
	class Event;

	namespace Editor
	{
		class EditorLayer;
	}
}

export namespace Elevate
{
	constexpr uint32_t INVALID_ENTITY_ID = ComponentRegistry::InvalidEntity;

	class GameObject;

	class IScene
	{
	public:
		virtual ~IScene() = default;

		virtual void AddRootObject(const EEObjectPtr<GameObject>& obj) = 0;
		virtual void RemoveFromRoot(const EEObjectPtr<GameObject>& obj) = 0;
		virtual uint32_t GetRegistryId() = 0;
	};

	class GameObject : public ITransformable, public EEObject
	{
	public:
		BEGIN_OBJECT(GameObject);
		using Super = EEObject;

		GameObject(
			std::string name,
			std::shared_ptr<IScene> scene,
			std::shared_ptr<GameObject> parent = nullptr);

		~GameObject();

		std::shared_ptr<GameObject> GetShared();
		std::weak_ptr<GameObject> GetWeak();
		std::shared_ptr<const GameObject> GetShared() const;
		std::weak_ptr<const GameObject> GetWeak() const;

		std::vector<Component*> GetComponents();
		std::vector<const Component*> GetComponents() const;

		inline void SetName(std::string newName)
		{
			m_name = newName;
		}

		std::string GetName() const override;
		virtual TypeLayout GetLayout() const override;

		void SetParent(const EEObjectPtr<GameObject>& newParent);
		void Destroy();
		void RemoveChild(const EEObjectPtr<GameObject>& child);

		inline uint32_t GetEntityId() { return m_entityId; }
		inline uint32_t GetObjectId() { return m_goId; }

		inline const bool HasChild() const { return m_childs.empty(); }
		inline std::set<EEObjectPtr<GameObject>> GetChilds() const
		{
			return m_childs;
		}

		static std::shared_ptr<GameObject> Create(
			std::string name,
			std::shared_ptr<IScene> scene,
			std::shared_ptr<GameObject> parent = nullptr);

		IScene* GetScene() { return m_scene; }

		glm::mat4 GenGlobalMatrix() const;
		void SetFromGlobalMatrix(const glm::mat4& newWorld);
		glm::vec3 GetGlobalPosition();

		// Component operations: definitions in GameObjectImplf

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args);

		template<typename T>
		T* GetComponent(bool onlyReturnActive = false);

		template<typename T>
		const T* GetComponent(bool onlyReturnActive = false) const;

		template<typename T>
		bool HasComponent();

		template<typename T>
		void RemoveComponent();

	protected:
		void Awake();
		void Start();
		void Update();
		void Render();
		void Notify(Event& event);

		// ITransformable
		void OnSetPosition() override;
		void OnSetRotation() override;
		void OnSetScale() override;

		// Editor Rendering
		void RenderInEditor();
		void RenderWhenSelected();

		// Parent management
		void AddChild(const EEObjectPtr<GameObject>& child);

	private:
		void Initialize();

		ComponentRegistry& Registry() const;

	private:
		std::string m_name;
		PROPERTY(m_name);

		EEObjectPtr<GameObject> m_parent;
		std::set<EEObjectPtr<GameObject>> m_childs;

		uint32_t m_entityId = INVALID_ENTITY_ID;
		uint32_t m_goId = INVALID_ENTITY_ID;

		static uint32_t s_goIdCount;
		bool m_isInitialized = false;

		IScene* m_scene;

		END_OBJECT_CUSTOM();

		friend class Scene;
		friend class TypeRegistry;
		friend class Editor::EditorLayer;
	};
}

namespace Elevate
{
	template <typename T, typename... Args>
	T& GameObject::AddComponent(Args&&... args)
	{
		static_assert(std::is_base_of_v<Component, T>, "AddComponent: T must derive from Component.");

		auto& registry = Registry();
		const uint32_t registryId = m_scene->GetRegistryId();

		if (T* existing = registry.TryGet<T>(registryId, m_entityId))
		{
			CoreLogger::Error("Error: Tried to add an already existing component to the {} GameObject", m_name);
			return *existing;
		}

		T& comp = registry.Add<T>(registryId, m_entityId, std::forward<Args>(args)...);

		comp.gameObject = this;
		comp.Init();

		return comp;
	}

	template <typename T>
	T* GameObject::GetComponent(bool onlyReturnActive)
	{
		static_assert(std::is_base_of_v<Component, T>, "GetComponent: T must derive from Component.");

		T* component = Registry().TryGet<T>(m_scene->GetRegistryId(), m_entityId);

		if (onlyReturnActive && component && !component->IsActive())
			return nullptr;

		return component;
	}

	template <typename T>
	const T* GameObject::GetComponent(bool onlyReturnActive) const
	{
		static_assert(std::is_base_of_v<Component, T>, "GetComponent: T must derive from Component.");

		const T* component = Registry().TryGet<T>(m_scene->GetRegistryId(), m_entityId);

		if (onlyReturnActive && component && !component->IsActive())
			return nullptr;

		return component;
	}

	template <typename T>
	bool GameObject::HasComponent()
	{
		static_assert(std::is_base_of_v<Component, T>, "HasComponent: T must derive from Component.");

		return Registry().Has<T>(m_scene->GetRegistryId(), m_entityId);
	}

	template <typename T>
	void GameObject::RemoveComponent()
	{
		static_assert(std::is_base_of_v<Component, T>, "RemoveComponent: T must derive from Component.");

		auto& registry = Registry();
		const uint32_t registryId = m_scene->GetRegistryId();

		T* component = registry.TryGet<T>(registryId, m_entityId);

		if (!component)
		{
			CoreLogger::Error(
				"Trying to remove a missing component. "
				"You need to add the component before removing it.");
			return;
		}

		component->Destroy();
		registry.Remove<T>(registryId, m_entityId);
	}
}
