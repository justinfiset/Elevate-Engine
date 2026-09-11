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
		float ZPadding = 10.0f;

		PROPERTY(CastShadows)
		PROPERTY(Resolution)
		PROPERTY(ZPadding)
		END_STRUCT()
	};

	struct DirectionalShadowSettings
	{
		BEGIN_STRUCT(DirectionalShadowSettings)

		bool CastShadows = true;
		int Resolution = 2048;
		float ZPadding = 10.0f;
		float FarPlane = 100.0f;

		PROPERTY(CastShadows)
		PROPERTY(Resolution)
		PROPERTY(ZPadding)
		END_STRUCT()
	};

	struct LightAttenuation
	{
		BEGIN_STRUCT(LightAttenuation)

		float Constant = 1.0f;
		float Linear = 0.09f;
		float Quadratic = 0.032f;

		float CalculateLightRange(float cutOff = 0.01f);

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

		Light(const glm::vec3& color, float intensity = 1.0f)
			: m_color(color), m_intensity(intensity) { }

		// Getter and setters
		inline void SetColor(const glm::vec3& color) { m_color = color; }
		inline void SetIntensity(float intensity) { m_intensity = intensity; }
		inline const glm::vec3& GetColor() const { return m_color; }
		inline const float& GetIntensity() const { return m_intensity; }

		//virtual Component* Clone() override;
	protected:
		glm::vec3 m_color = { 1.0f, 1.0f, 1.0f };
		PROPERTY(m_color, EE_ColorPicker)

		float m_intensity = 1.0f;
		PROPERTY(m_intensity)
				
		END_COMPONENT()
	};
}