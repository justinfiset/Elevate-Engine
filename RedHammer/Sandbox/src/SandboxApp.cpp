#include <RedHammer.h>

class DebugLayer : public Hammer::Layer
{
public:
    DebugLayer()
        : Layer("Debug") { }

    void OnUpdate() override
    {
        if (Hammer::Input::IsKeyPressed(RH_KEY_TAB))
        {
            RH_TRACE("Tab key is pressed!");
        }
    }

    void OnEvent(Hammer::Event& event) override
    {
    }
};

class Sandbox : public Hammer::Application
{
public:
    Sandbox() 
    {
        PushLayer(new DebugLayer());
        PushOverlay(new Hammer::ImGuiLayer());
    }

    ~Sandbox()
    {

    }
};

Hammer::Application* Hammer::CreateApplication() {
    return new Sandbox();
}