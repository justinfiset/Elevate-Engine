#pragma once

// Keep every textual include before Collider.h: Collider.h imports modules,
// and GCC reprocesses standard headers included after an import.
#include "Reflection.h"
#include <ElevateEngine/Physics/Public/Shapes/CapsuleShape.h>

#include "Collider.h"

namespace Elevate
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

	private:
		CapsuleShape m_Shape;
		//PROPERTY(m_Shape, FlattenTag())

		END_COMPONENT();
	};
}
