#pragma once

#include <glm/glm.hpp>

namespace Elevate::Editor 
{
	class EditorLayer;

	class ScenePanel
	{
	public:
		ScenePanel() = default;

		void OnImGuiRender();

	private:
		void UpdateViewportAspectRatio();
		std::string GetAspectRatioText(glm::ivec2 ar);
	private:

		// Editor tool option
		int m_CurrentEditorTool = 7;

		// Aspect Ratio Selection
		int m_AspectRatioValue = 3;

		const glm::ivec2 s_AspectRatioSettings[5] =
		{
			{ 3, 2 },
			{ 4, 3 },
			{ 5, 4 },
			{ 16, 9 },
			{ 16, 10 }
		};
	};
}