#include "JoltShapeFactory.h"

#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/PlaneShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>

#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Physics/Colliders/Collider.h>
#include <ElevateEngine/Physics/Colliders/BoxCollider.h>
#include <ElevateEngine/Physics/Colliders/CapsuleCollider.h>
#include <ElevateEngine/Physics/Colliders/SphereCollider.h>
#include <ElevateEngine/Physics/Colliders/PlaneCollider.h>

namespace Elevate::Jolt
{
	constexpr float PLANE_COLLIDER_THICKNESS = 0.001f;

	JPH::ShapeRefC JoltShapeFactory::Create(const Collider* collider)
	{
		if (!collider)
		{
			EE_CORE_WARN("Cannot create a shape out of a null collider.");
			return nullptr;
		}

		auto typeId = collider->GetTypeIndex();
		if (typeId == typeid(BoxCollider))
		{
			return CreateBoxShape(static_cast<const BoxCollider&>(*collider));
		}

		if (typeId == typeid(CapsuleCollider))
		{
			return CreateCapsuleShape(static_cast<const CapsuleCollider&>(*collider));
		}

		if (typeId == typeid(SphereCollider))
		{
			return CreateSphereShape(static_cast<const SphereCollider&>(*collider));
		}

		if (typeId == typeid(PlaneCollider))
		{
			return CreatePlaneShape(static_cast<const PlaneCollider&>(*collider));
		}
			
		EE_CORE_WARN("Unsupported collider type.");
		return nullptr;
	}

	bool JoltShapeFactory::ValidateShapeResult(const JPH::ShapeSettings::ShapeResult& shapeResult)
	{
		if (shapeResult.HasError())
		{
			EE_CORE_ERROR("Failed to create Jolt Physics shape: {}", shapeResult.GetError().c_str());
			return false;
		}
		return true;
	}

	JPH::ShapeRefC JoltShapeFactory::CreateBoxShape(const BoxCollider& collider)
	{
		const auto& shape = collider.GetShape();
		const auto& extents = shape.GetHalfExtents();
		JPH::BoxShapeSettings shapeSettings(JPH::Vec3(extents.x, extents.y, extents.z));
		JPH::ShapeSettings::ShapeResult shapeResult = shapeSettings.Create();
		ValidateShapeResult(shapeResult);
		return shapeResult.Get();
	}

	JPH::ShapeRefC JoltShapeFactory::CreateCapsuleShape(const CapsuleCollider& collider)
	{
		const auto& shape = collider.GetShape();
		JPH::CapsuleShapeSettings shapeSettings(shape.GetHalfHeight(), shape.GetRadius());
		JPH::ShapeSettings::ShapeResult shapeResult = shapeSettings.Create();
		ValidateShapeResult(shapeResult);
		return shapeResult.Get();
	}

	JPH::ShapeRefC JoltShapeFactory::CreateSphereShape(const SphereCollider& collider)
	{
		const auto& shape = collider.GetShape();
		JPH::SphereShapeSettings shapeSettings(shape.GetRadius());
		JPH::ShapeSettings::ShapeResult shapeResult = shapeSettings.Create();
		ValidateShapeResult(shapeResult);
		return shapeResult.Get();
	}

	JPH::ShapeRefC JoltShapeFactory::CreatePlaneShape(const PlaneCollider& collider)
	{
		const auto& shape = collider.GetShape();
		const auto& size = shape.GetSize();
		JPH::BoxShapeSettings shapeSettings(JPH::Vec3(size.x * 0.5f, PLANE_COLLIDER_THICKNESS, size.y * 0.5f));
		JPH::ShapeSettings::ShapeResult shapeResult = shapeSettings.Create();
		ValidateShapeResult(shapeResult);
		return shapeResult.Get();
	}
}
