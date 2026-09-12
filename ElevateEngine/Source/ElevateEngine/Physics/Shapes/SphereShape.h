#pragma once

#include "PhysicsShape.h"

namespace Elevate
{
    class SphereShape : public PhysicsShape
    {
    public:
        SphereShape(float radius)
            : m_Radius(radius) {}
        ~SphereShape() = default;

        float GetRadius() const { return m_Radius; }

    private:
        float m_Radius;
    };
}