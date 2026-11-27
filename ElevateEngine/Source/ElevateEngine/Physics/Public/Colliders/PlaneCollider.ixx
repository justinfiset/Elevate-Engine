module;

#include <glm/glm.hpp>

#include "Reflection.h"
#include <ElevateEngine/Physics/Public/Shapes/PlaneShape.h>

export module Elevate.Physics.PlaneCollider;

import Elevate.Core.Scene;
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

#ifdef EE_EDITOR_BUILD
		void RenderWhenSelected() override;
#endif

	private:
		PlaneShape m_Shape;
		PROPERTY(m_Shape, FlattenTag());

		END_COMPONENT();
	};
}
