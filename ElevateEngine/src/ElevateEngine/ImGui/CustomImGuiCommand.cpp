#include "eepch.h"
#include "CustomImGuiCommand.h"
#include "imgui.h"

void Elevate::UI::InputField(char* label, std::string& data)
{
    ImGui::Text(label);
    ImGui::SameLine();
    ImGui::InputText(" ", data.data(), 255);
}
