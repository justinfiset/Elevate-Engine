module;

#include "Reflection.h"

export module Elevate.Physics.CapsuleCollider;

import Elevate.Core.Scene;
import Elevate.Physics.PhysicsShape;
export import Elevate.Physics.CapsuleShape;
export import Elevate.Physics.Collider;

export namespace Elevate
{
	class CapsuleCollider : public Collider
	{
		BEGIN_COMPONENT(CapsuleCollider);
		DECLARE_BASE(Collider);
		EECATEGORY("Physics");

	public:
		CapsuleCollider() = default;
		CapsuleCollider(float radius, float halfHeight) : m_Shape(radius, halfHeight) {}

		const CapsuleShape& GetShape() const
		{
			return m_Shape;
		}

		const PhysicsShape* GetPhysicsShape() const override { return &m_Shape; }

	private:
		CapsuleShape m_Shape;
		//PROPERTY(m_Shape, FlattenTag())

		END_COMPONENT();
	};
}
