#pragma once

namespace Elevate
{
	class PhysicsSystem
	{
	public:
		~PhysicsSystem() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void Update(float deltaTime) = 0;
	};
}
