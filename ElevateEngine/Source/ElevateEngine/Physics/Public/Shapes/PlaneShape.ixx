module;

#include <glm/glm.hpp>

#include "Reflection.h"

export module Elevate.Physics.PlaneShape;

import Elevate.Physics.PhysicsShape;

export namespace Elevate
{
	struct PlaneShape : public PhysicsShape
	{
		BEGIN_STRUCT(PlaneShape);
	public:
		PlaneShape() : m_Size(0.5f, 0.5f) {}
		PlaneShape(const glm::vec2& size) : m_Size(size) {}
		~PlaneShape() = default;

		const glm::vec2& GetSize() const { return m_Size; }

	private:
		glm::vec2 m_Size;
		PROPERTY(m_Size);

		END_STRUCT();
	};
}
