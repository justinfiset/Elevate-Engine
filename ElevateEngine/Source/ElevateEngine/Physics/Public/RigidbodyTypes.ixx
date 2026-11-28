module;

#include <cstdint>

#include <glm/glm.hpp>

#include "Reflection.h"

export module Elevate.Physics.Rigidbody:Types;

import Elevate.Foundations.Enums;

export namespace Elevate
{
	enum class RigidbodyType : EnumType
	{
		Dynamic,
		Kinematic,
		Static
	};

	BEGIN_ENUM(RigidbodyType)
		ENUM_VALUE(Dynamic)
		ENUM_VALUE(Kinematic)
		ENUM_VALUE(Static)
		END_ENUM(RigidbodyType);

	struct SurfaceMaterial
	{
		BEGIN_STRUCT(SurfaceMaterial);

		float m_staticFriction = 0;
		PROPERTY(m_staticFriction);
		float m_dynamicFriction = 0;
		PROPERTY(m_dynamicFriction);
		float m_restitution = 1;
		PROPERTY(m_restitution);

		END_STRUCT();
	};

	struct RigidbodyData
	{
		BEGIN_STRUCT(RigidbodyData);

		RigidbodyType m_type = RigidbodyType::Dynamic;
		PROPERTY(m_type, DisplayNameTag("Body Type"));

		glm::vec3 m_gravity = { 0, -9.81, 0 };
		PROPERTY(m_gravity);

		glm::vec3 m_force = { 0, 0, 0 };
		PROPERTY(m_force);

		glm::vec3 m_velocity = { 0, 0, 0 };
		PROPERTY(m_velocity);

		float m_mass = 100;
		PROPERTY(m_mass);

		SurfaceMaterial material;
		PROPERTY(material);

		END_STRUCT();
	};
}
