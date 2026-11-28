module;

#include <glm/glm.hpp>

module Elevate.Physics.Collider;

import Elevate.Core.Scene;
import Elevate.Physics.Rigidbody;

namespace Elevate
{
	const glm::vec3 Collider::GetWorldCenter() const
	{
		return gameObject->GetPosition() + m_Center;
	}

	void Collider::Init()
	{
		OnInitCollider();
	}

	void Collider::Destroy()
	{
		OnRemoveCollider();
	}

	void Collider::OnInitCollider()
	{
		if (!gameObject->HasComponent<Rigidbody>())
		{
			gameObject->AddComponent<Rigidbody>();
		}

		Rigidbody* rb = gameObject->GetComponent<Rigidbody>();
		rb->AddCollider(this);
	}

	void Collider::OnRemoveCollider()
	{
		if (gameObject->HasComponent<Rigidbody>())
		{
			Rigidbody* rb = gameObject->GetComponent<Rigidbody>();
			rb->RemoveCollider(this);
		}
	}
}
