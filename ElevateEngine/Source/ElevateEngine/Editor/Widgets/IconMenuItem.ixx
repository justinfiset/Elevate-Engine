module;

#include <imgui.h>

export module Elevate.Editor.Widgets.IconMenuItem;

export namespace Elevate::Editor::Widgets
{
	bool IconMenuItem(const char* label, ImTextureID icon, bool enabled = true)
	{
		const float iconSize = ImGui::GetFontSize();

		ImGui::BeginDisabled(!enabled);

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.0f);
		ImGui::Image(icon, ImVec2(iconSize, iconSize));
		ImGui::SameLine(0.0f, 8.0f);

		bool clicked = false;
		if (ImGui::MenuItem(label))
		{
			clicked = true;
		}

		ImGui::EndDisabled();

		return clicked;
	}
}