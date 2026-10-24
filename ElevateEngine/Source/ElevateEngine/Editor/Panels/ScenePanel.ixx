module;

#include <string>

#include <glm/glm.hpp> // iwyu: keep

export module Elevate.Editor.Panels.ScenePanel;

#ifdef EE_EDITOR_BUILD

import Elevate.Editor.Widgets.Widget;

export namespace Elevate::Editor
{
	class EditorLayer;

	class ScenePanel : public Widget
	{
	public:
		void OnImGuiRender() override;

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

#endif // EE_EDITOR_BUILD