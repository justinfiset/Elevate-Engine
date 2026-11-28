#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

import Elevate.Physics.IPhysicsCollider;
import Elevate.Physics.BoxShape;
import Elevate.Physics.CapsuleShape;
import Elevate.Physics.SphereShape;
import Elevate.Physics.PlaneShape;

namespace Elevate::Jolt
{
	class JoltShapeFactory
	{
	public:
		static JPH::ShapeRefC Create(const IPhysicsCollider* collider);

	private:
		static bool ValidateShapeResult(const JPH::ShapeSettings::ShapeResult& result);
		static JPH::ShapeRefC CreateBoxShape(const BoxShape& shape);
		static JPH::ShapeRefC CreateCapsuleShape(const CapsuleShape& shape);
		static JPH::ShapeRefC CreateSphereShape(const SphereShape& shape);
		static JPH::ShapeRefC CreatePlaneShape(const PlaneShape& shape);
	};
}
