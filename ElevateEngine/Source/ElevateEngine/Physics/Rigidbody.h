#pragma once

#include <vector>

#include <ElevateEngine/Core/Reflection.h>
#include <ElevateEngine/Core/Component.h>
#include <ElevateEngine/Core/GameObject.h>
#include <ElevateEngine/Core/TypeRegistry.h>
#include <ElevateEngine/Editor/Components/ComponentsIcon.h>

#include "glm/glm.hpp"

namespace Elevate
{
	class Collider;
}

namespace Elevate
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
	END_ENUM()

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

		RigidbodyType m_type = RigidbodyType::Dynamic;
		PROPERTY(m_type, EE_DisplayName("Body Type"))

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
		friend class Collider;

	public:
		BEGIN_COMPONENT(Rigidbody, EE_EditorIcon(Editor::Icons::COMPONENT_ICON_RIGIDBODY))
		EECATEGORY("Physics")

		Rigidbody() = default;

		void Init() override;
		void Destroy() override;

		RigidbodyType GetType() const;
		const std::vector<const Collider*>& GetColliders() const;

	private:
		void AddCollider(const Collider* collider);
		void RemoveCollider(const Collider* collider);

	private:
		RigidbodyData m_data;
		PROPERTY(m_data, EE_Flatten)

		std::vector<const Collider*> m_Colliders;

		END_COMPONENT()
	};
}