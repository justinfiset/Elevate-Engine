// Keep every textual include before the first module import. Project headers
// that import modules are grouped last; their own textual dependencies are
// included above, so GCC does not reprocess standard/Jolt headers after an
// import. Jolt.h must come first because it defines the platform and compiler
// macros used by every other Jolt header.
#include <Jolt/Jolt.h>

#include <algorithm>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <Jolt/Core/Factory.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/Memory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Physics/Collision/Shape/StaticCompoundShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include <ElevateEngine/Physics/Public/PhysicsBackend.h>

// Import-bearing headers last.
#include "JoltShapeFactory.h"
#include "JoltPhysicsSystem.h"

import Elevate.Foundations.CoreLogger;
import Elevate.Core.Application.GameContext;
import Elevate.Physics.IPhysicsCollider;
import Elevate.Physics.IPhysicsRigidbody;
import Elevate.Physics.PhysicsSystem;

namespace Elevate
{
	void RegisterPhysicsSystemBackend()
	{
		PhysicsSystem::SetFactory([]() -> std::unique_ptr<PhysicsSystem> {
			return std::make_unique<Jolt::JoltPhysicsSystem>();
		});
	}
}

namespace Elevate::Jolt
{
	///////////////////////////////////////////////////////////////////////
	/// JoltPhysicsSystem
	///////////////////////////////////////////////////////////////////////

	// Connect Jolt with out trace function
	static void TraceImpl(const char* inFMT, ...)
	{
		// Format the message
		va_list list;
		va_start(list, inFMT);
		char buffer[1024];
		vsnprintf(buffer, sizeof(buffer), inFMT, list);
		va_end(list);

		CoreLogger::Trace("[Jolt Physics] {}", buffer);
	}

#ifdef JPH_ENABLE_ASSERTS
	// Callback for asserts, connect this to your own assert handler if you have one
	static bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, JPH::uint inLine)
	{
		// Log the error
		CoreLogger::Error("{}:{}: ({}) {}", inFile, inLine, inExpression, (inMessage != nullptr ? inMessage : ""));
		// Breakpoint
		return true;
	};
#endif

	JoltPhysicsSystem::JoltPhysicsSystem()
	{
		s_Instance = this;
	}

	JoltPhysicsSystem::~JoltPhysicsSystem()
	{
		s_Instance = nullptr;
	}

	void JoltPhysicsSystem::Init()
	{
		JPH::RegisterDefaultAllocator();

		// Connect the logger and assertions to our logging system
		JPH::Trace = TraceImpl;

		JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = AssertFailedImpl;);

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

		CoreLogger::Info("Initialized the Jolt Physics engine!");
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
		if (GetGameState() == GameContextState::Runtime)
		{
			constexpr float PHYSICS_TIMESTEP = 1.0f / 30.0f;

			m_PhysicsSystem.Update(
				PHYSICS_TIMESTEP,
				1,
				m_TempAllocator,
				m_JobSystem
			);

			SyncDynamicBodies();
			SyncKinematicBodies();
		}
	}

	JPH::EMotionType GetBodyMotionType(const IPhysicsRigidbody* rigidbody)
	{
		if (rigidbody->IsStatic())
		{
			return JPH::EMotionType::Static;
		}
		return rigidbody->IsKinematic() ? JPH::EMotionType::Kinematic : JPH::EMotionType::Dynamic;
	}

	JPH::ObjectLayer GetObjectLayer(const IPhysicsRigidbody* rigidbody)
	{
		return rigidbody->IsStatic() ? PhysicsLayers::NON_MOVING : PhysicsLayers::MOVING;
	}

	JPH::EActivation GetBodyActivation(const IPhysicsRigidbody* rigidbody)
	{
		return rigidbody->IsStatic() ? JPH::EActivation::DontActivate : JPH::EActivation::Activate;
	}

	void JoltPhysicsSystem::FlushAllBodies()
	{
		while (!m_Bodies.empty())
		{
			RemoveRigidbody(m_Bodies.back().rigidbody);
		}

		m_Bodies.clear();
	}

	void JoltPhysicsSystem::AddRigidbody(IPhysicsRigidbody* rigidbody)
	{
		JPH::StaticCompoundShapeSettings compoundSettings;
		const glm::vec3 position = rigidbody->GetPosition();
		const glm::quat quat = rigidbody->GetRotation();
		const auto& colliders = rigidbody->GetPhysicsColliders();

		if (colliders.empty())
		{
			return;
		}

		for (const IPhysicsCollider* collider : colliders)
		{
			auto shapeResult = JoltShapeFactory::Create(collider);

			if (!shapeResult)
			{
				CoreLogger::Warn("JoltShapeFactory returned a non valid collider.");
				return;
			}

			const auto& center = collider->GetCenter();
			compoundSettings.AddShape(
				JPH::Vec3(center.x, center.y, center.z),
				JPH::Quat::sIdentity(),
				shapeResult
			);
		}

		JPH::ShapeSettings::ShapeResult compoundResult = compoundSettings.Create();

		if (compoundResult.HasError())
		{
			CoreLogger::Error("Failed to create Jolt Physics shape: {}", compoundResult.GetError().c_str());
			return;
		}

		JPH::BodyCreationSettings bodySettings(
			compoundResult.Get(),
			JPH::RVec3(position.x, position.y, position.z),
			JPH::Quat(quat.x, quat.y, quat.z, quat.w),
			GetBodyMotionType(rigidbody),
			GetObjectLayer(rigidbody)
		);

		auto activation = GetBodyActivation(rigidbody);
		auto bodyID = m_BodyInterface->CreateAndAddBody(bodySettings, activation);

		if (bodyID.IsInvalid())
		{
			CoreLogger::Warn("Failed to create Jolt physics body!");
			return;
		}

		JoltPhysicsBody joltBody;
		joltBody.rigidbody = rigidbody;
		joltBody.bodyID = bodyID;
		m_Bodies.push_back(joltBody);
	}

	void JoltPhysicsSystem::RemoveRigidbody(IPhysicsRigidbody* rigidbody)
	{
		auto it = std::find_if(m_Bodies.begin(), m_Bodies.end(), [rigidbody](const auto& body)
			{
				return body.rigidbody == rigidbody;
			});

		if (it == m_Bodies.end())
		{
			return;
		}

		m_BodyInterface->RemoveBody(it->bodyID);
		m_BodyInterface->DestroyBody(it->bodyID);

		m_Bodies.erase(it);
	}

	void JoltPhysicsSystem::RebuildRigidbody(IPhysicsRigidbody* rigidbody)
	{
		RemoveRigidbody(rigidbody);
		AddRigidbody(rigidbody);
	}

	void JoltPhysicsSystem::SyncDynamicBodies()
	{
		for (auto& body : m_Bodies)
		{
			IPhysicsRigidbody* rigidbody = body.rigidbody;

			if (!rigidbody->IsDynamic())
			{
				continue;
			}

			// Fetch the data from jolt
			JPH::Vec3 position;
			JPH::Quat rotation;
			m_BodyInterface->GetPositionAndRotation(body.bodyID, position, rotation);

			// Apply the data to the world object through the physics interface.
			rigidbody->SetPosition(glm::vec3{ position.GetX(), position.GetY(), position.GetZ() });
			rigidbody->SetRotation(glm::quat{ rotation.GetX(), rotation.GetY(), rotation.GetZ(), rotation.GetW() });
		}
	}

	void JoltPhysicsSystem::SyncKinematicBodies()
	{
		// todo implement this.
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