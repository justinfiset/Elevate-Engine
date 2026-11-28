
// Jolt.h must come first because it defines the platform and compiler macros
// used by the other Jolt headers. Every textual include stays ahead of
// JoltShapeFactory.h, which imports a module.
#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/PlaneShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>

// Shape modules expose glm types from their global module fragment, so importers
// that use those accessors need the textual include as well.
#include <glm/glm.hpp>

#include "JoltShapeFactory.h"

import Elevate.Foundations.CoreLogger;
import Elevate.Physics.IPhysicsCollider;

namespace Elevate::Jolt
{
	constexpr float PLANE_COLLIDER_THICKNESS = 0.001f;

	JPH::ShapeRefC JoltShapeFactory::Create(const IPhysicsCollider* collider)
	{
		if (!collider)
		{
			CoreLogger::Warn("Cannot create a shape out of a null collider.");
			return nullptr;
		}

		auto* shape = collider->GetPhysicsShape();
		if (!shape)
		{
			CoreLogger::Warn("Unsupported collider type.");
			return nullptr;
		}

		if (const auto* boxShape = dynamic_cast<const BoxShape*>(shape))
		{
			return CreateBoxShape(*boxShape);
		}

		if (const auto* capsuleShape = dynamic_cast<const CapsuleShape*>(shape))
		{
			return CreateCapsuleShape(*capsuleShape);
		}

		if (const auto* sphereShape = dynamic_cast<const SphereShape*>(shape))
		{
			return CreateSphereShape(*sphereShape);
		}

		if (const auto* planeShape = dynamic_cast<const PlaneShape*>(shape))
		{
			return CreatePlaneShape(*planeShape);
		}

		CoreLogger::Warn("Unsupported collider shape.");
		return nullptr;
	}

	bool JoltShapeFactory::ValidateShapeResult(const JPH::ShapeSettings::ShapeResult& shapeResult)
	{
		if (shapeResult.HasError())
		{
			CoreLogger::Error("Failed to create Jolt Physics shape: {}", shapeResult.GetError().c_str());
			return false;
		}
		return true;
	}

	JPH::ShapeRefC JoltShapeFactory::CreateBoxShape(const BoxShape& shape)
	{
		const auto& extents = shape.GetHalfExtents();
		JPH::BoxShapeSettings shapeSettings(JPH::Vec3(extents.x, extents.y, extents.z));
		JPH::ShapeSettings::ShapeResult shapeResult = shapeSettings.Create();
		ValidateShapeResult(shapeResult);
		return shapeResult.Get();
	}

	JPH::ShapeRefC JoltShapeFactory::CreateCapsuleShape(const CapsuleShape& shape)
	{
		JPH::CapsuleShapeSettings shapeSettings(shape.GetHalfHeight(), shape.GetRadius());
		JPH::ShapeSettings::ShapeResult shapeResult = shapeSettings.Create();
		ValidateShapeResult(shapeResult);
		return shapeResult.Get();
	}

	JPH::ShapeRefC JoltShapeFactory::CreateSphereShape(const SphereShape& shape)
	{
		JPH::SphereShapeSettings shapeSettings(shape.GetRadius());
		JPH::ShapeSettings::ShapeResult shapeResult = shapeSettings.Create();
		ValidateShapeResult(shapeResult);
		return shapeResult.Get();
	}

	JPH::ShapeRefC JoltShapeFactory::CreatePlaneShape(const PlaneShape& shape)
	{
		const auto& size = shape.GetSize();
		JPH::BoxShapeSettings shapeSettings(JPH::Vec3(size.x * 0.5f, PLANE_COLLIDER_THICKNESS, size.y * 0.5f));
		JPH::ShapeSettings::ShapeResult shapeResult = shapeSettings.Create();
		ValidateShapeResult(shapeResult);
		return shapeResult.Get();
	}
}
