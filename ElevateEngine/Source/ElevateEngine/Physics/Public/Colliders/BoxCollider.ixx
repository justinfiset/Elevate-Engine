module;

#include <glm/glm.hpp>

#include "Reflection.h"

export module Elevate.Physics.BoxCollider;

import Elevate.Core.Scene;
import Elevate.Physics.PhysicsShape;
export import Elevate.Physics.BoxShape;
export import Elevate.Physics.Collider;

export namespace Elevate
{
	class BoxCollider : public Collider
	{
		BEGIN_COMPONENT(BoxCollider);
		DECLARE_BASE(Collider);
		EECATEGORY("Physics");

	public:
		BoxCollider() = default;
		BoxCollider(const glm::vec3& size) : m_Shape(size * 0.5f) {}

		const BoxShape& GetShape() const
		{
			return m_Shape;
		}

		const PhysicsShape* GetPhysicsShape() const override { return &m_Shape; }

#ifdef EE_EDITOR_BUILD
		void RenderWhenSelected() override;
#endif

	private:
		BoxShape m_Shape;
		PROPERTY(m_Shape, FlattenTag());

		END_COMPONENT();
	};
}
