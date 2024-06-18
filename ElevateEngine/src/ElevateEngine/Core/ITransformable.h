#pragma once

#include "Transform.h"

namespace Elevate
{
	/// <summary>
	/// An interface to add all of the methods needed to have a working 
	/// transform wrapped by getter and setters
	/// </summary>
	class ITransformable
	{
	public:
		inline Transform* GetTransform() { return &m_Transform; }

		// position
		void SetPosition(glm::vec3 pos) { m_Transform.SetPosition(pos); }
		void SetRotation(glm::vec3 rotation) { m_Transform.SetRotation(rotation); }
		void SetScale(glm::vec3 scale) { m_Transform.SetScale(scale); }

		glm::vec3* GetPoition() { return &m_Transform.position; }
		glm::vec3* GetRotation() { return &m_Transform.rotation; }
		glm::vec3* GetScale() { return &m_Transform.scale; }
	protected:
		Transform m_Transform;
	};
}