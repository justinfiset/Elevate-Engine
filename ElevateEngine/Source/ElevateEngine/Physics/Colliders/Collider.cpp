#include "Collider.h"

const glm::vec3 Elevate::Collider::GetWorldCenter() const
{
    return gameObject->GetPosition() + m_Center;
}
