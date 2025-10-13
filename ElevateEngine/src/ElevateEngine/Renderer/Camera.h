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
		BEGIN_COMPONENT(Camera, EditorIcon("./editor/icons/light/components/camera.png"))
		EECATEGORY("Rendering")

		// All possible constructors
		Camera(float fov = 60.0f, bool overrideCurrent = true);
		Camera(float fov, float aspectRatio, bool overrideCurrent = true);

		const void UpdateAspectRatio(float aspectRatio);

		inline const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix;  }
		glm::mat4 GenViewMatrix();

		inline const glm::vec3& GetFrontVec() const { return m_front; }
		inline const glm::vec3& GetRightVec() const { return m_right; }
		inline const glm::vec3& GetUpVec() const { return m_up; }

		inline const float GetFOV() const { return m_FOV; }
		inline void SetFOV(float fov);

		// Near and Far planes
		inline const float GetNear() const { return m_near; }
		inline void SetNear(float nearPlane);

		inline const float GetFar() const { return m_far; }
		inline void SetFar(float farPlane);

		inline const float GetAspectRatio() const { return m_aspectRatio; }

		void UpdateCameraVectors();

		glm::mat4 GenViewProjectionMatrix();

		void Init() override;
		void Destroy() override;

#ifdef EE_EDITOR_BUILD
		void RenderWhenSelected() override;
		void DrawDebugFrustum();
		std::array<glm::vec3, 8> CalculateFrustumCorners(float visualFarScale = 0.5f);
#endif

	private:
		glm::mat4 GenProjectionMatrix();
		void UpdateProjectionMatrix();

	private:
		float m_aspectRatio;

		float m_FOV;
		EXPOSE(m_FOV)

		glm::vec3 m_front;
		glm::vec3 m_right;
		glm::vec3 m_up;
		glm::mat4 m_projectionMatrix;

		bool m_canBeMainCamera = true;

		float m_near = 0.1f;
		EXPOSE(m_near)

		float m_far = 100.0f;
		EXPOSE(m_far)

		END_COMPONENT()
	};
	using CameraPtr = std::shared_ptr<Camera>;
}