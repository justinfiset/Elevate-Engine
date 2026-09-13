#pragma once
#include "Collider.h"

#include <glm/glm.hpp>

#include <ElevateEngine/Physics/Shapes/SphereShape.h>
#include <ElevateEngine/Core/Reflection.h>

namespace Elevate
{
    class SphereCollider : public Collider
    {
        BEGIN_COMPONENT(SphereCollider)
        DECLARE_BASE(Collider)
        EECATEGORY("Physics")

    public:
        SphereCollider() = default;
        SphereCollider(float radius) : m_Shape(radius) {}

        const SphereShape& GetShape() const
        {
            return m_Shape;
        }

    private:
        SphereShape m_Shape;
        PROPERTY(m_Shape, EE_Flatten)

        END_COMPONENT()
    };
}
