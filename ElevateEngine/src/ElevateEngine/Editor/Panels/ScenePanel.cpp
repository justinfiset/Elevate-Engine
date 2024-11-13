﻿#include "eepch.h"
#include "ScenePanel.h"

#include "imgui.h"
#include "ImGuizmo.h"

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <ElevateEngine/Editor/GizmoUtility.h>
#include <ElevateEngine/Core/Application.h>
#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Editor/EditorLayer.h>

void Elevate::Editor::ScenePanel::OnImGuiRender()
{
    ImGui::Begin("Scene View", nullptr, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {
        // Aspect Ratio Selection
        if (ImGui::BeginMenu(GuizmoOperationToString(m_CurrentEditorTool).c_str()))
        {
            ImGui::RadioButton("Translate", &m_CurrentEditorTool, 7);
            ImGui::RadioButton("Rotate", &m_CurrentEditorTool, 120);
            ImGui::RadioButton("Scale", &m_CurrentEditorTool, 896);
            ImGui::RadioButton("Universal", &m_CurrentEditorTool, 14463);
            ImGui::EndMenu();
        }

        // Permet d'aligner le prochain menu � gauche en fonction de la taille du texte
        ImGui::Dummy(ImVec2(ImGui::GetWindowSize().x - ImGui::CalcTextSize("XXX:XXX").x - ImGui::GetCursorPos().x - 10.0f, 0.0f));

        // Aspect Ratio Selection
        if (ImGui::BeginMenu(GetAspectRatioText(s_AspectRatioSettings[m_AspectRatioValue]).c_str()))
        {
            for (int i = 0; i < sizeof(s_AspectRatioSettings) / sizeof(s_AspectRatioSettings[0]); i++)
            {
                if (ImGui::RadioButton(GetAspectRatioText(s_AspectRatioSettings[i]).c_str(), &m_AspectRatioValue, i)) { UpdateViewportAspectRatio(); }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // we access the ImGui window size
    uint32_t window_width = (uint32_t)ImGui::GetContentRegionAvail().x;
    uint32_t window_height = (uint32_t)ImGui::GetContentRegionAvail().y;

    // Keeping the aspect ratio for the scene view
    const glm::ivec2 aspect = s_AspectRatioSettings[m_AspectRatioValue];
    const float arX = (float)aspect.x;
    const float arY = (float)aspect.y;
    const float qtX = window_width / arX;
    const float qtY = window_height / arY;
    if (qtX < qtY)
        window_height = (uint32_t)(qtX * arY);
    else
        window_width = (uint32_t)(qtY * arX);

    std::shared_ptr<Elevate::FrameBuffer> frameBuffer = Elevate::Application::Get().GetFrameBuffer();
    // we rescale the framebuffer to the actual window size here and reset the glViewport 
    frameBuffer->Rescale(window_width, window_height);
    Renderer::SetViewPort(0, 0, window_width, window_height);

    // we get the screen position of the window
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::Image((void*)(intptr_t)frameBuffer->GetTextureId(), ImVec2((float)window_width, (float)window_height), ImVec2(0, 1), ImVec2(1, 0));

    // ImGuizmo //////////////////////////////////////////
    GameObjectPtr selected = EditorLayer::Get().GetSelectedObject();
    Camera* cam = EditorLayer::Get().GetCamera();
    if (selected)
    {
        ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
        ImGuizmo::SetOrthographic(false); // TODO SET DINAMICLY FROM THE EDITOR AND SETUP THE CAMERA ACCORDINGLY
        ImGuizmo::SetRect(pos.x, pos.y, (float)window_width, (float)window_height);

        glm::mat4 cameraProjection = cam->GetProjectionMatrix();
        glm::mat4 cameraView = cam->GenViewMatrix();
        glm::mat4 entityMatrix = selected->GetModelMatrix();

        // TODO SET VIA BUTTONS
        ImGuizmo::Manipulate(
            glm::value_ptr(cameraView),
            glm::value_ptr(cameraProjection),
            (ImGuizmo::OPERATION)m_CurrentEditorTool,    // Change to ROTATE or SCALE as neeSded
            ImGuizmo::LOCAL,        // Change to LOCAL if needed
            glm::value_ptr(entityMatrix)
        );

        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        if (ImGuizmo::IsUsingAny())
        {
            ImGuizmo::DecomposeMatrixToComponents
            (
                glm::value_ptr(entityMatrix),
                glm::value_ptr(position),
                glm::value_ptr(rotation),
                glm::value_ptr(scale)
            );

            selected->SetScale(scale);
            selected->SetRotation(rotation);
            selected->SetPosition(position);
        }
    }

    ImGui::End();
}

void Elevate::Editor::ScenePanel::UpdateViewportAspectRatio()
{
    Camera* cam = EditorLayer::Get().GetCamera();
    const glm::ivec2 values = s_AspectRatioSettings[m_AspectRatioValue];
    const float ratio = (float)values.x / (float)values.y;
    // Todo: faire que ce soit toutes les cam�ras qui utilisent ce ratio
    // ->>> IDÉE LE FAIRE VIA LE RENDERER
    cam->UpdateAspectRatio(ratio);
}

std::string Elevate::Editor::ScenePanel::GetAspectRatioText(glm::ivec2 ar)
{
    return (std::to_string(ar.x) + ":" + std::to_string(ar.y));
}