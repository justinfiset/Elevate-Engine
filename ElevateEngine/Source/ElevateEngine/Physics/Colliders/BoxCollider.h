#pragma once
#include "Collider.h"

#include <glm/glm.hpp>

#include <ElevateEngine/Physics/Shapes/BoxShape.h>
#include <ElevateEngine/Core/Reflection.h>

namespace Elevate
{
    class BoxCollider : public Collider
    {
        BEGIN_COMPONENT(BoxCollider)
        DECLARE_BASE(Collider)
        EECATEGORY("Physics")

    public:
        BoxCollider() = default;
        BoxCollider(const glm::vec3& size) : m_Shape(size * 0.5f) { }

        const BoxShape& GetShape() const
        {
            return m_Shape;
        }

    private:
        BoxShape m_Shape;
        PROPERTY(m_Shape, EE_Flatten)

        END_COMPONENT()
    };
}
