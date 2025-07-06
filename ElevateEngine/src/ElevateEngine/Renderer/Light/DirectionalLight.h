#pragma once
#include "Light.h"

namespace Elevate
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight(const glm::vec3& color) : Light(color) { }

		const glm::vec3 CalculateDirection() const;
	};
}