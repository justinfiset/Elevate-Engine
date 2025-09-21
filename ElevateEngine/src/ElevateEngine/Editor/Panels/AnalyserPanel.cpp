#include "eepch.h"
#include "./AnalyserPanel.h"

#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ElevateEngine/Editor/EditorLayer.h>
#include <ElevateEngine/ImGui/CustomImGuiCommand.h>

#include <ElevateEngine/Editor/Commands/ComponentCommand.h>

void Elevate::Editor::AnalyserPanel::OnImGuiRender()
{
    ImGui::Begin("Analyse");

    std::weak_ptr<GameObject> selected = EditorLayer::Get().GetSelectedObject();
    if (selected.lock())
    {
        std::shared_ptr<GameObject> obj = selected.lock();
        Elevate::UI::InputField("Name: ", obj->GetName());

        // Serialisation of the tranform and all other components
        glm::vec4 color = EECategory("Transform").GetCategoryColor() * 0.6f;

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(color.r + 0.3f, color.g + 0.3f, color.b + 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(color.r, color.g, color.b, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(color.r + 0.1f, color.g + 0.1f, color.b + 0.1f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(color.r + 0.2f, color.g + 0.2f, color.b + 0.2f, 1.0f));

        RenderComponentLayout(obj->GetTransform().GetLayout());

        ImGui::PopStyleVar();
        

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
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(color.r + 0.3f, color.g + 0.3f, color.b + 0.3f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(color.r, color.g, color.b, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(color.r + 0.1f, color.g + 0.1f, color.b + 0.1f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(color.r + 0.2f, color.g + 0.2f, color.b + 0.2f, 1.0f));
            if (ImGui::CollapsingHeader(headerLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::PopStyleColor(4);
                ImGui::PopStyleVar();

                for (Component* comp : entry.second)
                {
                    if (comp)
                    {
                        RenderComponent(comp);
                    }
                }
            }
            else 
            {
                ImGui::PopStyleColor(4);
                ImGui::PopStyleVar();
            }
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

void Elevate::Editor::AnalyserPanel::RenderComponent(Component* comp) const
{
    ComponentLayout& layout = comp->GetLayout();

    EECategory category = comp->GetCategory();

    glm::vec4 color = category.GetCategoryColor() * 0.5f;

    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(color.r + 0.3f, color.g + 0.3f, color.b + 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(color.r, color.g, color.b, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(color.r + 0.1f, color.g + 0.1f, color.b + 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(color.r + 0.2f, color.g + 0.2f, color.b + 0.2f, 1.0f));

    RenderComponentLayout(layout, comp);
}

void Elevate::Editor::AnalyserPanel::RenderComponentLayout(ComponentLayout& layout, Component* component) const
{
    if (UI::EECollapsingHeader((layout.GetName()).c_str(),
        layout.GetFieldCount() > 0,
        [&layout, &component]()
        {
            float menu_width = 0;

            if (component)
            {
                ImGuiWindow* window = ImGui::GetCurrentWindow();
                ImGuiStyle& style = ImGui::GetStyle();

                float customButtonWidth = 20.0f;
                float customButtonHeight = ImGui::GetFontSize() + style.FramePadding.y * 2.0f;
                menu_width = customButtonWidth + style.FramePadding.x;

                ImGui::SetCursorPos(ImVec2(window->Size.x - customButtonWidth - style.FramePadding.x, ImGui::GetCursorPosY()));

                ImGui::PushID(layout.GetName().c_str());
                if (ImGui::Button("...", ImVec2(customButtonWidth, customButtonHeight)))
                    ImGui::OpenPopup("HeaderMenu");

                if (ImGui::BeginPopup("HeaderMenu"))
                {
                    if (ImGui::MenuItem("Remove Component"))
                    {
                        EditorLayer::Get().Execute(std::make_unique<RemoveComponentCommand>(component));
                    }
                    ImGui::EndPopup();
                }
                ImGui::PopID();
            }

            return menu_width;
        }))
    {
        ImGui::PopStyleColor(4);

        for (ComponentField& field : layout)
        {
            RenderField(field);
        }
    }
    else
    {
        ImGui::PopStyleColor(4);
    }
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

    case ComponentDataType::Custom:
        if (ImGui::TreeNodeEx(field.name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding))
        {
            for (const auto& child : field.children)
                RenderField(child);

            ImGui::TreePop();
        }
        break;

    default:
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Unsupported data type: %s", field.name.c_str());
        break;
    }
}
