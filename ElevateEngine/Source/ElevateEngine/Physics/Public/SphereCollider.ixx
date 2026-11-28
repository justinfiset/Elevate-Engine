module;

#include "Reflection.h"

export module Elevate.Physics.SphereCollider;

import Elevate.Core.Scene;
import Elevate.Physics.PhysicsShape;
export import Elevate.Physics.SphereShape;
export import Elevate.Physics.Collider;

export namespace Elevate
{
	class SphereCollider : public Collider
	{
		BEGIN_COMPONENT(SphereCollider);
		DECLARE_BASE(Collider);
		EECATEGORY("Physics");

	public:
		SphereCollider() = default;
		explicit SphereCollider(float radius) : m_Shape(radius) {}

		const SphereShape& GetShape() const
		{
			return m_Shape;
		}

		const PhysicsShape* GetPhysicsShape() const override { return &m_Shape; }

	private:
		SphereShape m_Shape;
		PROPERTY(m_Shape, FlattenTag());

		END_COMPONENT();
	};
}
