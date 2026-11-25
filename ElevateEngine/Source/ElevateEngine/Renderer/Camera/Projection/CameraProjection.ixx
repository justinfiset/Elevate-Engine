module;

#include <glm/glm.hpp>

#include <cstdint>

#include "Reflection.h"

export module Elevate.Renderer.Camera:CameraProjection;

export namespace Elevate
{
	struct CameraProjection
	{
		BEGIN_STRUCT(CameraProjection);
		using Super = CameraProjection;

	public:
		virtual ~CameraProjection() = default;

		// Near and Far planes
		float GetNear() const
		{
			return m_near;
		}

		void SetNear(float nearPlane)
		{
			m_near = nearPlane;
			m_isDirty = true;
		}

		float GetFar() const
		{
			return m_far;
		}

		void SetFar(float farPlane)
		{
			m_far = farPlane;
			m_isDirty = true;
		}

		std::uint32_t GetWidth() const
		{
			return m_width;
		}

		void SetWidth(std::uint32_t width)
		{
			m_width = width;
			UpdateAspectRatio();
		}

		std::uint32_t GetHeight() const
		{
			return m_height;
		}

		void SetHeight(std::uint32_t height)
		{
			m_height = height;
			UpdateAspectRatio();
		}

		void SetSize(std::uint32_t width, std::uint32_t height)
		{
			m_width = width;
			m_height = height;
			UpdateAspectRatio();
		}

		glm::mat4 GetProjectionMatrix() const
		{
			if (m_isDirty)
			{
				m_projectionMatrix = GenProjectionMatrix();
				m_isDirty = false;
			}
			return m_projectionMatrix;
		}

	protected:
		CameraProjection() = default;

		CameraProjection(std::uint32_t width, std::uint32_t height, float near, float far)
			: m_width(width), m_height(height), m_aspectRatio((float)m_width / (float)m_height), m_near(near), m_far(far),
			m_isDirty(true), m_projectionMatrix(0.0f)
		{
		}

		virtual glm::mat4 GenProjectionMatrix() const = 0;

		float m_near = 0.1f;
		PROPERTY(m_near);

		float m_far = 1000.0f;
		PROPERTY(m_far);

		std::uint32_t m_width = 1280;
		std::uint32_t m_height = 720;
		float m_aspectRatio = (float)m_width / (float)m_height;

		mutable bool m_isDirty = true;

	private:
		void UpdateAspectRatio()
		{
			m_aspectRatio = (float)m_width / (float)m_height;
		}

		mutable glm::mat4 m_projectionMatrix = glm::mat4(0.0f);

		END_STRUCT();
	};
}
