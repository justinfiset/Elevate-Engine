#include "JoltPhysicsSystem.h"

#include <Jolt/Core/Factory.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/Memory.h>
#include <Jolt/RegisterTypes.h>

// todo remove once not used in the debug code
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include <ElevateEngine/Core/Log.h>

namespace Elevate::Jolt
{
	///////////////////////////////////////////////////////////////////////
	/// JoltPhysicsSystem
	///////////////////////////////////////////////////////////////////////

	void JoltPhysicsSystem::Init()
	{
		JPH::RegisterDefaultAllocator();

		JPH::Factory::sInstance = new JPH::Factory();
		JPH::RegisterTypes();

		constexpr JPH::uint maxBodies = 10240;
		constexpr JPH::uint numBodyMutexes = 0;
		constexpr JPH::uint maxBodyPairs = 65536;
		constexpr JPH::uint maxContactConstraints = 10240;

		m_TempAllocator = new JPH::TempAllocatorImpl(
			10 * 1024 * 1024
		);

		m_JobSystem = new JPH::JobSystemThreadPool(
			JPH::cMaxPhysicsJobs,
			JPH::cMaxPhysicsBarriers,
			-1
		);

		m_PhysicsSystem.Init(
			maxBodies,
			numBodyMutexes,
			maxBodyPairs,
			maxContactConstraints,
			m_BroadPhaseLayer,
			m_ObjectVsBroadPhaseLayerFilter,
			m_ObjectLayerPairFilter
		);

		m_BodyInterface = &m_PhysicsSystem.GetBodyInterface();

		/* The folloowing code is for debug purposes do not commit this */
		// todo remove this debug code to test the impl.
		JPH::BoxShapeSettings shapeSettings(
			JPH::Vec3(1.0f, 1.0f, 1.0f)
		);

		JPH::ShapeSettings::ShapeResult shapeResult = shapeSettings.Create();

		if (shapeResult.HasError())
		{
			EE_CORE_ERROR("Failed to create Jolt Physics shape: {}", shapeResult.GetError().c_str());
			return;
		}

		JPH::ShapeRefC cubeShape = shapeResult.Get();

		JPH::BodyCreationSettings cubeSettings(
			cubeShape,
			JPH::RVec3(0.0, 5.0, 0.0),
			JPH::Quat::sIdentity(),
			JPH::EMotionType::Dynamic,
			PhysicsLayers::MOVING
		);

		JPH::BoxShapeSettings floorShapeSettings(
			JPH::Vec3(10.0f, 0.5f, 10.0f)
		);

		JPH::ShapeSettings::ShapeResult floorShapeResult = floorShapeSettings.Create();

		if (floorShapeResult.HasError())
		{
			EE_CORE_ERROR("Failed to create Jolt Physics shape: {}", floorShapeResult.GetError().c_str());
			return;
		}

		JPH::ShapeRefC floorShape = floorShapeResult.Get();

		JPH::BodyCreationSettings floorSettings(
			floorShape,
			JPH::RVec3(0.0, -0.5, 0.0),
			JPH::Quat::sIdentity(),
			JPH::EMotionType::Static,
			PhysicsLayers::NON_MOVING
		);

		m_TestCube = m_BodyInterface->CreateAndAddBody(
			cubeSettings,
			JPH::EActivation::Activate
		);

		m_TestFloor = m_BodyInterface->CreateAndAddBody(
			floorSettings,
			JPH::EActivation::DontActivate
		);

		if (m_TestCube.IsInvalid())
		{
			EE_CORE_ERROR("Failed to create Jolt cube body!");
			return;
		}

		if (m_TestFloor.IsInvalid())
		{
			EE_CORE_ERROR("Failed to create Jolt floor body!");
			return;
		}

		EE_CORE_INFO("Initialized the Jolt Physics engine!");
	}

	void JoltPhysicsSystem::Shutdown()
	{
		delete m_JobSystem;
		m_JobSystem = nullptr;

		delete m_TempAllocator;
		m_TempAllocator = nullptr;

		JPH::UnregisterTypes();

		delete JPH::Factory::sInstance;
		JPH::Factory::sInstance = nullptr;
	}

	void JoltPhysicsSystem::Update(float deltaTime)
	{
		constexpr float PHYSICS_TIMESTEP = 1.0f / 30.0f;

		m_PhysicsSystem.Update(
			PHYSICS_TIMESTEP,
			1,
			m_TempAllocator,
			m_JobSystem
		);
	}

	///////////////////////////////////////////////////////////////////////
	/// JoltBroadPhaseLayer
	///////////////////////////////////////////////////////////////////////

	JPH::uint JoltBroadPhaseLayer::GetNumBroadPhaseLayers() const
	{
		return BroadPhaseLayers::NUM_LAYERS;
	}

	JPH::BroadPhaseLayer JoltBroadPhaseLayer::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const
	{
		switch (inLayer)
		{
		case PhysicsLayers::NON_MOVING:
			return BroadPhaseLayers::NON_MOVING;

		case PhysicsLayers::MOVING:
			return BroadPhaseLayers::MOVING;

		default:
			JPH_ASSERT(false);
			return BroadPhaseLayers::NON_MOVING;
		}
	}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	const char* JoltBroadPhaseLayer::GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const
	{
		switch (inLayer.GetValue())
		{
		case BroadPhaseLayers::NON_MOVING.GetValue():
			return "NON_MOVING";

		case BroadPhaseLayers::MOVING.GetValue():
			return "MOVING";

		default:
			return "UNKNOWN";
		}
	}
#endif

	///////////////////////////////////////////////////////////////////////
	/// JoltObjectLayerPairFilter
	///////////////////////////////////////////////////////////////////////

	bool JoltObjectLayerPairFilter::ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const
	{
		switch (inObject1)
		{
		case PhysicsLayers::NON_MOVING:
			return inObject2 == PhysicsLayers::MOVING;

		case PhysicsLayers::MOVING:
			return
				inObject2 == PhysicsLayers::NON_MOVING ||
				inObject2 == PhysicsLayers::MOVING;

		default:
			JPH_ASSERT(false);
			return false;
		}
	}

	///////////////////////////////////////////////////////////////////////
	/// JoltObjectVsBroadPhaseLayerFilter
	///////////////////////////////////////////////////////////////////////

	bool JoltObjectVsBroadPhaseLayerFilter::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const
	{
		switch (inLayer1)
		{
		case PhysicsLayers::NON_MOVING:
			return inLayer2 == BroadPhaseLayers::MOVING;

		case PhysicsLayers::MOVING:
			return
				inLayer2 == BroadPhaseLayers::NON_MOVING ||
				inLayer2 == BroadPhaseLayers::MOVING;

		default:
			JPH_ASSERT(false);
			return false;
		}
	}
}