module;

#include <glm/glm.hpp>

#include "Reflection.h"

export module Elevate.Physics.PlaneCollider;

import Elevate.Core.Scene;
import Elevate.Physics.PhysicsShape;
export import Elevate.Physics.PlaneShape;
export import Elevate.Physics.Collider;

export namespace Elevate
{
	class PlaneCollider : public Collider
	{
		BEGIN_COMPONENT(PlaneCollider);
		DECLARE_BASE(Collider);
		EECATEGORY("Physics");

	public:
		PlaneCollider() = default;
		PlaneCollider(const glm::vec3& size) : m_Shape(size * 0.5f) {}

		const PlaneShape& GetShape() const
		{
			return m_Shape;
		}

		const PhysicsShape* GetPhysicsShape() const override { return &m_Shape; }

#ifdef EE_EDITOR_BUILD
		void RenderWhenSelected() override;
#endif

	private:
		PlaneShape m_Shape;
		PROPERTY(m_Shape, FlattenTag());

		END_COMPONENT();
	};
}
