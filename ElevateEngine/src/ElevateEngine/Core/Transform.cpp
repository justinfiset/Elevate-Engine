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
}

glm::vec3 Elevate::Transform::GetRight() const
{
	return m_ModelMatrix[2];
}

glm::vec3 Elevate::Transform::GetUp() const
{
	return m_ModelMatrix[1];
}

glm::vec3 Elevate::Transform::GetBackward() const
{
	return m_ModelMatrix[0];
}

glm::vec3 Elevate::Transform::GetForward() const
{
	return -GetBackward();
}

glm::vec3 Elevate::Transform::GetGlobalScale() const
{
	return { glm::length(GetRight()), glm::length(GetUp()), glm::length(GetBackward()) };
}

const glm::mat4& Elevate::Transform::GetModelMatrix() const
{
	return m_ModelMatrix;
}

void Elevate::Transform::UpdateModelMatrix()
{
	ImGuizmo::RecomposeMatrixFromComponents(
		glm::value_ptr(position),
		glm::value_ptr(rotation),
		glm::value_ptr(scale),
		glm::value_ptr(m_ModelMatrix)
	);
}
