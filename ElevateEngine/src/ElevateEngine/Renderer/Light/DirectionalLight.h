#pragma once
#include "Light.h"

namespace Elevate
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight();

		const void Use(std::shared_ptr<Shader> shader) override;
	private:
		const glm::vec3 CalculateDirection() const;
	};
}