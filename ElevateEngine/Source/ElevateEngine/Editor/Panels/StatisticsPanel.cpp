module;

#include <imgui.h>

module Elevate.Editor.Panels.StatisticsPanel;

void Elevate::Editor::StatisticsPanel::OnImGuiRender()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Begin("Stats");
	ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();
}