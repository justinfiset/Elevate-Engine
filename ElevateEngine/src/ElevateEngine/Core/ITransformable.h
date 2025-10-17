#pragma once

#include "Transform.h"

namespace Elevate
{
	// TODO UPDATE TO MATCH NEW TRANSFORM FEATURES;
	/// <summary>
	/// An interface to add all of the methods needed to have a working 
	/// transform wrapped by getter and setters
	/// </summary>
	class ITransformable
	{
	public:
		inline Transform& GetTransform() { return m_Transform; }
		inline const Transform& GetTransform() const { return m_Transform; }

		// position
		inline void SetPosition(glm::vec3 pos)
		{ 
			m_Transform.SetPosition(pos);
			OnSetPosition();
		}
		inline void SetRotation(glm::vec3 rotation)
		{
			m_Transform.SetRotation(rotation);
			OnSetRotation();
		}
		inline void SetScale(glm::vec3 scale)
		{
			m_Transform.SetScale(scale);
			OnSetScale();
		}

		inline glm::vec3& GetPosition() { return m_Transform.GetPosition(); }
		inline glm::vec3& GetRotation() { return m_Transform.GetRotation(); }
		inline glm::vec3& GetScale() { return m_Transform.GetScale(); }

		inline const glm::mat4& GetModelMatrix() const { return m_Transform.GetModelMatrix(); }

	protected:
		// Callbakcs for any objects the inherits
		virtual void OnSetPosition() {}
		virtual void OnSetRotation() {}
		virtual void OnSetScale() {}

	protected:
		Transform m_Transform;
	};
}