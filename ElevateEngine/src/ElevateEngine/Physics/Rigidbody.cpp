#include "eepch.h"
#include "Rigidbody.h"
#include "ElevateEngine/Inputs/Input.h"
#include "ElevateEngine/Core/GameObject.h"
#include "ElevateEngine/Core/Time.h"

namespace Elevate
{
	Rigidbody::Rigidbody()
	{
	}

	Rigidbody::Rigidbody(const Rigidbody& other)
	{
		m_data = other.m_data;
	}

	void Rigidbody::Update()
	{
		m_data.m_force = m_data.m_gravity * m_data.m_mass;
		m_data.m_velocity = m_data.m_velocity + m_data.m_force / m_data.m_mass * Time::GetDeltaTime();
		glm::vec3 pos = gameObject->GetPosition() + m_data.m_velocity * Time::GetDeltaTime();
		gameObject->SetPosition(pos);
	}

	//Component* Rigidbody::Clone()
	//{
	//	return new Rigidbody(*this);
	//}
}