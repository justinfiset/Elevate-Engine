#include "eepch.h"
#include "Rigidbody.h"
#include "ElevateEngine/Inputs/Input.h"
#include "ElevateEngine/Core/GameObject.h"
#include "ElevateEngine/Core/Time.h"
#include <ElevateEngine/Physics/PhysicsSystem.h>

namespace Elevate
{
	void Rigidbody::Init()
	{
		PhysicsSystem::Get().AddRigidbody(this);
	}

	void Rigidbody::Destroy()
	{
		PhysicsSystem::Get().RemoveRigidbody(this);
	}

	RigidbodyType Rigidbody::GetType()
	{
		return m_data.m_type;
	}
}