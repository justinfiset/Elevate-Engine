#pragma once

#include <ElevateEngine/Core/Component.h>
#include <ElevateEngine/Core/GameObject.h>
#include <ElevateEngine/Core/TypeRegistry.h>

#include <glm/vec3.hpp>

namespace Elevate
{
	struct ShadowSettings
	{
		BEGIN_STRUCT(ShadowSettings)

		bool CastShadows = true;
		int Resolution = 2048;
		float NearPlane = 0.1f;
		float FarPlane = 100.0f;
		float Bias = 0.005f;

		PROPERTY(CastShadows)
		PROPERTY(Resolution)
		PROPERTY(NearPlane)
		PROPERTY(FarPlane)
		PROPERTY(Bias)
		END_STRUCT()
	};

	struct DirectionalShadowSettings
	{
		BEGIN_STRUCT(DirectionalShadowSettings)

		bool CastShadows = true;
		float OrthographicSize = 50.0f;
		int Resolution = 2048;
		float NearPlane = 0.1f;
		float FarPlane = 100.0f;
		float Bias = 0.005f;

		PROPERTY(CastShadows)
		PROPERTY(OrthographicSize)
		PROPERTY(Resolution)
		PROPERTY(NearPlane)
		PROPERTY(FarPlane)
		PROPERTY(Bias)
		END_STRUCT()
	};

	struct LightAttenuation
	{
		BEGIN_STRUCT(LightAttenuation)

		float Constant = 1.0f;
		float Linear = 0.09f;
		float Quadratic = 0.032f;

		PROPERTY(Constant)
		PROPERTY(Linear)
		PROPERTY(Quadratic)
		END_STRUCT()
	};

	class Light : public Component
	{
	public:
		BEGIN_COMPONENT(Light, EE_HideInInspector)
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

		PROPERTY(m_ambientColor, EE_ColorPicker)
		PROPERTY(m_diffuseColor, EE_ColorPicker)
		PROPERTY(m_specularColor, EE_ColorPicker)
		PROPERTY(m_intensity)
		END_COMPONENT()
	};
}