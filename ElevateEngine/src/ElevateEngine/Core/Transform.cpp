#include "eepch.h"
#include "Transform.h"

// Maths
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include <ImGuizmo.h>

Elevate::Transform::Transform()
{
	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	m_isDirty = true;
}

glm::vec3 Elevate::Transform::GetRight() const
{
	return glm::normalize(glm::vec3(GetModelMatrix()[0]));
}

glm::vec3 Elevate::Transform::GetUp() const
{
	return glm::normalize(glm::vec3(GetModelMatrix()[1]));
}

glm::vec3 Elevate::Transform::GetBackward() const
{
	return glm::normalize(glm::vec3(GetModelMatrix()[2]));
}

glm::vec3 Elevate::Transform::GetForward() const
{
	return -glm::normalize(glm::vec3(GetModelMatrix()[2]));
}

glm::vec3 Elevate::Transform::GetGlobalScale() const
{
	return { glm::length(GetRight()), glm::length(GetUp()), glm::length(GetBackward()) };
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

	model = glm::rotate(model, glm::radians(rotation.y), { 0, 1, 0 });
	model = glm::rotate(model, glm::radians(rotation.x), { 1, 0, 0 });
	model = glm::rotate(model, glm::radians(rotation.z), { 0, 0, 1 });

	model = glm::scale(model, scale);
	m_ModelMatrix = model;
	m_isDirty = false;
}
