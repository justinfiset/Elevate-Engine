module;

#include <imgui.h>

export module Elevate.Editor.Panels.ProfilerPanel;

import Elevate.Editor.Widgets.Widget;

export namespace Elevate::Editor
{
	class ProfilerPanel : public Widget
	{
	public:
		ProfilerPanel()
		{
		}

		virtual ~ProfilerPanel()
		{
		}

		void OnImGuiRender() override
		{
			DrawToolbar();
		}

		void DrawToolbar()
		{
			ImGui::Begin("Profiler");

			ImGui::Button(m_isRecording ? "Pause" : "Start");
			ImGui::SameLine();
			ImGui::Button("Capture Frame");
			ImGui::SameLine();
			ImGui::Button("Clear");
			ImGui::Separator();

			ImGui::End();
		}

	private:
		bool m_isRecording = false;
	};
}
