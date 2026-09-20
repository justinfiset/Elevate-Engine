#pragma once

// Physics Forwards
namespace Elevate
{
	class Rigidbody;
	class BoxCollider;
	class CapsuleCollider;
	class Collider;
	class MeshCollider;
	class SphereCollider;
	class PlaneCollider;
}

namespace Elevate	
{
	class PhysicsSystem
	{
	public:
		static PhysicsSystem& Get();

		~PhysicsSystem() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void Update(float deltaTime) = 0;
	
		virtual void AddRigidbody(Rigidbody* rigidbody) = 0;
		virtual void RemoveRigidbody(Rigidbody* rigidbody) = 0;

	protected:
		static PhysicsSystem* s_Instance;
	};
}
