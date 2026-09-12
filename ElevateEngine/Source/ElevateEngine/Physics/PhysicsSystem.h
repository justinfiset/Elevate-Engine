#pragma once

namespace Elevate
{
	class PhysicsSystem
	{
		~PhysicsSystem() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void Update(float deltaTime) = 0;
	};
}
