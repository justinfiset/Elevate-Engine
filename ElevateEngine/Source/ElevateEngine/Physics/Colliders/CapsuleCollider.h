#pragma once
#include "Collider.h"

#include <ElevateEngine/Physics/Shapes/CapsuleShape.h>
#include <ElevateEngine/Core/Reflection.h>

namespace Elevate
{
    class CapsuleCollider : public Collider
    {
        BEGIN_COMPONENT(CapsuleCollider)
        DECLARE_BASE(Collider)
        EECATEGORY("Physics")

    public:
        CapsuleCollider() = default;
        CapsuleCollider(float radius, float halfHeight) : m_Shape(radius, halfHeight) {}

        const CapsuleShape& GetShape() const
        {
            return m_Shape;
        }

    private:
        CapsuleShape m_Shape;
        //PROPERTY(m_Shape, EE_Flatten)

        END_COMPONENT()
    };
}
