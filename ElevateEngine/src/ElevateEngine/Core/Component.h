#pragma once

namespace Elevate
{
	class GameObject;
	class Scene;

	class Component
	{
		friend class GameObject;
		friend class Scene;

	public:
		Component() = default;
		~Component() = default;

	protected:
		virtual void Init() {}
		virtual void Destroy() {}

		virtual void Update() {}
		virtual void Render() {}

	protected:
		GameObject* gameObject;
	};
}