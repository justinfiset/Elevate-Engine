module;

#include <ElevateEngine/Physics/Public/PhysicsBackend.h>
#include <functional>
#include <memory>

export module Elevate.Physics.PhysicsSystem;

// The backend depends on this narrow interface instead of the Rigidbody
// component: Rigidbody's implementation needs PhysicsSystem, so importing
// Rigidbody here would create a module purview cycle under GCC.
import Elevate.Physics.IPhysicsRigidbody;

export namespace Elevate
{
	class PhysicsSystem
	{
	public:
		using Factory = std::function<std::unique_ptr<PhysicsSystem>()>;

		static void SetFactory(Factory factory)
		{
			s_factory = factory;
		}

		static std::unique_ptr<PhysicsSystem> Create()
		{
			RegisterPhysicsSystemBackend();
			return s_factory ? s_factory() : nullptr;
		}

		static PhysicsSystem& Get();
		virtual ~PhysicsSystem() = default;

		static bool IsInitialized();

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void Update(float deltaTime) = 0;

		virtual void FlushAllBodies() = 0;
		virtual void AddRigidbody(IPhysicsRigidbody* rigidbody) = 0;
		virtual void RemoveRigidbody(IPhysicsRigidbody* rigidbody) = 0;
		virtual void RebuildRigidbody(IPhysicsRigidbody* rigidbody) = 0;

	protected:
		static PhysicsSystem* s_Instance;

	private:
		inline static Factory s_factory = nullptr;
	};
}
