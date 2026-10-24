module;

#include "glm/glm.hpp"

#include "Reflection.h"

export module Elevate.Renderer.Camera:Camera;

import Elevate.Foundations;
import Elevate.Core.Scene;

// TODO add ortographic or projection settings (bool value or constructors)
export namespace Elevate
{
	class Camera : public Component
	{
	public:
		BEGIN_COMPONENT(Camera, EditorIconTag(Editor::Icons::COMPONENT_ICON_CAMERA));
		EECATEGORY("Rendering");

		// All possible constructors
		Camera(float fov = 60.0f, bool overrideCurrent = true);
		Camera(float fov, float aspectRatio, bool overrideCurrent = true);

		void UpdateAspectRatio(float aspectRatio);

		inline const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
		glm::mat4 GenViewMatrix() const;

		glm::vec3 GetFrontVec() const;
		glm::vec3 GetRightVec() const;
		glm::vec3 GetUpVec() const;

		inline float GetFOV() const { return m_FOV; }
		inline void SetFOV(float fov);

		// Near and Far planes
		inline float GetNear() const { return m_near; }
		inline void SetNear(float nearPlane);

		inline float GetFar() const { return m_far; }
		inline void SetFar(float farPlane);

		inline float GetAspectRatio() const { return m_aspectRatio; }

		glm::mat4 GenViewProjectionMatrix() const;

		void Init() override;
		void Destroy() override;

		std::array<glm::vec3, 8> CalculateFrustumCorners(float nearScale, float farScale) const;
		std::array<glm::vec3, 8> CalculateFrustumCorners() const;

#ifdef EE_EDITOR_BUILD
		void Render() override;
		void RenderWhenSelected() override;
		void DrawDebugFrustum();
#endif

	private:
		glm::mat4 GenProjectionMatrix();
		void UpdateProjectionMatrix();

	private:
		float m_aspectRatio;

		float m_FOV;
		PROPERTY(m_FOV);

		glm::mat4 m_projectionMatrix;

		bool m_canBeMainCamera = true;

		float m_near = 0.1f;
		PROPERTY(m_near);

		float m_far = 50.0f;
		PROPERTY(m_far);

		END_COMPONENT();
	};
}