#include "eepch.h"
#include "Application.h"

#include <glad/glad.h>

#include "ElevateEngine/Core/Log.h"
#include "ElevateEngine/Core/Assert.h"

#include "ElevateEngine/Inputs/Input.h"

namespace Hammer {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		EE_CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEve­nt));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()	
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEve­nt(Event& e)
	{
		EventDispatcher dispatcher(e); 
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		// Keyboard
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(OnKeyReleasedEvent));

		// Mousse
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(OnMouseButtonReleasedEvent));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() 
	{
		while (m_Running)
		{
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			//ImGui
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			Input::ManageMidStates();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		Input::OnKeyPressedEvent(e);
		return true;
	}

	bool Application::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		Input::OnKeyReleasedEvent(e);
		return true;
	}

	bool Application::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		Input::OnMouseButtonPressedEvent(e);
		return true;
	}

	bool Application::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		Input::OnMouseButtonReleasedEvent(e);
		return true;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}