#pragma once

#include <ElevateEngine/Core/Component.h>
#include <ElevateEngine/Core/GameObject.h>
#include <ElevateEngine/Core/ComponentRegistry.h>

#include <glm/vec3.hpp>

namespace Elevate
{
	class Light : public Component
	{
	public:
		BEGIN_COMPONENT(Light, HideInInspector)
		EECATEGORY("Rendering")

		Light() = default;

		Light(const glm::vec3& color)
			: m_ambientColor(color), m_diffuseColor(color), m_specularColor(color) { }

		Light(const glm::vec3& amb, const glm::vec3& dif, const glm::vec3& spec, float intensity = 1.0f)
			: m_ambientColor(amb), m_diffuseColor(dif), m_specularColor(spec), m_intensity(intensity) { }

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

		//virtual Component* Clone() override;
	protected:
		glm::vec3 m_ambientColor = { 1.0f, 1.0f, 1.0f };
		glm::vec3 m_diffuseColor = { 1.0f, 1.0f, 1.0f };
		glm::vec3 m_specularColor = { 1.0f, 1.0f, 1.0f };
		float m_intensity = 1.0f;

		EXPOSE(m_ambientColor, Color)
		EXPOSE(m_diffuseColor, Color)
		EXPOSE(m_specularColor, Color)
		EXPOSE(m_intensity)
		END_COMPONENT()
	};
}