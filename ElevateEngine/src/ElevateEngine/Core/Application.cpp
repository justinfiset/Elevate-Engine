#include "eepch.h"
#include "Application.h"

#include "ElevateEngine/Core/Time.h"
#include "ElevateEngine/Core/Log.h"
#include "ElevateEngine/Core/Assert.h"

#include "ElevateEngine/Renderer/Renderer.h"
#include "ElevateEngine/Inputs/Input.h"

#include "ElevateEngine/Files/FileUtility.h"

namespace Elevate {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		EE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEve­nt));

		m_FrameBuffer.reset(FrameBuffer::Create(m_Window->GetWidth(), m_Window->GetHeight())); 

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

	// Event Dispatcher
	void Application::OnEve­nt(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
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

	void Application::Run() // Each frame
	{
		float lastTime = 0.0f;
		while (m_Running)
		{
			try
			{
				// TIME UPDATE //////////////////
				Time::currentTime_ = m_Window->GetTime();
				Time::deltaTime_ = Time::currentTime_ - lastTime;
				lastTime = Time::currentTime_;
				/////////////////////////////////

				m_ImGuiLayer->PreRender();

				//ImGui
				m_ImGuiLayer->Begin();
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();

				for (Layer* layer : m_LayerStack)
					layer->OnUpdate();

				m_FrameBuffer->Bind(); // Rendering the screen in a single texture
				m_FrameBuffer->Clear();

				// Draw Layers and Scenes
				for (Layer* layer : m_LayerStack)
					layer->OnRender();

				m_FrameBuffer->Unbind(); // Back to normal

				m_ImGuiLayer->Render(); // Render ImGui
				m_ImGuiLayer->End(); // Finish the ImGui Rendering

				Input::ManageMidStates(); // Manage Key/Button up and down state

				Renderer::FlushBuffers();
				// Poll events and swap buffers
				m_Window->OnUpdate();
			}
			catch (const std::exception& exc)
			{
				EE_CORE_ERROR(exc.what());
			}
		}
		Exit();
	}

	void Application::Exit()
	{
		m_ImGuiLayer->Cleanup();
	}

#pragma region Events
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

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		Renderer::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
		return true;
	}

#pragma endregion
}