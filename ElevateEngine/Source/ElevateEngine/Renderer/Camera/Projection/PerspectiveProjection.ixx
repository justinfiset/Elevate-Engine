module;

#include <cstdint>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Reflection.h>

export module Elevate.Renderer.Camera:PerspectiveProjection;

import :CameraProjection;

export namespace Elevate
{
	class PerspectiveProjection : public CameraProjection
	{
		BEGIN_STRUCT(PerspectiveProjection);
		DECLARE_STRUCT_BASE(CameraProjection);

	public:
		PerspectiveProjection() = default;

		PerspectiveProjection(std::uint32_t width, std::uint32_t height, float near, float far, float fov)
			: CameraProjection(width, height, near, far), m_FOV(fov)
		{
		}

		float GetFOV() const
		{
			return m_FOV;
		}

		void SetFOV(float fov)
		{
			m_FOV = fov;
			m_isDirty = true;
		}

	protected:
		glm::mat4 GenProjectionMatrix() const override
		{
			return glm::perspective(glm::radians(m_FOV), m_aspectRatio, m_near, m_far);
		}

	private:
		float m_FOV = 60.0f;
		PROPERTY(m_FOV);

		END_STRUCT();
	};
}
