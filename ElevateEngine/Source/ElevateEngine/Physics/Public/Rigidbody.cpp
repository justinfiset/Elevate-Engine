module;

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

module Elevate.Physics.Rigidbody;

import :Types;
import Elevate.Core.Transform.Transform;
import Elevate.Physics.PhysicsSystem;

namespace Elevate
{
	void Rigidbody::Awake()
	{
		PhysicsSystem::Get().AddRigidbody(this);
	}

	void Rigidbody::Destroy()
	{
		PhysicsSystem::Get().RemoveRigidbody(this);
	}

	RigidbodyType Rigidbody::GetType() const
	{
		return m_data.m_type;
	}

	bool Rigidbody::IsDynamic() const
	{
		return m_data.m_type == RigidbodyType::Dynamic;
	}

	bool Rigidbody::IsKinematic() const
	{
		return m_data.m_type == RigidbodyType::Kinematic;
	}

	bool Rigidbody::IsStatic() const
	{
		return m_data.m_type == RigidbodyType::Static;
	}

	glm::vec3 Rigidbody::GetPosition() const
	{
		return gameObject->GetTransform().GetPosition();
	}

	glm::quat Rigidbody::GetRotation() const
	{
		return gameObject->GetTransform().GetRotationQuat();
	}

	void Rigidbody::SetPosition(const glm::vec3& position)
	{
		gameObject->GetTransform().SetPosition(position);
	}

	void Rigidbody::SetRotation(const glm::quat& rotation)
	{
		gameObject->GetTransform().SetRotationQuaternion(rotation);
	}

	const std::vector<const IPhysicsCollider*>& Rigidbody::GetPhysicsColliders() const
	{
		return m_Colliders;
	}

	void Rigidbody::AddCollider(const IPhysicsCollider* collider)
	{
		m_Colliders.push_back(collider);
		PhysicsSystem::Get().RebuildRigidbody(this);
	}

	void Rigidbody::RemoveCollider(const IPhysicsCollider* collider)
	{
		const auto it = std::find(m_Colliders.begin(), m_Colliders.end(), collider);
		if (it == m_Colliders.end())
		{
			return;
		}

		m_Colliders.erase(it);
		PhysicsSystem::Get().RebuildRigidbody(this);
	}
}
