#pragma once

#include <ElevateEngine/Events/Event.h>
#include <ElevateEngine/Editor/Serialization/ComponentLayout.h>
#include <ElevateEngine/Core/EEObject.h>

#define COMPONENT_LAYOUT(...) \
	ComponentLayout GetLayout() const override { return ComponentLayout(GetName(), __VA_ARGS__);}

namespace Elevate {
	class GameObject;
	class Scene;
}

namespace Elevate
{
	class Component : public EEObject
	{
		friend class GameObject;
		friend class Scene;

	public:
		std::function<bool()> RemoveFromGOFunc;

		Component() = default;
		virtual ~Component() = default;

		inline void SetActive(bool newState) { m_IsActive = newState; }
		inline bool IsActive() { return m_IsActive; }

		GameObject* gameObject = nullptr;

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

		virtual void OnNotify(Event& event) {}
	protected:
		bool m_IsActive = true;
	};
}