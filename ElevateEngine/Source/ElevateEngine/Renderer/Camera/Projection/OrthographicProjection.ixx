module;

#include <cstdint>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Reflection.h>

export module Elevate.Renderer.Camera:OrthographicProjection;

import :CameraProjection;

export namespace Elevate
{
	class OrthographicProjection : public CameraProjection
	{
		BEGIN_STRUCT(OrthographicProjection);
		DECLARE_STRUCT_BASE(CameraProjection);

	public:
		OrthographicProjection() = default;

		OrthographicProjection(std::uint32_t width, std::uint32_t height, float near, float far, float size)
			: CameraProjection(width, height, near, far), m_size(size)
		{
		}

	protected:
		glm::mat4 GenProjectionMatrix() const override
		{
			return glm::ortho(-m_size * m_aspectRatio, m_size * m_aspectRatio, -m_size, m_size, m_near, m_far);
		}

	private:
		float m_size;
		PROPERTY(m_size);

		END_STRUCT();
	};
}
