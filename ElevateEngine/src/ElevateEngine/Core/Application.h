#pragma once

#include "Core.h"

#include "ElevateEngine/Core/Layers/LayerStack.h"
#include "ElevateEngine/Events/Event.h"
#include "ElevateEngine/Events/ApplicationEvent.h"

#include "ElevateEngine/Core/Window.h"
#include "ElevateEngine/ImGui/ImGuiLayer.h"

#include <ElevateEngine/Renderer/Shader/Shader.h>
#include "ElevateEngine/Renderer/Buffer.h"
#include "ElevateEngine/Renderer/FrameBuffer.h"
#include "ElevateEngine/Renderer/VertexArray.h"
#include <ElevateEngine/Core/GameContext.h>

namespace Elevate {
	class EE_API Application : public GameContext
	{
	public:
		friend class Elevate::Editor::EditorLayer;

		Application();
		virtual ~Application();

		void Run();
		void Exit();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		// TODO changer pour mettre dans une classe qui s'occupe du rendu
		inline std::shared_ptr<Framebuffer> GetFrameBuffer() { return m_FrameBuffer; }

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

		inline static GameContextState GameState() { return s_Instance->State(); }

		// TODO METTRE PROTECTED
		inline static void SetGameState(GameContextState state) { s_Instance->GameContext::SetState(state); }
	protected:
		
		void OnStateChange(GameContextState oldState, GameContextState newState) override;
	private:
		/// Events Callback / Handler
		//Keyboard
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		//Mouse
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		//Window
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		// TODO Maybe move somewhere else
		std::shared_ptr<Framebuffer> m_FrameBuffer;

		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}