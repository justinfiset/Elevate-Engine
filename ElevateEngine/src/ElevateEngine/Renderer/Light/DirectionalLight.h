#pragma once
#include "Light.h"

namespace Elevate
{	
	class DirectionalLight : public Light
	{
	public:
		BEGIN_COMPONENT(DirectionalLight)

		DirectionalLight() = default;
		DirectionalLight(const glm::vec3& color) : Light(color) { }

		const glm::vec3 CalculateDirection() const;

		END_COMPONENT()
	};
}