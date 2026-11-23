module;

// Keep the forward declaration in the global module fragment so it names the
// same entity as the definition in Rigidbody.h.  A declaration in the module
// purview would create a second, module-attached Elevate::Rigidbody and make
// references ambiguous in translation units that import this module and
// include the header.
namespace Elevate
{
	class Rigidbody;
}

export module Elevate.Physics.PhysicsSystem;

export namespace Elevate
{
	class PhysicsSystem
	{
	public:
		static PhysicsSystem& Get();
		virtual ~PhysicsSystem() = default;

		static bool IsInitialized();

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void Update(float deltaTime) = 0;

		virtual void FlushAllBodies() = 0;
		virtual void AddRigidbody(const Rigidbody* rigidbody) = 0;
		virtual void RemoveRigidbody(const Rigidbody* rigidbody) = 0;
		virtual void RebuildRigidbody(const Rigidbody* rigidbody) = 0;

	protected:
		static PhysicsSystem* s_Instance;
	};
}
