#include "WwiseBrowserWidget.h"

#include <ElevateEngine/Inputs/Input.h>
#include <ElevateEngine/Audio/Wwise/WwiseSoundEngine.h>
#include <ElevateEngine/Audio/Wwise/WAAPI/WAAPIClient.h>
#include "imgui.h"

namespace Elevate::Editor
{
    void WwiseBrowserWidget::OnImGuiRender()
    {
        // todo : prevent from being fetched each frame and only update when there is a refresh.
        WwiseSoundEngine* wwiseEngine = WwiseSoundEngine::Get();

        ImGui::Begin("Wwise Browser");

        ImGui::BeginGroup();
        ImGui::Text(WAAPIClient::IsConnected() ? "Conntected to WAAPI." : "Not Connected to WAAPI");
        ImGui::EndGroup();

        ImGui::BeginGroup();
        if (ImGui::Button("Expand All"))
        {
        }
        ImGui::SameLine();
        if (ImGui::Button("Collapse All"))
        {
        }
        ImGui::SameLine();
        ImGui::InputText("##", m_searchBuffer, sizeof(m_searchBuffer) / sizeof(char));
        ImGui::SameLine();
        if (ImGui::Button("Refresh"))
        {
            wwiseEngine->GetDataSource()->RefreshSource();
            // todo add a notifier here
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Button("Generate SoundBanks"))
        {
        }
        ImGui::EndGroup();
        ImGui::Separator();
        ImGui::BeginTable("WwiseBrowserTreeView", 3, ImGuiTableFlags_Resizable);
        ImGui::TableSetupColumn("Wwise Item");
        ImGui::TableSetupColumn("State on Disk");
        ImGui::TableSetupColumn("State in SoundBanks");
        ImGui::TableHeadersRow();

        DrawTreeView(wwiseEngine->GetDataSource()->GetItems().lock());

        ImGui::EndTable();
        ImGui::End();
    }

    void WwiseBrowserWidget::DrawTreeView(WwiseItemPtr root)
    {
        for (const auto& child : root->Children)
        {
            DrawItemsRecursive(child);
        }
    }

    void WwiseBrowserWidget::DrawItemsRecursive(WwiseItemPtr item)
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);

        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
        if (!item->HasChildren())
        {
            nodeFlags |= ImGuiTreeNodeFlags_Leaf;
        }

        if (m_selectedItems.contains(item))
        {
            nodeFlags |= ImGuiTreeNodeFlags_Selected;
        }

        bool open = false;
        // todo impl.
        //ImGui::Image((ImTextureID)m_objectTexture->GetNativeHandle(), ImVec2(16, 16));
        //ImGui::SameLine();
        open = ImGui::TreeNodeEx((item->Name + "##" + item->GUID).c_str(), nodeFlags);

        if (ImGui::BeginPopupContextItem())
        {
            ImGui::SeparatorText("Playback");
            ImGui::BeginDisabled(item->ShortID == 0 || !item->IsEvent());
            if (ImGui::MenuItem("Play Event"))
            {
                SoundEngine::PostEvent(item->ShortID);
            }
            ImGui::EndDisabled();
            ImGui::EndPopup();
        }

        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            if (!ImGui::IsKeyDown(ImGuiKey::ImGuiKey_LeftCtrl))
            {
                m_selectedItems.clear();
            }
            m_selectedItems.insert(item);
        }

        if (open)
        {
            for (const auto& child : item->Children)
            {
                DrawItemsRecursive(child);
            }
            ImGui::TreePop();
        }

        // We do not draw in the other columns if the item is a parent
        if (item->IsDirectory())
        {
            return;
        }

        ImGui::TableSetColumnIndex(1);
        EditorMessage msg = GetDiskStatusText(item);
        glm::vec4 color = msg.GetColor();
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(color.r, color.g, color.b, color.a));
        ImGui::Text(msg.message.c_str());
        ImGui::PopStyleColor();

        ImGui::TableSetColumnIndex(2);
        msg = GetSoundBanksStatusText(item);
        color = msg.GetColor();
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(color.r, color.g, color.b, color.a));
        ImGui::Text(msg.message.c_str());
        ImGui::PopStyleColor();
    }

    EditorMessage WwiseBrowserWidget::GetDiskStatusText(WwiseItemPtr item)
    {
        if (item->IsOnDisk)
        {
            return EditorMessage("On Disk", EditorMessage::Type::Message);
        }
        else
        {
            return EditorMessage("Not on Disk", EditorMessage::Type::Warning);
        }
    }

    EditorMessage WwiseBrowserWidget::GetSoundBanksStatusText(WwiseItemPtr item)
    {
        if (item->IsInBank)
        {
            return EditorMessage("SoundBank up to date", EditorMessage::Type::Message);
        }
        else
        {
            return EditorMessage("Missing in SoundBank", EditorMessage::Type::Warning);
        }
    }
}
