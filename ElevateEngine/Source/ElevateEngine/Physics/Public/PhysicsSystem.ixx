module;

#include <ElevateEngine/Physics/Public/PhysicsBackend.h>
#include <memory>

export module Elevate.Physics.PhysicsSystem;

// Keep this interface independent of Elevate.Physics.Rigidbody: Rigidbody's
// implementation needs PhysicsSystem, so importing it here would create a
// module dependency cycle. Component is the common typed handle instead.
import Elevate.Core.Scene;

export namespace Elevate
{
	class PhysicsSystem
	{
	public:
		using Factory = std::unique_ptr<PhysicsSystem> (*)();

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
		virtual void AddRigidbody(const Component* rigidbody) = 0;
		virtual void RemoveRigidbody(const Component* rigidbody) = 0;
		virtual void RebuildRigidbody(const Component* rigidbody) = 0;

	protected:
		static PhysicsSystem* s_Instance;

	private:
		inline static Factory s_factory = nullptr;
	};
}
