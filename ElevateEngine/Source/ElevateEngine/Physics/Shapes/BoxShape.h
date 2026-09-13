#pragma once

#include "PhysicsShape.h"
#include <glm/glm.hpp>

namespace Elevate
{
    struct BoxShape : public PhysicsShape
    {
        BEGIN_STRUCT(BoxShape)
    public:
        BoxShape() : m_HalfExtents(0.5f, 0.5f, 0.5f) { }
        BoxShape(const glm::vec3 &halfExtents) : m_HalfExtents(halfExtents) { }
        ~BoxShape() = default;

        const glm::vec3& GetHalfExtents() const { return m_HalfExtents; }

    private:
        glm::vec3 m_HalfExtents;
        PROPERTY(m_HalfExtents)

        END_STRUCT()
    };
}