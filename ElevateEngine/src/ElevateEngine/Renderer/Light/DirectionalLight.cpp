#include "eepch.h"
#include "DirectionalLight.h"

Elevate::DirectionalLight::DirectionalLight()
{
	m_Transform.rotation = { 0.0f, 45.0f, 20.0f };
	m_ambientColor = { 0.10f, 0.10f, 0.10f };
	m_diffuseColor = { 0.8f, 0.8f, 0.8f };
	m_specularColor = { 0.9f, 0.9f, 0.9f };
}

const glm::vec3 Elevate::DirectionalLight::CalculateDirection() const
{
	glm::vec3 dir;

	dir.x = cos(m_Transform.rotation.x) * cos(m_Transform.rotation.y);
	dir.y = sin(m_Transform.rotation.x);
	dir.z = cos(m_Transform.rotation.x) * sin(m_Transform.rotation.y);
	dir = glm::normalize(dir);

	return dir;
}
