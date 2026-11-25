module;

#include <vector>

module Elevate.Physics.Rigidbody;

import :Types;
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

	const std::vector<const Component*>& Rigidbody::GetColliders() const
	{
		return m_Colliders;
	}

	void Rigidbody::AddCollider(const Component* collider)
	{
		m_Colliders.push_back(collider);
		PhysicsSystem::Get().RebuildRigidbody(this);
	}

	void Rigidbody::RemoveCollider(const Component* collider)
	{
		m_Colliders.push_back(collider);
		PhysicsSystem::Get().RebuildRigidbody(this);
	}
}
