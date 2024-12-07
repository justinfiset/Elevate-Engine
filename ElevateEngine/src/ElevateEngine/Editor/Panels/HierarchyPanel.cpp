#include "HierarchyPanel.h"
#include "eepch.h"

#include "imgui.h"
#include <ElevateEngine/Editor/EditorLayer.h>
#include <ElevateEngine/Scene/SceneManager.h>

void Elevate::Editor::HierarchyPanel::DrawTreeHierarchy(GameObjectPtr object)
{
    ImGuiBackendFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth
        | ImGuiTreeNodeFlags_OpenOnArrow;

    // TODO test if working properly
    if (!object->HasChild())
    {
        nodeFlags |= ImGuiTreeNodeFlags_Leaf;
    }
    // TODO test if working properly
    if (object == EditorLayer::Get().GetSelectedObject())
    {
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    if (ImGui::TreeNodeEx(object->GetName().c_str(), nodeFlags))
    {
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            EditorLayer::Get().SelectObject(object);
        }

        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Delete")) { /*m_Scene->RemoveObject(object);.*/ }
            if (ImGui::MenuItem("Rename")) { /*TODO RENAME*/ }

            ImGui::EndPopup();
        }

        for each (std::shared_ptr<Elevate::GameObject> child in object->GetChilds())
        {
            DrawTreeHierarchy(child);
        }
        ImGui::TreePop();
    }
}

void Elevate::Editor::HierarchyPanel::OnImGuiRender()
{
    ImGui::Begin("Hierarchy");

    // TODO support pour plusieurs scenes ouvertes?????
    // TODO test if working properly


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


    // To unselect a selected item (if selected)
    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
    {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            EditorLayer::Get().SelectObject(nullptr);
        }
    }

    // Drag and Drop pour r�organiser
    //if (ImGui::BeginDragDropSource())
    //{
    //    ImGui::SetDragDropPayload("DND_OBJECT", &m_SelectedObject, sizeof(m_SelectedObject));
    //    ImGui::Text("Dragging %s", m_SelectedObject->GetName().c_str());
    //    ImGui::EndDragDropSource();
    //}
    //if (ImGui::BeginDragDropTarget())
    //{
    //    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_OBJECT"))
    //    {
    //        // Logique pour r�organiser les objets dans la hi�rarchie
    //    }
    //    ImGui::EndDragDropTarget();
    //}

    ImGui::End();
}