#pragma once

#include "imgui.h"
#include <ElevateEngine/Editor/EditorWidget.h>

namespace Elevate::Editor 
{
    class StatisticsPanel : public EditorWidget
    {
    public:
        void OnImGuiRender() override
        {
            ImGuiIO& io = ImGui::GetIO();
            ImGui::Begin("Stats");
            ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }
    };
}