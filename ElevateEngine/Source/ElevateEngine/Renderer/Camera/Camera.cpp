module;

#include <memory>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

module Elevate.Renderer.Camera;

import Elevate.Foundations.CoreLogger;

import Elevate.Core.Transform;
import Elevate.Core.Window;
import Elevate.Core.Application;
import Elevate.Core.Scene;
import Elevate.Audio.SoundEngine;

#ifdef EE_EDITOR_BUILD
import Elevate.Editor.Renderer;
import Elevate.Editor.Selection;
import Elevate.Renderer.Debug.DebugRenderer;
#endif

import :CameraProjectionMode;
import :PerspectiveProjection;
import :OrthographicProjection;

Elevate::Camera::Camera(CameraProjectionMode projection, bool overrideCurrent)
	: m_projectionMode(projection), m_canBeMainCamera(overrideCurrent), m_projectionWatcher(m_projectionMode)
{
}

void Elevate::Camera::Init()
{
	CreateProjection();

	if (m_canBeMainCamera)
	{
		CameraManager::SetCurrent(this);
	}

	// todo : change this bit of code to be in the camera manager
	// or let the scene manager handle it
	SoundEngine::SetDefaultListener(gameObject);
}

void Elevate::Camera::Destroy()
{
	CameraManager::NotifyDestruction(this);
}

glm::mat4 Elevate::Camera::GenViewMatrix() const
{
	Transform& transform = gameObject->GetTransform();
	return glm::lookAt(transform.GetPosition(), transform.GetPosition() + transform.GetForward(), transform.GetUp());
}

glm::mat4 Elevate::Camera::GenViewProjectionMatrix() const
{
	return m_projection->GetProjectionMatrix() * GenViewMatrix();
}

void Elevate::Camera::CreateProjection()
{
	// Create the new projection object
	switch (m_projectionMode)
	{
	case CameraProjectionMode::Perspective:
		m_projection = std::make_unique<PerspectiveProjection>();
		break;
	case CameraProjectionMode::Orthographic:
		m_projection = std::make_unique<OrthographicProjection>();
		break;
	default:
		CoreLogger::Warn("Cannot create a camera without a valid projection.");
		return;
	}

	// Set the width and height the new object
	Window& window = Application::Get().GetWindow();
	m_projection->SetSize(window.GetWidth(), window.GetHeight());
}

std::array<glm::vec3, 8> Elevate::Camera::CalculateFrustumCorners(float nearScale, float farScale) const
{
	std::array<glm::vec3, 8> corners;

	const glm::mat4 inverseViewProj = glm::inverse(GenViewProjectionMatrix());

	std::array<glm::vec3, 8> ndcCorners;
	ndcCorners[0] = { -1.f, -1.f, -1.f };
	ndcCorners[1] = { 1.f, -1.f, -1.f };
	ndcCorners[2] = { 1.f,  1.f, -1.f };
	ndcCorners[3] = { -1.f,  1.f, -1.f };
	ndcCorners[4] = { -1.f, -1.f,  1.f };
	ndcCorners[5] = { 1.f, -1.f,  1.f };
	ndcCorners[6] = { 1.f,  1.f,  1.f };
	ndcCorners[7] = { -1.f,  1.f,  1.f };

	for (std::size_t i = 0; i < ndcCorners.size(); ++i)
	{
		glm::vec4 worldCorner = inverseViewProj * glm::vec4(ndcCorners[i], 1.0f);
		worldCorner /= worldCorner.w;

		corners[i] = glm::vec3(worldCorner);
	}

	for (std::size_t i = 0; i < 4; ++i)
	{
		const glm::vec3 nearCorner = corners[i];
		const glm::vec3 farCorner = corners[i + 4];

		const glm::vec3 direction = farCorner - nearCorner;

		corners[i] = nearCorner + direction * nearScale;
		corners[i + 4] = nearCorner + direction * farScale;
	}

	return corners;
}

std::array<glm::vec3, 8> Elevate::Camera::CalculateFrustumCorners() const
{
	return CalculateFrustumCorners(0.0f, 1.0f);
}

// Only in the editor
#ifdef EE_EDITOR_BUILD

void Elevate::Camera::RenderWhenSelected()
{
	DrawDebugFrustum();
}

void Elevate::Camera::Render()
{
	m_projectionWatcher.HasChanged(m_projectionMode, [this](auto, auto)
		{
			CreateProjection();
		});

	bool isSelected = Editor::Selection::IsSelected(gameObject);
	glm::vec4 colorMod = isSelected ? glm::vec4(1.5f, 1.5f, 0.5f, 1.0f)
		: glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	EditorRenderer::DrawBillboard(gameObject->GetGlobalPosition(), GetEditorIcon(), 0.5f, colorMod);
}

void Elevate::Camera::DrawDebugFrustum()
{
	auto corners = CalculateFrustumCorners();

	glm::vec4 color = glm::vec4(0.96f, 0.47f, 0.12f, 1.0f);

	// Draw the lines from the near corners to the far corners
	for (int i = 0; i < 4; i++)
	{
		DebugRenderer::AddDebugLine({ corners[i], corners[i + 4], color });
	}

	// Draw near plane
	DebugRenderer::AddDebugLine({ corners[0], corners[1], color });
	DebugRenderer::AddDebugLine({ corners[1], corners[2], color });
	DebugRenderer::AddDebugLine({ corners[2], corners[3], color });
	DebugRenderer::AddDebugLine({ corners[3], corners[0], color });

	// Draw far plane
	DebugRenderer::AddDebugLine({ corners[4], corners[5], color });
	DebugRenderer::AddDebugLine({ corners[5], corners[6], color });
	DebugRenderer::AddDebugLine({ corners[6], corners[7], color });
	DebugRenderer::AddDebugLine({ corners[7], corners[4], color });
}
#endif