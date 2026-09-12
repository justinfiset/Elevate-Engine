#pragma once

#include "PhysicsShape.h"
#include <glm/glm.hpp>

namespace Elevate
{
    class BoxShape : public PhysicsShape
    {
    public:
        BoxShape(const glm::vec3 &halfExtents)
            : m_HalfExtents(halfExtents) {}
        ~BoxShape() = default;

        const glm::vec3& GetHalfExtents() const { return m_HalfExtents; }

    private:
        glm::vec3 m_HalfExtents;
    };
}