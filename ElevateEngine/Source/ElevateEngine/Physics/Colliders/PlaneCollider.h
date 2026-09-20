#pragma once
#include "Collider.h"

#include <ElevateEngine/Physics/Shapes/PlaneShape.h>
#include <ElevateEngine/Core/Reflection.h>

namespace Elevate
{
    class PlaneCollider : public Collider
    {
        BEGIN_COMPONENT(PlaneCollider)
        DECLARE_BASE(Collider)
        EECATEGORY("Physics")

    public:
        PlaneCollider() = default;
        PlaneCollider(const glm::vec3& size) : m_Shape(size * 0.5f) {}

        const PlaneShape& GetShape() const
        {
            return m_Shape;
        }

#ifdef EE_EDITOR_BUILD
        void RenderWhenSelected() override;
#endif

    private:
        PlaneShape m_Shape;
        PROPERTY(m_Shape, EE_Flatten)

        END_COMPONENT()
    };
}
