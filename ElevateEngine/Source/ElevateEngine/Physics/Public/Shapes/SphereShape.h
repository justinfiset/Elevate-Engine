#pragma once

#include "PhysicsShape.h"
#include <ElevateEngine/Core/Reflection.h>

namespace Elevate
{
    struct SphereShape : public PhysicsShape
    {
        BEGIN_STRUCT(SphereShape)
    public:
        SphereShape() : m_Radius(0.5f) { }
        SphereShape(float radius) : m_Radius(radius) { }
        ~SphereShape() = default;

        float GetRadius() const { return m_Radius; }

    private:
        float m_Radius;
        PROPERTY(m_Radius)

        END_STRUCT()
    };
}