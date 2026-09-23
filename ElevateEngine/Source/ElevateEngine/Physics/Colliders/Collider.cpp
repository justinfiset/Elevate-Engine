#include "Collider.h"
#include <ElevateEngine/Physics/Rigidbody.h>

const glm::vec3 Elevate::Collider::GetWorldCenter() const
{
    return gameObject->GetPosition() + m_Center;
}

void Elevate::Collider::Init()
{
    OnInitCollider();
}

void Elevate::Collider::Destroy()
{
    OnRemoveCollider();
}

void Elevate::Collider::OnInitCollider()
{
    if (!gameObject->HasComponent<Rigidbody>())
    {
        gameObject->AddComponent<Rigidbody>();
    }

    Rigidbody* rb = gameObject->GetComponent<Rigidbody>();
    rb->AddCollider(this);
}

void Elevate::Collider::OnRemoveCollider()
{
    if (gameObject->HasComponent<Rigidbody>())
    {
        Rigidbody* rb = gameObject->GetComponent<Rigidbody>();
        rb->RemoveCollider(this);
    }
}