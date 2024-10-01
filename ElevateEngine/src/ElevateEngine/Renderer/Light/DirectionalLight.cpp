#include "eepch.h"
#include "DirectionalLight.h"

Elevate::DirectionalLight::DirectionalLight()
{
	m_Transform.SetRotation({ 0.0f, 45.0f, 20.0f });
	m_ambientColor = { 0.10f, 0.10f, 0.10f };
	m_diffuseColor = { 0.8f, 0.8f, 0.8f };
	m_specularColor = { 0.9f, 0.9f, 0.9f };
}

const glm::vec3 Elevate::DirectionalLight::CalculateDirection() const
{
	glm::vec3 dir;

	glm::vec3 rotation = m_Transform.GetRotation();
	dir.x = cos(rotation.x) * cos(rotation.y);
	dir.y = sin(rotation.x);
	dir.z = cos(rotation.x) * sin(rotation.y);
	dir = glm::normalize(dir);

	return dir;
}
