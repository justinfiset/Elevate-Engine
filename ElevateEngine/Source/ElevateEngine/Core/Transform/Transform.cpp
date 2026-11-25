module;

#include <memory>

#include <glm/mat4x4.hpp>				// iwyu: keep
#include <glm/gtc/quaternion.hpp>		// iwyu: keep
#include <glm/vec3.hpp>					// iwyu: keep
#include <glm/gtc/matrix_transform.hpp> // iwyu: keep

module Elevate.Core.Transform.Transform;

namespace Elevate
{
	Transform::Transform()
		: m_ModelMatrix(glm::mat4(1.0f)), position(glm::vec3(0.0f)),
		rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), scale(glm::vec3(1.0f)), m_isDirty(true)
	{
	}

	void Transform::SetPosition(const glm::vec3& pos)
	{
		position = pos;
		m_isDirty = true;
	}

	void Transform::SetRotation(const glm::vec3& rot)
	{
		rotation = glm::normalize(glm::quat(glm::radians(rot)));
		m_isDirty = true;
	}

	void Transform::SetRotationQuaternion(const glm::quat& rot)
	{
		rotation = glm::normalize(rot);
		m_isDirty = true;
	}

	void Transform::SetScale(const glm::vec3& scale)
	{
		this->scale = scale;
		m_isDirty = true;
	}

	glm::vec3 Transform::GetRotation() const
	{
		return glm::degrees(glm::eulerAngles(rotation));
	}

	const glm::quat& Transform::GetRotationQuat() const
	{
		return rotation;
	}

	glm::vec3 Transform::GetRight() const
	{
		return glm::normalize(glm::vec3(GetModelMatrix()[0]));
	}

	glm::vec3 Transform::GetLeft() const
	{
		return -GetRight();
	}

	glm::vec3 Transform::GetUp() const
	{
		return glm::normalize(glm::vec3(GetModelMatrix()[1]));
	}

	glm::vec3 Transform::GetDown() const
	{
		return -GetUp();
	}

	glm::vec3 Transform::GetBackward() const
	{
		return glm::normalize(glm::vec3(GetModelMatrix()[2]));
	}

	glm::vec3 Transform::GetForward() const
	{
		return -GetBackward();
	}

	glm::vec3 Transform::GetGlobalScale() const
	{
		const glm::mat4& model = GetModelMatrix();

		return {
			glm::length(glm::vec3(model[0])),
			glm::length(glm::vec3(model[1])),
			glm::length(glm::vec3(model[2])) };
	}

	const glm::mat4& Transform::GetModelMatrix() const
	{
		if (m_isDirty)
		{
			const_cast<Transform*>(this)->UpdateModelMatrix();
		}
		return m_ModelMatrix;
	}

	void Transform::UpdateModelMatrix()
	{
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, position);
		model *= glm::mat4_cast(rotation);
		model = glm::scale(model, scale);

		m_ModelMatrix = model;
		m_isDirty = false;
	}
}
