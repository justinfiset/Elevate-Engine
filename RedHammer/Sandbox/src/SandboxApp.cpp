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
        if (event.GetEventType() == Hammer::EventType::KeyPressed)
        {
            Hammer::KeyPressedEvent& e = (Hammer::KeyPressedEvent&)event;

            // Convert keycode to ASCII keycode
            RH_TRACE("{0}", (char)e.GetKeyCode());
        }
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