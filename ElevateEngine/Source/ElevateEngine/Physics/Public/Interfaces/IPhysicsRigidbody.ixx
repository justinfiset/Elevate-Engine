module;

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

export module Elevate.Physics.IPhysicsRigidbody;

import Elevate.Physics.IPhysicsCollider;

export namespace Elevate
{
	class IPhysicsRigidbody
	{
	public:
		virtual ~IPhysicsRigidbody() = default;

		virtual bool IsDynamic() const = 0;
		virtual bool IsKinematic() const = 0;
		virtual bool IsStatic() const = 0;

		virtual glm::vec3 GetPosition() const = 0;
		virtual glm::quat GetRotation() const = 0;
		virtual void SetPosition(const glm::vec3& position) = 0;
		virtual void SetRotation(const glm::quat& rotation) = 0;

		virtual const std::vector<const IPhysicsCollider*>& GetPhysicsColliders() const = 0;
	};
}
