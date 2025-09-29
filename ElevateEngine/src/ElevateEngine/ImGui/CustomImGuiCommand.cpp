#include "eepch.h"
#include "CustomImGuiCommand.h"

#include <imgui.h>
#include <imgui_internal.h>

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

bool Elevate::UI::EECollapsingHeader(const char* label, bool canOpen, std::function<float()> headerCustomContent)
{
    using namespace ImGui;

    ImGuiStyle& style = GetStyle();
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiID id = window->GetID(label);

    float btnWidth;
    if (headerCustomContent)
    {
        SetItemAllowOverlap();
        ImVec2 prevCursor = GetCursorScreenPos();
        btnWidth = ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - headerCustomContent() - style.FramePadding.x;

        SetCursorScreenPos(prevCursor);
    }

    bool* p_open = GetStateStorage()->GetBoolRef(id, true);

    std::string btn_id = std::string("##EECollapsingHeaderBtn_") + label;
    bool clicked = Button(btn_id.c_str(), ImVec2(btnWidth, 0.0f));
    if (clicked)
        *p_open = !*p_open;

    ImVec2 r_min = GetItemRectMin();
    ImVec2 r_max = GetItemRectMax();
    float btn_height = r_max.y - r_min.y;

    float arrow_size = 7.0f;
    ImVec2 arrow_pos = ImVec2(
        r_min.x + arrow_size * 0.5f,
        r_min.y + style.FramePadding.y
    );

    ImVec2 text_pos = ImVec2(
        r_min.x + style.FramePadding.x + arrow_size * 2 + style.ItemInnerSpacing.x,
        r_min.y + (btn_height - GetFontSize()) * 0.5f
    );

    ImDrawList* dl = window->DrawList;

    if (canOpen)
        RenderArrow(dl, arrow_pos, GetColorU32(ImGuiCol_Text),
            *p_open ? ImGuiDir_Down : ImGuiDir_Right, 1.0f);

    dl->AddText(GetFont(), GetFontSize(), text_pos, GetColorU32(ImGuiCol_Text), label);

    return *p_open;
}