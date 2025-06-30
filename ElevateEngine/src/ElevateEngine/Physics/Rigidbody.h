#pragma once
#include "ElevateEngine/Core/Component.h"

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
		glm::vec3 m_gravity = { 0, -9.81, 0 };
		glm::vec3 m_force = { 0, 0, 0 };
		glm::vec3 m_velocity = { 0, 0, 0 };

		float m_mass = 100; // Mass in kg

		SurfaceMaterial material;
	};

	class Rigidbody : public Component
	{
	public:
		COMPONENT_LAYOUT(
			"Rigidbody",
			{
				//{"FOV", ComponentDataType::Float, &m_FOV}
			}
		);

		Rigidbody();

		void Update() override;
	private:
		RigidbodyData m_data;
	};

	using RigidbodyPtr = std::weak_ptr<Rigidbody>;
}