#include "eepch.h"
#include "Rigidbody.h"
#include "ElevateEngine/Inputs/Input.h"
#include "ElevateEngine/Core/GameObject.h"
#include "ElevateEngine/Core/Time.h"
#include <ElevateEngine/Physics/PhysicsSystem.h>

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

	const std::vector<const Collider*>& Rigidbody::GetColliders() const
	{
		return m_Colliders;
	}

	void Rigidbody::AddCollider(const Collider* collider)
	{	
		m_Colliders.push_back(collider);
		PhysicsSystem::Get().RebuildRigidbody(this);
	}

	void Rigidbody::RemoveCollider(const Collider* collider)
	{
		m_Colliders.push_back(collider);
		PhysicsSystem::Get().RebuildRigidbody(this);
	}
}