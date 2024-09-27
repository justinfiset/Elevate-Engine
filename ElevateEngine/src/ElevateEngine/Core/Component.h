#pragma once

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
	protected:
		virtual void Init() {}
		virtual void Destroy() {}

		virtual void Update() {}
		virtual void Render() {}

	protected:
		bool m_IsActive = true;

	};
}