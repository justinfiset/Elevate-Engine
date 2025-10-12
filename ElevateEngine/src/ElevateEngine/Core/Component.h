#pragma once

#include <ElevateEngine/Events/Event.h>
#include <ElevateEngine/Editor/Serialization/ComponentLayout.h>
#include <ElevateEngine/Core/EEObject.h>
#include <typeindex>

#ifdef EE_ENGINE_BUILD
#include <ElevateEngine/Renderer/Texture/Texture.h>
#endif

#define COMPONENT_LAYOUT(...) \
	ComponentLayout GetLayout() const override { return ComponentLayout(GetName(), __VA_ARGS__);}

namespace Elevate
{
	class GameObject;
	class Scene;

	using GameObjectComponentGetter = std::function<Component* (std::weak_ptr<GameObject>)>;
	using GameObjectComponentFactory = std::function<Component* (std::weak_ptr<GameObject>)>;
	using GameObjectComponentDestructor = std::function<void(std::weak_ptr<GameObject>)>;

	class Component : public EEObject
	{
		friend class GameObject;
		friend class Scene;

	public:
		std::function<bool()> RemoveFromGOFunc;

		Component() = default;
		virtual ~Component() = default;

		// Reflection methods
		virtual Component* Clone() = 0;
		virtual void CopyFrom(Component* other) = 0;
		virtual GameObjectComponentFactory GetFactory() const = 0;
		virtual GameObjectComponentDestructor GetDestructor() const = 0;
		virtual const void* GetEditorIconHandle() const { return nullptr; }
		virtual std::type_index GetTypeIndex() const = 0;

		inline void SetActive(bool newState) { m_IsActive = newState; }
		inline bool IsActive() { return m_IsActive; }

		// Method to override to define a layout in the editor, not mandatory but higly recommanded
		// If no overrode, an empty layout is generated and nothing is shown in the inspector
		virtual ComponentLayout GetLayout() const { return ComponentLayout(GetName(), {}); }

		virtual bool RemoveFromGameObject() { return false; }

		inline virtual std::string GetName() const {
			return "Unknown Component Name";
			//return ComponentRegistry::GetName(typeid(*this));
		}
	protected:
		virtual void Init() {}
		virtual void Destroy() {}
		virtual void Update() {}

		virtual void PreRender() {}
		virtual void Render() {}
		virtual void RenderInEditor() {} // Function that is only called if we are in the editor
		virtual void RenderWhenSelected() {}

		virtual void OnNotify(Event& event) {}

	public:
		GameObject* gameObject = nullptr;
	protected:
		bool m_IsActive = true;
	};
}