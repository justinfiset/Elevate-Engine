module;

#include <string>
#include <algorithm>

#include <imgui.h>

module Elevate.Editor.Widgets.InputField;

namespace Elevate::Editor::Widgets
{
	void InputField(const char* label, std::string& data)
	{
		ImGui::Text("%s", label);
		ImGui::SameLine();

		char buffer[255]{};
		std::copy_n(data.c_str(), std::min(data.size(), sizeof(buffer) - 1), buffer);

		if (ImGui::InputText(label, buffer, sizeof(buffer)))
		{
			data = buffer;
		}
	}

	void InputField(const char* label, const std::string& data)
	{
		ImGui::Text("%s", label);
		ImGui::SameLine();

		char buffer[255]{};
		std::copy_n(data.c_str(), std::min(data.size(), sizeof(buffer) - 1), buffer);

		ImGui::InputText(label, buffer, sizeof(buffer));
	}
}
