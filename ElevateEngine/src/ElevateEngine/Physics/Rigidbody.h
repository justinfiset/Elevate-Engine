#pragma once
#include <ElevateEngine/Core/Component.h>
#include <ElevateEngine/Core/GameObject.h>
#include <ElevateEngine/Core/ComponentRegistry.h>
#include <ElevateEngine/Editor/Components/ComponentsIcon.h>

#include "glm/glm.hpp"

namespace Elevate
{
	struct SurfaceMaterial
	{
		BEGIN_STRUCT(SurfaceMaterial)

		float m_staticFriction = 0; // static friction coefficient
		PROPERTY(m_staticFriction)
		float m_dynamicFriction = 0; // dynamic friction coefficient
		PROPERTY(m_dynamicFriction)
		float m_restitution = 1; // 0 = no rebound, 1 = bounce with no energy loss
		PROPERTY(m_restitution)

		END_STRUCT()
	};

	struct RigidbodyData
	{	
		BEGIN_STRUCT(RigidbodyData)

		glm::vec3 m_gravity = { 0, -9.81, 0 };
		PROPERTY(m_gravity)

		glm::vec3 m_force = { 0, 0, 0 };
		PROPERTY(m_force)

		glm::vec3 m_velocity = { 0, 0, 0 };
		PROPERTY(m_velocity)

		float m_mass = 100; // Mass in kg
		PROPERTY(m_mass)

		SurfaceMaterial material;
		PROPERTY(material)

		END_STRUCT()
	};

	class Rigidbody : public Component
	{
	public:
		BEGIN_COMPONENT(Rigidbody, EditorIcon(std::string(Editor::Icons::COMPONENT_ICON_RIGIDBODY)))
		EECATEGORY("Physics")

		Rigidbody();
		Rigidbody(const Rigidbody& other);

		void Update() override;

	private:
		RigidbodyData m_data;
		PROPERTY(m_data, Flatten)

		END_COMPONENT()
	};
}