module;

#include <string>

#include <imgui.h>
#include <imgui_internal.h>

module Elevate.Editor.Widgets.InputField;

namespace Elevate::Editor::Widgets
{
	void InputField(const char* label, std::string& data)
	{
		ImGui::Text("%s", label);
		ImGui::SameLine();

		char buffer[255];
		strncpy(buffer, data.c_str(), sizeof(buffer));
		buffer[sizeof(buffer) - 1] = '\0';

		if (ImGui::InputText("##InputField", buffer, sizeof(buffer)))
		{
			data = buffer;
		}
	}

	void InputField(const char* label, const std::string& data)
	{
		ImGui::Text("%s", label);
		ImGui::SameLine();

		char buffer[255];
		strncpy(buffer, data.c_str(), sizeof(buffer));
		buffer[sizeof(buffer) - 1] = '\0';

		ImGui::InputText("##InputField", buffer, sizeof(buffer));
	}
}
