module;

#include <glm/glm.hpp>

#include "Reflection.h"

export module Elevate.Physics.SphereShape;

import Elevate.Physics.PhysicsShape;

export namespace Elevate
{
	struct SphereShape : public PhysicsShape
	{
		BEGIN_STRUCT(SphereShape);
	public:
		SphereShape() : m_Radius(0.5f) {}
		explicit SphereShape(float radius) : m_Radius(radius) {}
		~SphereShape() = default;

		float GetRadius() const { return m_Radius; }

	private:
		float m_Radius;
		PROPERTY(m_Radius);

		END_STRUCT();
	};
}
