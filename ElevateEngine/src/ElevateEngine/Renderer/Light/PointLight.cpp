#include "eepch.h"
#include "PointLight.h"

const void Elevate::PointLight::Use(std::shared_ptr<Shader> shader)
{
	Light::Use(shader);
}
