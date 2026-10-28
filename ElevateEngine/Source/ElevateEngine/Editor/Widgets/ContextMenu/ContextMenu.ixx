module;

#include <imgui.h>

export module Elevate.Editor.Widgets.ContextMenu;

export namespace Elevate::Editor::Widgets
{
	class ContextMenu
	{
	public:
		static bool Begin(const char* id, ImGuiWindowFlags flags = 0)
		{
			PushStyle();
			if (ImGui::BeginPopup(id, flags))
			{
				return true;
			}

			PopStyle();
			return false;
		}

		static bool BeginOnItem(const char* id, ImGuiPopupFlags flags = ImGuiPopupFlags_MouseButtonRight)
		{
			PushStyle();
			if (ImGui::BeginPopupContextItem(id, flags))
			{
				return true;
			}

			PopStyle();
			return false;
		}

		static void End()
		{
			ImGui::EndPopup();
			PopStyle();
		}

	private:
		static constexpr float kRowHeight = 22.0f;
		static constexpr float kSpacingY = 5.0f;

		static void PushStyle()
		{
			ImGui::PushFont(nullptr, kRowHeight - kSpacingY);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6.0f, 6.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 9.0f));
		}

		static void PopStyle()
		{
			ImGui::PopStyleVar(2);
			ImGui::PopFont();
		}
	};
}
