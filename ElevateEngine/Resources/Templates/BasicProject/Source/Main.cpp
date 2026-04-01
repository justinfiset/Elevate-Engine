#include <ElevateEngine.h>

class MyApplicationLayer : public Elevate::Layer
{
public:
	MyApplicationLayer() : Layer("Application") { }

	void OnAttach() override { }

	void OnImGuiRender() override { }
};

class MyApplication : public Elevate::Application
{
public:
	MyApplication()
	{
		PushLayer(new MyApplicationLayer());
	}
	~MyApplication() = default;
};

Elevate::Application* Elevate::CreateApplication() 
{
	return new MyApplication();
}