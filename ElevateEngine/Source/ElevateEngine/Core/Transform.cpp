#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp> // iwyu: keep

Elevate::Transform::Transform()
	: m_ModelMatrix(glm::mat4(1.0f)), position(glm::vec3(0.0f)),
		rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), scale(glm::vec3(1.0f)), m_isDirty(true)
{ }

void Elevate::Transform::SetPosition(const glm::vec3& pos)
{
	position = pos;
	m_isDirty = true;
}

void Elevate::Transform::SetRotation(const glm::vec3 & rot)
{
	rotation = glm::normalize(glm::quat(glm::radians(rot)));
	m_isDirty = true;
}

void Elevate::Transform::SetRotationQuaternion(const glm::quat& rot)
{
	rotation = glm::normalize(rot);
	m_isDirty = true;
}

void Elevate::Transform::SetScale(const glm::vec3& scale)
{
	this->scale = scale;
	m_isDirty = true;
}

glm::vec3 Elevate::Transform::GetRotation() const
{
	return glm::degrees(glm::eulerAngles(rotation));
}

const glm::quat& Elevate::Transform::GetRotationQuat() const
{
	return rotation;
}

glm::vec3 Elevate::Transform::GetRight() const
{
	return glm::normalize(glm::vec3(GetModelMatrix()[0]));
}

glm::vec3 Elevate::Transform::GetLeft() const
{
	return -GetRight();
}

glm::vec3 Elevate::Transform::GetUp() const
{
	return glm::normalize(glm::vec3(GetModelMatrix()[1]));
}

glm::vec3 Elevate::Transform::GetDown() const
{
	return -GetUp();
}

glm::vec3 Elevate::Transform::GetBackward() const
{
	return glm::normalize(glm::vec3(GetModelMatrix()[2]));
}

glm::vec3 Elevate::Transform::GetForward() const
{
	return -GetBackward();
}

glm::vec3 Elevate::Transform::GetGlobalScale() const
{
	const glm::mat4& model = GetModelMatrix();

	return {
		glm::length(glm::vec3(model[0])),
		glm::length(glm::vec3(model[1])),
		glm::length(glm::vec3(model[2]))
	};
}

const glm::mat4& Elevate::Transform::GetModelMatrix() const
{
	if (m_isDirty)
	{
		const_cast<Transform*>(this)->UpdateModelMatrix();
	}
	return m_ModelMatrix;
}

void Elevate::Transform::UpdateModelMatrix()
{
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, position);
	model *= glm::mat4_cast(rotation);
	model = glm::scale(model, scale);

	m_ModelMatrix = model;
	m_isDirty = false;
}
