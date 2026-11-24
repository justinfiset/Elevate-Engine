module;

#include <cfloat>
#include <cmath>

// EnTT iterator operators must be visible in the global module when inline
// Scene code is instantiated here; they otherwise stay attached to a module
// BMI and are not found by ADL.
#include <entt/entt.hpp>

module Elevate.Renderer.Lights.Light;

namespace Elevate
{
	float LightAttenuation::CalculateLightRange(float cutOff)
	{
		float target = 1.0f / cutOff;

		if (Quadratic <= 0.0f)
		{
			if (Linear < +0.0f)
			{
				return FLT_MAX;
			}
			return (target - Constant) / Linear;
		}

		float discriminant = Linear * Linear - 4.0f * Quadratic * (Constant - target);
		if (discriminant < 0.0f)
		{
			return 0.0f;
		}

		return (-Linear + std::sqrt(discriminant)) / (2.0f * Quadratic);
	}
}
