module;

#include <glm/glm.hpp>

export module Elevate.Physics.IPhysicsCollider;

import Elevate.Physics.PhysicsShape;

export namespace Elevate
{
	class IPhysicsCollider
	{
	public:
		virtual ~IPhysicsCollider() = default;

		virtual const glm::vec3& GetCenter() const = 0;

		// Colliders without a primitive shape keep the default and are reported
		// as unsupported by the physics backend.
		virtual const PhysicsShape* GetPhysicsShape() const { return nullptr; }
	};
}
