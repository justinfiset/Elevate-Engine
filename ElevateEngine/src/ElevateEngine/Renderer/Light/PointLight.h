#pragma once
#include "Light.h"

namespace Elevate
{
	class PointLight : public Light
	{
	public:
		const void Use(std::shared_ptr<Shader> shader);
	private:

	};
}