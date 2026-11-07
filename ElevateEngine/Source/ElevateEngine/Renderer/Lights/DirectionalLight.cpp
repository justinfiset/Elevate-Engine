module;

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/geometric.hpp>

module Elevate.Renderer.Lights.DirectionalLight;

import Elevate.Core.Transform.Transform;
import Elevate.Renderer.Scene.Lighting;

#ifdef EE_EDITOR_BUILD
import Elevate.Renderer.Debug.DebugRenderer;
import Elevate.Editor.Renderer;
import Elevate.Editor.Selection;
#endif

namespace Elevate
{
	void DirectionalLight::Init()
	{
		GetSceneLighting(gameObject->GetScene())->AddDirectionalLight(this);
	}

	void DirectionalLight::Destroy()
	{
		GetSceneLighting(gameObject->GetScene())->RemoveDirectionalLight(this);
	}

	const glm::vec3 DirectionalLight::CalculateDirection() const
	{
		return gameObject->GetTransform().GetForward();
	}

#ifdef EE_EDITOR_BUILD
	void DirectionalLight::Render()
	{
		bool isSelected = Editor::Selection::IsSelected(gameObject);
		glm::vec4 colorMod = isSelected ? glm::vec4(1.5f, 1.5f, 0.5f, 1.0f) : glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		EditorRenderer::DrawBillboard(gameObject->GetGlobalPosition(), GetEditorIcon(), 0.7f, colorMod);
	}

	void DirectionalLight::RenderWhenSelected()
	{
		DrawDebugDirection();
	}

	void DirectionalLight::DrawDebugDirection()
	{
		const auto& transform = gameObject->GetTransform();

		glm::vec3 position = gameObject->GetGlobalPosition();
		glm::vec3 forward = glm::normalize(transform.GetForward());
		glm::vec3 right = glm::normalize(transform.GetRight());
		glm::vec3 up = glm::normalize(transform.GetUp());

		const float rayLength = 3.0f;
		const float circleRadius = 0.6f;
		const int rayCount = 12;

		const glm::vec4 rayColor = glm::vec4(1.0f, 0.85f, 0.1f, 1.0f);

		DebugRenderer::AddDebugLine({ position, position + forward * rayLength, rayColor });

		constexpr float TWO_PI = glm::two_pi<float>();

		for (int i = 0; i < rayCount; i++)
		{
			float angle = TWO_PI * static_cast<float>(i) / rayCount;
			glm::vec3 offset = (right * std::cos(angle) + up * std::sin(angle)) * circleRadius;

			glm::vec3 start = position + offset;
			glm::vec3 end = start + forward * rayLength;

			DebugRenderer::AddDebugLine({ start, end, rayColor });
		}

		for (int i = 0; i < rayCount; i++)
		{
			float angle0 = TWO_PI * static_cast<float>(i) / rayCount;
			float angle1 = TWO_PI * static_cast<float>(i + 1) / rayCount;

			glm::vec3 p0 = position + (right * std::cos(angle0) + up * std::sin(angle0)) * circleRadius;
			glm::vec3 p1 = position + (right * std::cos(angle1) + up * std::sin(angle1)) * circleRadius;

			DebugRenderer::AddDebugLine({ p0, p1, rayColor });
		}
	}

#endif
}