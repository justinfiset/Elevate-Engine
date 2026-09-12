#pragma once

#include <cstdint>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>

#include <ElevateEngine/Physics/PhysicsSystem.h>

namespace Elevate::Jolt
{
	///////////////////////////////////////////////////////////////////////
	/// JoltBroadPhaseLayer
	///////////////////////////////////////////////////////////////////////

	namespace PhysicsLayers
	{
		static constexpr JPH::ObjectLayer NON_MOVING = 0;
		static constexpr JPH::ObjectLayer MOVING = 1;

		static constexpr uint8_t NUM_LAYERS = 2;
	}

	namespace BroadPhaseLayers
	{
		static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
		static constexpr JPH::BroadPhaseLayer MOVING(1);

		static constexpr uint8_t NUM_LAYERS = 2;
	}

	class JoltBroadPhaseLayer final : public JPH::BroadPhaseLayerInterface
	{
	public:
		JPH::uint GetNumBroadPhaseLayers() const override;

		JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
		const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override;
#endif
	};

	///////////////////////////////////////////////////////////////////////
	/// JoltObjectLayerPairFilter
	///////////////////////////////////////////////////////////////////////
	
	class JoltObjectLayerPairFilter final : public JPH::ObjectLayerPairFilter
	{
	public:
		bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override;
	};

	///////////////////////////////////////////////////////////////////////
	/// JoltObjectVsBroadPhaseLayerFilter
	///////////////////////////////////////////////////////////////////////
	 
	class JoltObjectVsBroadPhaseLayerFilter final : public JPH::ObjectVsBroadPhaseLayerFilter
	{
	public:
		bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override;
	};

	///////////////////////////////////////////////////////////////////////
	/// JoltPhysicsSystem
	///////////////////////////////////////////////////////////////////////

	class JoltPhysicsSystem final : public PhysicsSystem
	{
	public:
		void Init() override;
		void Shutdown() override;
		void Update(float deltaTime) override;

	private:
		JPH::PhysicsSystem m_PhysicsSystem;

		JoltBroadPhaseLayer m_BroadPhaseLayer;
		JoltObjectLayerPairFilter m_ObjectLayerPairFilter;
		JoltObjectVsBroadPhaseLayerFilter m_ObjectVsBroadPhaseLayerFilter;

		JPH::TempAllocatorImpl* m_TempAllocator = nullptr;
		JPH::JobSystem* m_JobSystem = nullptr;
	};
}
