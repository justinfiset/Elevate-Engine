#include "eepch.h"
#include "HierarchyPanel.h"

#include "imgui.h"
#include <ElevateEngine/Editor/EditorLayer.h>
#include <ElevateEngine/Scene/SceneManager.h>

namespace Elevate::Editor
{
    void HierarchyPanel::DrawTreeHierarchy(std::shared_ptr<GameObject> object)
    {
        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;

        if (!object->HasChild())
        {
            nodeFlags |= ImGuiTreeNodeFlags_Leaf;
        }

        if (object == EditorLayer::Get().GetSelectedObject().lock())
        {
            nodeFlags |= ImGuiTreeNodeFlags_Selected;
        }

        bool open = false;
        if (m_renaming && object == m_renamedObject) 
        {
            ImGui::PushID(object.get());
            open = ImGui::TreeNodeEx(("##" + std::to_string((uintptr_t)object.get())).c_str(), nodeFlags);

            ImGui::SameLine();

            ImGui::SetKeyboardFocusHere();

            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            if (ImGui::InputText("##Rename", m_renameBuffer, sizeof(m_renameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                EndRename(object);
            }

            // Stop renaming if click is detected outside of rename box
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            {
                ImVec2 min = ImGui::GetItemRectMin();
                ImVec2 max = ImGui::GetItemRectMax();
                ImVec2 mousePos = ImGui::GetMousePos();

                bool isMouseOutsideInput = mousePos.x < min.x || mousePos.x > max.x || mousePos.y < min.y || mousePos.y > max.y;

                if (isMouseOutsideInput)
                {
                    EndRename(object);
                }
            }

            ImGui::PopID();
        }
        else
        {
            open = ImGui::TreeNodeEx((object->GetName() + "##" + std::to_string((uintptr_t)object.get())).c_str(), nodeFlags);

            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Delete")) { object->Destroy(); }
                if (ImGui::MenuItem("Rename")) 
                {
                    m_renaming = true;
                    m_renamedObject = object;
                    strncpy_s(m_renameBuffer, sizeof(m_renameBuffer), object->GetName().c_str(), _TRUNCATE);
                }

                ImGui::EndPopup();
            }
        }

        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            EditorLayer::Get().SelectObject(object);
        }

        if (open)
        {
            for (const std::shared_ptr<Elevate::GameObject>& child : object->GetChilds())
            {
                DrawTreeHierarchy(child);
            }
            ImGui::TreePop();
        }
    }

    void HierarchyPanel::EndRename(std::shared_ptr<GameObject> object)
    {
        object->SetName(m_renameBuffer);
        m_renaming = false;
        m_renamedObject = nullptr;
    }

    void HierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Hierarchy");

        for (auto it = SceneManager::begin(); it != SceneManager::end(); it++)
        {
            ScenePtr scene = *it;
            if (scene->GetType() == SceneType::RuntimeScene && !scene->GetRootObjects().empty())
            {
                if (ImGui::TreeNodeEx(scene->GetName().c_str(), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth))
                {
                    for (std::shared_ptr<Elevate::GameObject> object : scene->GetRootObjects())
                    {
                        DrawTreeHierarchy(object);
                    }
                    ImGui::TreePop();
                }
            }
        }

        if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
        {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                EditorLayer::Get().SelectObject(nullptr);
            }
        }
        ImGui::End();
    }
}
