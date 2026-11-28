module;

#include <cstdint>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

export module Elevate.Renderer.Lights.Contract;

export import Elevate.Renderer.Lights.Light;
export import Elevate.Renderer.Shaders;

export namespace Elevate
{
	class IDirectionalLight
	{
	public:
		virtual ~IDirectionalLight() = default;

		virtual const Light& GetLight() const = 0;
		virtual glm::vec3 CalculateDirection() const = 0;
		virtual const DirectionalShadowSettings& GetShadowSettings() const = 0;
		virtual glm::mat4 GetWorldMatrix() const = 0;
	};

	class IPointLight
	{
	public:
		virtual ~IPointLight() = default;

		virtual void UploadToShader(Shader* shader, uint32_t index) const = 0;
	};

	class ISpotLight
	{
	public:
		virtual ~ISpotLight() = default;

		virtual void UploadToShader(Shader* shader, uint32_t index) const = 0;
	};
}
