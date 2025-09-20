#include "eepch.h"
#include "./AnalyserPanel.h"

#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ElevateEngine/Editor/EditorLayer.h>
#include <ElevateEngine/ImGui/CustomImGuiCommand.h>

void Elevate::Editor::AnalyserPanel::OnImGuiRender()
{
    ImGui::Begin("Analyse");

    std::weak_ptr<GameObject> selected = EditorLayer::Get().GetSelectedObject();
    if (selected.lock())
    {
        std::shared_ptr<GameObject> obj = selected.lock();
        Elevate::UI::InputField("Name: ", obj->GetName());

        // Serialisation of the tranform and all other components
        RenderComponentLayout(obj->GetTransform().GetLayout());

        std::map<EECategory, std::vector<Component*>> m_sortedComponents;
        for (Component* comp : obj->GetComponents())
        {
            m_sortedComponents[comp->GetCategory()].push_back(comp);
        }

        for (std::pair<EECategory, std::vector<Component*>> entry : m_sortedComponents)
        {
            const std::string& categoryName = entry.first.GetName();
            std::string headerLabel = categoryName.empty() ? "Default" : categoryName;

            glm::vec4 color = entry.first.GetCategoryColor();
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(color.r, color.g, color.b, 1.0f));
            if(ImGui::CollapsingHeader(headerLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
            {
                for (Component* comp : entry.second)
                {
                    if (comp)
                    {
                        RenderComponentLayout(comp->GetLayout());
                    }
                }
            }
            ImGui::PopStyleColor();
        }

        if (ImGui::Button("Add Component", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
        {
            ImGui::OpenPopup("AddComponentPopup");
        }

        if (ImGui::BeginPopup("AddComponentPopup"))
        {
            ImGui::Text("Add component :");
            ImGui::Separator();
            for (const auto& [key, name] : ComponentRegistry::GetTypeNames())
            {
                if (ImGui::Selectable(name.c_str()))
                {
                    // TODO: TRANSFORM IN AN EDITOR COMMAND
                }
            }

            ImGui::EndPopup();
        }
    }

    ImGui::End();
}

void Elevate::Editor::AnalyserPanel::RenderComponentLayout(ComponentLayout& layout) const
{
    ImGui::BeginChild(("ComponentContainer" + layout.GetName()).c_str(), ImVec2(0, 0), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Border);
    ImGui::SeparatorText(layout.GetName().c_str());

    for (ComponentField& field : layout)
    {
        RenderField(field);
    }

    ImGui::EndChild();
}

void Elevate::Editor::AnalyserPanel::RenderField(const ComponentField& field) const
{
    switch (field.type)
    {
    case ComponentDataType::Float:
        ImGui::InputFloat(field.name.c_str(), (float*)(field.data));
        break;

    case ComponentDataType::Float2:
        ImGui::InputFloat2(field.name.c_str(), (float*)(field.data));
        break;

    case ComponentDataType::Float3:
        ImGui::InputFloat3(field.name.c_str(), (float*)(field.data));
        break;

    case ComponentDataType::Float4:
        ImGui::InputFloat4(field.name.c_str(), (float*)(field.data));
        break;

    //case ComponentDataType::Color:
    //    ImGui::ColorEdit4(field.name.c_str(), static_cast<float*>(field.data));
    //    break;

    case ComponentDataType::Bool:
        ImGui::Checkbox(field.name.c_str(), (bool*)(field.data));
        break;

    default:
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Unsupported data type: %s", field.name.c_str());
        break;
    }
}
