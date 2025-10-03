#pragma once
#include "ElevateEngine/Core/GameObject.h"
#include "ElevateEngine/Core/Component.h"
#include "ElevateEngine/Core/ComponentRegistry.h"

#include "glm/glm.hpp"

// TODO add ortographic or projection settings (bool value or constructors)
namespace Elevate
{
	class Camera : public Component
	{
	public:
		BEGIN_COMPONENT(Camera)
		EECATEGORY("Rendering")

		// All possible constructors
		Camera(float fov = 60.0f, bool overrideCurrent = true);
		Camera(float fov, float aspectRatio, bool overrideCurrent = true);

		const void UpdateAspectRatio(float aspectRatio);

		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix;  }
		glm::mat4 GenViewMatrix();

		inline const glm::vec3& GetFrontVec() const { return m_Front; }
		inline const glm::vec3& GetRightVec() const { return m_Right; }
		inline const glm::vec3& GetUpVec() const { return m_Up; }
		
		inline const float GetFOV() const { return m_FOV; }
		inline void SetFOV(float fov);

		// Near and Far planes
		inline const float GetNear() const { return m_Near; }
		inline void SetNear(float nearPlane);

		inline const float GetFar() const { return m_Far; }
		inline void SetFar(float farPlane);

		inline const float GetAspectRatio() const { return m_AspectRatio; }

		void UpdateCameraVectors();

		glm::mat4 GenViewProjectionMatrix();

		//virtual Component* Clone() override;

		void Init() override;
	private:
		glm::mat4 GenProjectionMatrix();
		void UpdateProjectionMatrix();

	private:
		float m_AspectRatio;
		float m_FOV;

		glm::vec3 m_Front;
		glm::vec3 m_Right;
		glm::vec3 m_Up;
		glm::mat4 m_ProjectionMatrix;

		float m_Near = 0.1f;
		float m_Far = 100.0f;

		END_COMPONENT()
	};
	using CameraPtr = std::shared_ptr<Camera>;
}