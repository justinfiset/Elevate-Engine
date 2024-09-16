#include "eepch.h"
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

Elevate::Transform::Transform()
{
	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
}

glm::mat4 Elevate::Transform::GetModelMatrix() const
{
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, position);
	model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
	model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
	model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
	model = glm::scale(model, scale);

	return model;
}