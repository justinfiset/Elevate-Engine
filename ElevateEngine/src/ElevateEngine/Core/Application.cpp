#include "eepch.h"
#include "Application.h"

#include "ElevateEngine/Core/Time.h"
#include "ElevateEngine/Core/Log.h"
#include "ElevateEngine/Core/Assert.h"

#include "ElevateEngine/Renderer/Renderer.h"
#include "ElevateEngine/Inputs/Input.h"

#include "ElevateEngine/Files/FileUtility.h"

// TODO REMOVE FROM HERE AT SOME POINT
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "glad/glad.h"

namespace Elevate {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		// Make sure there is a single instance of class Application
		EE_CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEve­nt));

		m_FrameBuffer.reset(FrameBuffer::Create(m_Window->GetWidth(), m_Window->GetHeight())); 
		// TODO MOVE IN ANOTHER FILE maybe???
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
			// TIME UPDATE //////////////////
			Time::currentTime_ = m_Window->GetTime();
			Time::deltaTime_ = Time::currentTime_ - lastTime;
			lastTime = Time::currentTime_;
			/////////////////////////////////

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->PreRender(); // New frame with OpenGl and GLFW

			// Renderer setup for a new frame
			Elevate::Renderer::SetClearColor({ 1.0f, 0.0f, 1.0f, 1.0f }); // Peut être facultatif car on s'en fou un peu au final
			Elevate::Renderer::Clear();

			//ImGui
			m_ImGuiLayer->Begin();
			//for (Layer* layer : m_LayerStack)
			//	layer->OnImGuiRender();
			////
			ImGui::Begin("My Scene");

			// we access the ImGui window size
			const float window_width = ImGui::GetContentRegionAvail().x;
			const float window_height = ImGui::GetContentRegionAvail().y;

			// we rescale the framebuffer to the actual window size here and reset the glViewport 
			m_FrameBuffer->Rescale(window_width, window_height);
			glViewport(0, 0, window_width, window_height);

			// we get the screen position of the window
			ImVec2 pos = ImGui::GetCursorScreenPos();

			// and here we can add our created texture as image to ImGui
			// unfortunately we need to use the cast to void* or I didn't find another way tbh
			ImGui::GetWindowDrawList()->AddImage(
				(void*)m_FrameBuffer->GetTextureID(),
				ImVec2(pos.x, pos.y),
				ImVec2(pos.x + window_width, pos.y + window_height),
				ImVec2(0, 1),
				ImVec2(1, 0)
			);

			ImGui::End();
			////
			m_ImGuiLayer->Render();

			m_FrameBuffer->Bind();
			for (Layer* layer : m_LayerStack)
				layer->OnRender();
			m_FrameBuffer->Unbind();

			// Finish ImGui Rendering
			m_ImGuiLayer->End();

			Input::ManageMidStates(); // Manage Key/Button up and down state
			// Poll events and swap buffers
			m_Window->OnUpdate();
		}
		Cleanup();
	}

	// TODO SUBDIVISER EN SOUS FONCTION RELATIVES AUX DIFFÉRENTES LIBRARIRES
	void Application::Cleanup()
	{
		m_ImGuiLayer->OnDetach();
		// TODO to implements in used classes
		/*
		    glDeleteFramebuffers(1, &FBO);
    glDeleteTextures(1, &texture_id);
    glDeleteRenderbuffers(1, &RBO);
    
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
		*/
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
#pragma endregion
}