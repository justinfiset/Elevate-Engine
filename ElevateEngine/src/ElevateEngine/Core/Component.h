#pragma once

#include <ElevateEngine/Events/Event.h>
#include <ElevateEngine/Editor/Serialization/ComponentLayout.h>

#define COMPONENT_LAYOUT(...) \
	ComponentLayout GetLayout() const override { return ComponentLayout(__VA_ARGS__);}

namespace Elevate
{
	class GameObject;
	class Scene;

	class Component
	{
		friend class GameObject;
		friend class ComponentWrapper;
		friend class Scene;

	public:
		Component() = default;
		~Component() = default;

		inline void SetActive(bool newState) { m_IsActive = newState; }
		inline bool IsActive() { return m_IsActive; }

		GameObject* gameObject;

		// Method to override to define a layout in the editor, not mandatory but higly recommanded
		// If no overrode, an empty layout is generated and nothing is shown in the inspector
		virtual ComponentLayout GetLayout() const { return ComponentLayout("Unknown Component Name", {}); }
	protected:
		virtual void Init() {}
		virtual void Destroy() {}
		virtual void Update() {}
		virtual void Render() {}
		virtual void OnNotify(Event& event) {}
	protected:
		bool m_IsActive = true;

	};
}