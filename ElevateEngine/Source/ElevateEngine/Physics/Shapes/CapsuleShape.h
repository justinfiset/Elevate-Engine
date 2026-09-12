#pragma once

#include "PhysicsShape.h"

namespace Elevate
{
    class CapsuleShape : public PhysicsShape
    {
    public:
        CapsuleShape(float radius, float halfHeight)
            : m_Radius(radius), m_HalfHeight(halfHeight) {}
        ~CapsuleShape() = default;

        float GetRadius() const { return m_Radius; }
        float GetHalfHeight() const { return m_HalfHeight; }

    private:
        float m_Radius;
        float m_HalfHeight;
    };
}