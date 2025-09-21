#pragma once
#include <ElevateEngine/Core/Component.h>
#include <ElevateEngine/Core/GameObject.h>
#include "glm/glm.hpp"

namespace Elevate
{
	struct SurfaceMaterial
	{
		float m_staticFriction = 0; // static friction coefficient
		float m_dynamicFriction = 0; // dynamic friction coefficient
		float m_restitution = 1; // 0 = no rebound, 1 = bounce with no energy loss
	};

	struct RigidbodyData
	{	
		BEGIN_STRUCT(RigidbodyData)

		glm::vec3 m_gravity = { 0, -9.81, 0 };
		EXPOSE(m_gravity)

		glm::vec3 m_force = { 0, 0, 0 };
		EXPOSE(m_force)

		glm::vec3 m_velocity = { 0, 0, 0 };
		EXPOSE(m_velocity)

		float m_mass = 100; // Mass in kg

		SurfaceMaterial material;

		END_STRUCT()
	};

	class Rigidbody : public Component
	{
	public:
		BEGIN_COMPONENT(Rigidbody)
		EECATEGORY("Physics")

		Rigidbody();

		void Update() override;
	private:
		RigidbodyData m_data;
		EXPOSE(m_data);

		float m_testData = 123.0f;
		EXPOSE(m_testData)

		END_COMPONENT()
	};
}