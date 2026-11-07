#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

namespace Elevate
{
	class Collider;
	class BoxCollider;
	class CapsuleCollider;
	class SphereCollider;
	class PlaneCollider;
}

namespace Elevate::Jolt
{
	class JoltShapeFactory
	{
	public:
		static JPH::ShapeRefC Create(const Collider* collider);

	private:
		static bool ValidateShapeResult(const JPH::ShapeSettings::ShapeResult& result);
		static JPH::ShapeRefC CreateBoxShape(const BoxCollider& collider);
		static JPH::ShapeRefC CreateCapsuleShape(const CapsuleCollider& collider);
		static JPH::ShapeRefC CreateSphereShape(const SphereCollider& collider);
		static JPH::ShapeRefC CreatePlaneShape(const PlaneCollider& collider);
	};
}
