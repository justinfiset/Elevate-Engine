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

        // TODO impl dans la classe Component qui est capable de s�rialiser le tout
        // PLacer dans un rendu de transform
        ImGui::SeparatorText("Transform");
        ImGui::InputFloat3("Position", glm::value_ptr(selected->GetPosition()));
        ImGui::InputFloat3("Rotation", glm::value_ptr(selected->GetRotation()));
        ImGui::InputFloat3("Scale", glm::value_ptr(selected->GetScale()));
    }

    ImGui::End();
}
