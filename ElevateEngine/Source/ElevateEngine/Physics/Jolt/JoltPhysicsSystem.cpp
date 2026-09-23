#include "JoltPhysicsSystem.h"
#include "JoltShapeFactory.h"

#include <cstdarg>
#include <cstdio>

#include <Jolt/Core/Factory.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/Memory.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Physics/Collision/Shape/StaticCompoundShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Core/Assert.h>
#include <ElevateEngine/Core/Application.h>
#include <ElevateEngine/Physics/Rigidbody.h>
#include <ElevateEngine/Physics/Colliders/Collider.h>

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

		EE_CORE_TRACE("{}", buffer);
	}

	// Connect the Jolt assertion with our assertion
#ifdef JPH_ENABLE_ASSERTS
	// Callback for asserts, connect this to your own assert handler if you have one
	static bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, JPH::uint inLine)
	{
		// Log the error
		EE_CORE_ERROR("{}:{}: ({}) {}", inFile, inLine, inExpression, (inMessage != nullptr ? inMessage : ""));
		// Breakpoint
		return true;
	};
#endif // JPH_ENABLE_ASSERTS

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
		JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = AssertFailedImpl;)

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
		if (Application::GetGameState() == GameContextState::Runtime)
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

	JPH::EMotionType GetBodyMotionType(const Rigidbody* rigidbody)
	{
		switch (rigidbody->GetType())
		{
		case RigidbodyType::Dynamic:
			return JPH::EMotionType::Dynamic;
		case RigidbodyType::Kinematic:
			return JPH::EMotionType::Kinematic;
		case RigidbodyType::Static:
			return JPH::EMotionType::Static;
		default:
			EE_CORE_ERROR("Unknown RigidbodyType.");
			break;
		}
		return JPH::EMotionType::Static;
	}

	JPH::ObjectLayer GetObjectLayer(const Rigidbody* rigidbody)
	{
		switch (rigidbody->GetType())
		{
		case RigidbodyType::Static:
			return PhysicsLayers::NON_MOVING;
		case RigidbodyType::Dynamic:
		case RigidbodyType::Kinematic:
			return PhysicsLayers::MOVING;
		default:
			EE_CORE_ERROR("Unknown RigidbodyType.");
			return PhysicsLayers::NON_MOVING;
		}
	}

	JPH::EActivation GetBodyActivation(const Rigidbody* rigidbody)
	{
		return rigidbody->GetType() == RigidbodyType::Static ? JPH::EActivation::DontActivate : JPH::EActivation::Activate;
	}

	void JoltPhysicsSystem::AddRigidbody(const Rigidbody* rigidbody)
	{
		JPH::StaticCompoundShapeSettings compoundSettings;

		const auto& colliders = rigidbody->GetColliders();
		if (colliders.empty())
		{
			return;
		}

		for (const auto& collider : colliders)
		{
			auto shapeResult = JoltShapeFactory::Create(collider);

			if (!shapeResult)
			{
				EE_CORE_WARN("JoltShapeFactory returned a non valid collider.");
				return;
			}

			const auto& center = collider->GetCenter();
			JPH::Vec3 position(center.x, center.y, center.z);
			JPH::Quat rotation(0.0f, 0.0f, 0.0f, 1.0f);

			compoundSettings.AddShape(
				JPH::Vec3(center.x, center.y, center.z),
				JPH::Quat(0.0f, 0.0f, 0.0f, 1.0f),
				shapeResult
			);
		}

		JPH::ShapeSettings::ShapeResult compoundResult = compoundSettings.Create();

		if (compoundResult.HasError())
		{
			EE_CORE_ERROR("Failed to create Jolt Physics shape: {}", compoundResult.GetError().c_str());
			return;
		}

		JPH::BodyCreationSettings bodySettings(
			compoundResult.Get(),
			JPH::RVec3::sZero(),
			JPH::Quat::sIdentity(),
			GetBodyMotionType(rigidbody),
			GetObjectLayer(rigidbody)
		);

		auto activation = GetBodyActivation(rigidbody);
		auto bodyID = m_BodyInterface->CreateAndAddBody(bodySettings, activation);

		if (bodyID.IsInvalid())
		{
			EE_CORE_WARN("Failed to create Jolt physics body!");
			return;
		}

		JoltPhysicsBody joltBody;
		joltBody.Rigidbody = rigidbody;
		joltBody.BodyID = bodyID;
		m_Bodies.push_back(joltBody);
	}

	void JoltPhysicsSystem::RemoveRigidbody(const Rigidbody* rigidbody)
	{
		auto it = std::find_if(m_Bodies.begin(), m_Bodies.end(), [rigidbody](const auto& body)
		{
			return body.Rigidbody == rigidbody;
		});

		if (it == m_Bodies.end())
		{
			return;
		}

		m_BodyInterface->RemoveBody(it->BodyID);
		m_BodyInterface->DestroyBody(it->BodyID);

		m_Bodies.erase(it);
	}

	void JoltPhysicsSystem::RebuildRigidbody(const Rigidbody* rigidbody)
	{
		RemoveRigidbody(rigidbody);
		AddRigidbody(rigidbody);
	}

	void JoltPhysicsSystem::SyncDynamicBodies()
	{
		for (auto& body : m_Bodies)
		{
			const Rigidbody* rigidbody = body.Rigidbody;

			if (rigidbody->GetType() != RigidbodyType::Dynamic)
			{
				continue;
			}

			// Fetch the data from jolt
			JPH::Vec3 position;
			JPH::Quat rotation;
			m_BodyInterface->GetPositionAndRotation(body.BodyID, position, rotation);
			
			// Apply the data to the world object
			Transform& transform = rigidbody->gameObject->GetTransform();
			transform.SetPosition(glm::vec3{ position.GetX(), position.GetY(), position.GetZ() });
			transform.SetRotationQuaternion(glm::quat{ rotation.GetX(), rotation.GetY(), rotation.GetZ(), rotation.GetW() });
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