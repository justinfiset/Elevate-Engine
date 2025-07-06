#pragma once
#include <memory>
#include <glm/vec3.hpp>
#include <ElevateEngine/Core/Component.h>

namespace Elevate
{
	class Shader;
}

namespace Elevate
{
	class Light : public Component
	{
	public:
		COMPONENT_LAYOUT(
			{
				{ "Intensity", ComponentDataType::Float, &m_intensity},
				{ "Ambient Color", ComponentDataType::Float3, &m_ambientColor},
				{ "Diffuse Color", ComponentDataType::Float3, &m_diffuseColor},
				{ "Specular Color", ComponentDataType::Float3, &m_specularColor},
			}
		);

		Light(const glm::vec3& color)
			: m_ambientColor(color), m_diffuseColor(color), m_specularColor(color) { }

		Light(const glm::vec3& amb, const glm::vec3& dif, const glm::vec3& spec)
			: m_ambientColor(amb), m_diffuseColor(dif), m_specularColor(spec) { }

		// Getter and setters
		inline void SetColor(const glm::vec3& color)
		{
			m_ambientColor = color;
			m_diffuseColor = color;
			m_specularColor = color;
		}
		inline void SetAmbientColor(const glm::vec3& color) { m_ambientColor = color; }
		inline void SetDiffuseColor(const glm::vec3& color) { m_diffuseColor = color; }
		inline void SetSpecularColor(const glm::vec3& color) { m_specularColor = color; }
		inline void SetIntensity(const float& intensity) { m_intensity = intensity; }

		inline const glm::vec3& GetAmbientColor() const { return m_ambientColor; }
		inline const glm::vec3& GetDiffuseColor() const { return m_diffuseColor; }
		inline const glm::vec3& GetSpecularColor() const { return m_specularColor; }
		inline const float& GetIntensity() const { return m_intensity; }
	protected:
		glm::vec3 m_ambientColor;
		glm::vec3 m_diffuseColor;
		glm::vec3 m_specularColor;
		float m_intensity = 1.0f;
	};
}