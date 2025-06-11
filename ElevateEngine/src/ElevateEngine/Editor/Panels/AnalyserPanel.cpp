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

    GameObjectPtr selected = EditorLayer::Get().GetSelectedObject();
    if (selected != nullptr)
    {
        Elevate::UI::InputField("Name: ", selected->GetName());

        // Serialisation of the tranform and all other components
        RenderComponentLayout(selected->GetTransform().GetLayout());
        for (std::weak_ptr<Component> comp : selected->GetComponents())
        {
            if (std::shared_ptr<Component> locked = comp.lock())
            {
                RenderComponentLayout(locked->GetLayout());
            }
        }
    }

    ImGui::End();
}

void Elevate::Editor::AnalyserPanel::RenderComponentLayout(ComponentLayout& layout) const
{
    ImGui::SeparatorText(layout.GetName().c_str());
    for (ComponentField& field : layout)
    {
        RenderField(field);
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

    default:
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Unsupported data type: %s", field.name.c_str());
        break;
    }
}
