#pragma once
#include <glm/glm.hpp>

#include <ElevateEngine/Core/Component.h>
#include <ElevateEngine/Core/Reflection.h>

namespace Elevate
{
    class Collider : public Component
    {
        BEGIN_COMPONENT(Collider, EE_HideInInspector)
        EECATEGORY("Physics")

    public:
        virtual ~Collider() = default;

        const glm::vec3& GetCenter() const { return m_Center; }
        void SetCenter(const glm::vec3& center) { m_Center = center; }

    private:
        bool m_IsTrigger{ false };
        PROPERTY(m_IsTrigger)

        glm::vec3 m_Center{0.0f, 0.0f, 0.0f};
        PROPERTY(m_Center)

        END_COMPONENT()
    };
}