#include "eepch.h"
#include "DirectionalLight.h"
#include <glm/geometric.hpp>

#include <ElevateEngine/Core/GameContext.h>

namespace Elevate
{
	REGISTER_COMPONENT(DirectionalLight);

	const glm::vec3 DirectionalLight::CalculateDirection() const
	{
		glm::vec3 dir;

		glm::vec3 rotation = gameObject->GetRotation();
		dir.x = cos(rotation.x) * cos(rotation.y);
		dir.y = sin(rotation.x);
		dir.z = cos(rotation.x) * sin(rotation.y);
		dir = glm::normalize(dir);

		return dir;
	}
}