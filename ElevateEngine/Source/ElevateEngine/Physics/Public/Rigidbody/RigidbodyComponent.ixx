module;

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "Reflection.h"

export module Elevate.Physics.Rigidbody:Component;

import :Types;
import Elevate.Core.Scene;
import Elevate.Physics.IPhysicsCollider;
import Elevate.Physics.IPhysicsRigidbody;

export namespace Elevate
{
	class Rigidbody : public Component, public IPhysicsRigidbody
	{
	public:
		BEGIN_COMPONENT(Rigidbody, EditorIconTag(Editor::Resources::Icons::Light::Components::Rigidbody));
		EECATEGORY("Physics");

		Rigidbody() = default;

		void Awake() override;
		void Destroy() override;

		RigidbodyType GetType() const;

		bool IsDynamic() const override;
		bool IsKinematic() const override;
		bool IsStatic() const override;

		glm::vec3 GetPosition() const override;
		glm::quat GetRotation() const override;
		void SetPosition(const glm::vec3& position) override;
		void SetRotation(const glm::quat& rotation) override;

		const std::vector<const IPhysicsCollider*>& GetPhysicsColliders() const override;
		void AddCollider(const IPhysicsCollider* collider);
		void RemoveCollider(const IPhysicsCollider* collider);

	private:
		RigidbodyData m_data;
		PROPERTY(m_data, FlattenTag());

		std::vector<const IPhysicsCollider*> m_Colliders;

		END_COMPONENT();
	};
}
