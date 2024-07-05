#pragma once

#include "ElevateEngine/Core/Transform.h"
#include "glm/glm.hpp"
#include "ElevateEngine/Core/ITransformable.h"


// TODO setup like an object/gameobject or component in the future
namespace Elevate
{
	class Camera : public ITransformable
	{
	public:
		// All possible constructors
		Camera(float fov = 60.0f);
		Camera(float fov, float aspectRatio);
		Camera(Transform transform, float fov);
		Camera(Transform transform, float fov, float aspectRatio);

		glm::mat4& GetProjectionMatrix() { return m_projectionMatrix;  }
		glm::mat4 GenViewMatrix();

		glm::mat4 GenViewProjectionMatrix();
		//glm::vec3 GetUp();
	private:
		glm::mat4 GenProjectionMatrix();

	public: // TODO IS TEMPORARY
		void UpdateCameraVectors();
	private:
		float m_AspectRatio;
		float m_FOV;


	// TODO set private ASAP
	public:
		glm::vec3 m_front;
		glm::vec3 m_right;
		glm::vec3 m_up;
		glm::mat4 m_projectionMatrix;
	};
}