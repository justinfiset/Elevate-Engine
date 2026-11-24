module;


#include <ElevateEngine/Core/Scene/SceneEnTTADL.h>
#include <imgui.h>
#include <vector>

export module Elevate.Editor.Panels.ProfilerPanel;

import Elevate.Editor.Widgets.Widget;
import Elevate.Core.Profiling;

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

		void ToggleRecording()
		{
			// todo : impl
		}

		void CaptureFrame()
		{
			m_frames.push_back(Profiler::GetLastFrame());
		}

		void Clear()
		{

		}

		void OnImGuiRender() override
		{
			ImGui::Begin("Profiler");

			DrawToolbar();

			ImGui::End();
		}

		void DrawToolbar()
		{
			if (ImGui::Button(m_isRecording ? "Pause" : "Start"))
			{
				ToggleRecording();
			}

			ImGui::SameLine();
			if (ImGui::Button("Capture Frame"))
			{
				CaptureFrame();
			}

			ImGui::SameLine();
			if (ImGui::Button("Clear"))
			{
				Clear();
			}

			ImGui::Separator();
		}

	private:
		std::vector<const ProfilerFrame*> m_frames;

		bool m_isRecording = false;
	};
}
