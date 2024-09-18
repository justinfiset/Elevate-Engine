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

glm::mat4 Elevate::Transform::GetModelMatrix() const
{
	glm::mat4 model = glm::mat4(1.0f);

	ImGuizmo::RecomposeMatrixFromComponents(
		glm::value_ptr(position),
		glm::value_ptr(rotation),
		glm::value_ptr(scale),
		glm::value_ptr(model)
	);

	return model;
}