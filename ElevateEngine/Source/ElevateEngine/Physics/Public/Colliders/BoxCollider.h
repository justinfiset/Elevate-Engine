#pragma once

// Keep every textual include before Collider.h: Collider.h imports modules,
// and GCC reprocesses standard headers included after an import.
#include <glm/glm.hpp>
#include "Reflection.h"
#include <ElevateEngine/Physics/Public/Shapes/BoxShape.h>

#include "Collider.h"

namespace Elevate
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

#ifdef EE_EDITOR_BUILD
		void RenderWhenSelected() override;
#endif

	private:
		BoxShape m_Shape;
		PROPERTY(m_Shape, FlattenTag());

		END_COMPONENT();
	};
}
