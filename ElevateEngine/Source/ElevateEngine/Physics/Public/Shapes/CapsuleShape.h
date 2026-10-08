#pragma once

#include "PhysicsShape.h"
#include <ElevateEngine/Core/Reflection.h>

namespace Elevate
{
    struct CapsuleShape : public PhysicsShape
    {
        BEGIN_STRUCT(CapsuleShape)

    public:
        CapsuleShape() : m_Radius(0.5f), m_HalfHeight(0.5f) { }
        CapsuleShape(float radius, float halfHeight) : m_Radius(radius), m_HalfHeight(halfHeight) { }
        ~CapsuleShape() = default;

        float GetRadius() const { return m_Radius; }
        float GetHalfHeight() const { return m_HalfHeight; }

    private:
        float m_Radius;
        PROPERTY(m_Radius)

        float m_HalfHeight;
        PROPERTY(m_HalfHeight)

        END_STRUCT()
    };
}