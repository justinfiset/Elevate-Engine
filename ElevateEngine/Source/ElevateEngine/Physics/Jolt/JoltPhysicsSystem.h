#pragma once

#include <cstdint>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>

#include <ElevateEngine/Physics/PhysicsSystem.h>

namespace Elevate::Jolt
{
	// Small struct to link ElevateEngine's Rigidbody with jols's system
	struct JoltPhysicsBody
	{
		const Rigidbody* Rigidbody = nullptr;
		JPH::BodyID BodyID;
	};

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
		JoltPhysicsSystem();
		~JoltPhysicsSystem();

		void Init() override;
		void Shutdown() override;
		void Update(float deltaTime) override;

		virtual void AddRigidbody(const Rigidbody* rigidbody) override;
		virtual void RemoveRigidbody(const Rigidbody* rigidbody) override;
		virtual void RebuildRigidbody(const Rigidbody* rigidbody) override;

	private:
		/// <summary>
		/// Syncs all of the rigidbodies positions to the positions calculated by
		/// the Jolt physics system.
		/// </summary>
		void SyncDynamicBodies();

		void SyncKinematicBodies();

		std::vector<JoltPhysicsBody> m_Bodies;

		JPH::PhysicsSystem m_PhysicsSystem;

		JoltBroadPhaseLayer m_BroadPhaseLayer;
		JoltObjectLayerPairFilter m_ObjectLayerPairFilter;
		JoltObjectVsBroadPhaseLayerFilter m_ObjectVsBroadPhaseLayerFilter;

		JPH::BodyInterface* m_BodyInterface = nullptr;

		JPH::TempAllocatorImpl* m_TempAllocator = nullptr;
		JPH::JobSystem* m_JobSystem = nullptr;
	
		JPH::BodyID m_TestCube;
		JPH::BodyID m_TestFloor;
	};
}
