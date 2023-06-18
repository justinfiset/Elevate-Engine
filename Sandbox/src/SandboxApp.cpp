#include <ElevateEngine.h>

#include "imgui/imgui.h"

class DebugLayer : public Hammer::Layer
{
private:
    bool debugMenuActive = false;

    bool isTabKeyPressed;
public:
    DebugLayer() : Layer("Debug") { }

    void OnUpdate() override
    {
        if (Hammer::Input::IsKeyDown('a'))
        {
            debugMenuActive = !debugMenuActive;
        }
    }

    void OnEvent(Hammer::Event& event) override
    {
    }

    void OnImGuiRender() override
    {
        if (!debugMenuActive) return;
        bool toolbarActive = true;
        ImGui::Begin("Elevate Engine", &toolbarActive, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit")) { exit(0); }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Debug"))
            {
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::Text("Elevate Engine - Debugging");
        ImGui::End();
    }
};

class Sandbox : public Hammer::Application
{
public:
    Sandbox() 
    {
        PushLayer(new DebugLayer());
    }

    ~Sandbox()
    {

    }
};

Hammer::Application* Hammer::CreateApplication() {
    return new Sandbox();
}