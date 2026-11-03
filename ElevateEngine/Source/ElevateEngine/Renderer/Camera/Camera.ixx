module;

#include <memory>
#include <array>
#include "glm/glm.hpp"

#include "Reflection.h"

export module Elevate.Renderer.Camera:Camera;

import Elevate.Foundations;
import Elevate.Core.Scene;

import :CameraProjectionMode;
import :CameraProjection;

export namespace Elevate
{
	class Camera : public Component
	{
	public:
		BEGIN_COMPONENT(Camera, EditorIconTag(Editor::Icons::COMPONENT_ICON_CAMERA));
		EECATEGORY("Rendering");

		// All possible constructors
		Camera(CameraProjectionMode projection = CameraProjectionMode::Perspective, bool overrideCurrent = true);
		~Camera() = default;

		CameraProjection* GetProjection() const
		{
			return m_projection.get();
		}

		glm::mat4 GenViewMatrix() const;
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
		void CreateProjection();

	private:
		bool m_canBeMainCamera = true;

		CameraProjectionMode m_projectionMode = CameraProjectionMode::Perspective;
		CameraProjectionMode m_previousMode; // Used to check if dirty

		std::unique_ptr<CameraProjection> m_projection;
		PROPERTY(m_projection);

		END_COMPONENT();
	};
}