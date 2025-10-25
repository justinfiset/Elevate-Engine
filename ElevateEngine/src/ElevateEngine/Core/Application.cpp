#include "eepch.h"
#include "Application.h"

#include "ElevateEngine/Core/Time.h"
#include "ElevateEngine/Core/Core.h"
#include "ElevateEngine/Core/Log.h"
#include "ElevateEngine/Core/Assert.h"
#include "ElevateEngine/Core/Layers/LayerStack.h"

#include "ElevateEngine/Renderer/Renderer.h"
#include "ElevateEngine/Renderer/Texture/TextureManager.h"
#include <ElevateEngine/Renderer/Debug/DebugRenderer.h>

#include "ElevateEngine/Inputs/Input.h"
#include "ElevateEngine/Files/FileUtility.h"
#include "ElevateEngine/Events/ApplicationEvent.h"
#include "ElevateEngine/ImGui/ImGuiLayer.h"
#include "ElevateEngine/Editor/EditorLayer.h"

#include <ElevateEngine/Audio/SoundEngine.h>

namespace Elevate {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		EE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		EE_CORE_TRACE("Current working directory : %s", std::filesystem::current_path().string().c_str());

		SoundEngine::Init();

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		DebugRenderer::Init();

		FrameBuffer.reset(Framebuffer::Create(m_Window->GetWidth(), m_Window->GetHeight())); 
		FrameBuffer->SetClearColor({ 0.8f, 0.4f, 0.7f, 1.0f }); // Pink / purple for debug purposes

		#ifdef EE_EDITOR_BUILD
			PushOverlay(new Elevate::Editor::EditorLayer());
			SetGameState(GameContextState::EditorMode);
		#else
			SetGameState(GameContextState::Runtime);
		#endif

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
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
	void Application::OnEvent(Event& e)
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

	void Application::Start(int argc, char** argv)
	{
		//Log::Init(); // todo remove or uncomment depending on if the workarround worked
		EE_CORE_INFO("Initializing ElevateEngine...");
		auto app = CreateApplication();
		EE_CORE_TRACE("Application Initialized.");
		app->Run();
		delete app;
	}

	void Application::Run() // Each frame
	{
		float lastTime = 0.0f;
		while (m_Running)
		{
			try
			{
				// TIME UPDATE //////////////////
				Time::currentTime_ = (float) m_Window->GetTime();
				Time::deltaTime_ = Time::currentTime_ - lastTime;
				lastTime = Time::currentTime_;
				/////////////////////////////////

				SoundEngine::RenderAudio();
				TextureManager::UpdateLoadingTextures();

				FrameBuffer->Bind(); // Rendering the screen in a single texture
				FrameBuffer->Clear();

				for (Layer* layer : m_LayerStack)
					layer->OnUpdate();

				// Draw Layers and Scenes
				for (Layer* layer : m_LayerStack)
					layer->OnRender();

				DebugRenderer::Render();

				FrameBuffer->Unbind(); // Back to normal

				//imgui
				m_ImGuiLayer->PreRender();
				m_ImGuiLayer->Begin();

				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
				
				m_ImGuiLayer->Render(); // render imgui
				m_ImGuiLayer->End(); // finish the imgui rendering

				Input::ManageMidStates(); // Manage Key/Button up and down state

				Renderer::FlushBuffers();	
				// Poll events and swap buffers
				m_Window->OnUpdate();
			}
			catch (const std::exception& exc)
			{
				EE_CORE_ERROR("{}", exc.what());
			}
		}
		Exit();
	}

	void Application::Exit()
	{
		m_ImGuiLayer->Cleanup();
		SoundEngine::Terminate();
	}

	const GameContextState& Application::GetGameState()
	{
		return s_Instance->m_state;
	}

	void Application::SetGameState(GameContextState newState)
	{
		if (s_Instance->m_state != newState)
		{
			GameContextState oldState = s_Instance->m_state;
			s_Instance->m_state = newState;

			EE_CORE_INFO("GameContext state changed from %s to %s",
				GetGameContextStateName(oldState),
				GetGameContextStateName(newState));

			s_Instance->OnStateChange(oldState, newState);
		}
	}

#pragma region Events	
	void Application::OnStateChange(GameContextState oldState, GameContextState newState)
	{
		GameContextEvent e(oldState, newState);
		OnEvent(e);
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

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		Renderer::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
		return true;
	}

#pragma endregion
}