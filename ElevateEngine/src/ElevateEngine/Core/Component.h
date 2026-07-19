#pragma once

#include <ElevateEngine/Events/Event.h>
#include <ElevateEngine/Core/EEObject.h>
#include <ElevateEngine/Core/Reflection.h>

#ifdef EE_ENGINE_BUILD
	#include <ElevateEngine/Renderer/Texture/Texture.h>
#endif

#define COMPONENT_LAYOUT(...) \
	TypeLayout GetLayout() const override { return TypeLayout(GetName(), __VA_ARGS__);}

namespace Elevate
{
	class GameObject;
	class Scene;

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

		inline void SetActive(bool newState) { m_IsActive = newState; }
		inline bool IsActive() { return m_IsActive; }

		// EEObject Implementation
		// If no overrode, an empty layout is generated and nothing is shown in the inspector
		inline virtual std::string GetName() const { return "Unknown Component"; }
		inline virtual std::type_index GetTypeIndex() const { return typeid(Component); }

		virtual bool RemoveFromGameObject() { return false; }

	protected:
		virtual void Init() {}
		virtual void Destroy() {}
		virtual void Update() {}

		virtual void Render() {}
		virtual void RenderInEditor() {} // Function that is only called if we are in the editor
		virtual void RenderWhenSelected() {}

		// Transform callbacks
		virtual void OnSetPosition() {}
		virtual void OnSetRotation() {}
		virtual void OnSetScale()    {}

		virtual void OnNotify(Event& event) {}

	public:
		GameObject* gameObject = nullptr;
	protected:
		bool m_IsActive = true;
	};
}