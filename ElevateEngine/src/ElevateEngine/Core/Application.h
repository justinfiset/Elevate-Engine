#pragma once

#include <ElevateEngine/Core/Layers/LayerStack.h>
#include <ElevateEngine/Core/Window.h>
#include <ElevateEngine/Renderer/FrameBuffer.h>
#include <ElevateEngine/Core/GameContext.h>

namespace Elevate {
	class Event;
	class KeyPressedEvent;
	class KeyReleasedEvent;
	class MouseButtonPressedEvent;
	class MouseButtonReleasedEvent;
	class WindowCloseEvent;
	class WindowResizeEvent;

	class LayerStack;
	class ImGuiLayer;

	namespace Editor
	{
		class EditorLayer;
	}

	class Application : public GameContext
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