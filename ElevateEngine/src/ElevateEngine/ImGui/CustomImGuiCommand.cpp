#include "eepch.h"
#include "CustomImGuiCommand.h"
#include "imgui.h"

void Elevate::UI::InputField(const char* label, std::string& data)
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
