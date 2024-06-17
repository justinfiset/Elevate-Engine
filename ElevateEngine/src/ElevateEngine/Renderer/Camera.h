#pragma once

#include "ElevateEngine/Core/Transform.h"
#include "glm/glm.hpp"
#include "ElevateEngine/Core/ITransformable.h"
// TODO setup like an object/gameobject in the future
namespace Elevate
{
	class Camera : public ITransformable
	{
	public:
		Camera(float fov, unsigned int width, unsigned int height) :
			m_FOV(fov), m_Width(width), m_Weight(height) { }

		Camera(Transform transform, float fov, unsigned int width, unsigned int height) : m_FOV(fov), m_Width(width), m_Weight(height) {
			m_Transform = transform;
		}

		glm::mat4 GetViewProjectionMatrix();
		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionMatrix();
	private:
		unsigned int m_Width = 1280;
		unsigned int m_Weight = 720;
		float m_FOV = 60.0f;
	};
}