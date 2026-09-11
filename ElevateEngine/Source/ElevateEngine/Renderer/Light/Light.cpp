#include "eepch.h"
#include "Light.h"

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
