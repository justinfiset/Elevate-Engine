module;

#include <vector>

#include "Reflection.h"

export module Elevate.Physics.Rigidbody:Component;

import :Types;
import Elevate.Core.Scene;

export namespace Elevate
{
	class Rigidbody : public Component
	{
	public:
		BEGIN_COMPONENT(Rigidbody, EditorIconTag(Editor::Resources::Icons::Light::Components::Rigidbody));
		EECATEGORY("Physics");

		Rigidbody() = default;

		void Awake() override;
		void Destroy() override;

		RigidbodyType GetType() const;

		// Store Component instead of Collider so Rigidbody does not import the
		// Collider module; Collider.cpp imports Rigidbody to register itself.
		const std::vector<const Component*>& GetColliders() const;
		void AddCollider(const Component* collider);
		void RemoveCollider(const Component* collider);

	private:
		RigidbodyData m_data;
		PROPERTY(m_data, FlattenTag());

		std::vector<const Component*> m_Colliders;

		END_COMPONENT();
	};
}
