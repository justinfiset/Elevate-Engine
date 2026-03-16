#pragma once

#include <ElevateEngine/Core/Layers/LayerStack.h>
#include <ElevateEngine/Core/Window.h>
#include <ElevateEngine/Renderer/FrameBuffer.h> // todo remove once the framebuffer is not present in this file anymore
#include <ElevateEngine/Core/GameContext.h>

#ifdef EE_USES_WWISE
#include <ElevateEngine/Audio/Wwise/WwiseSoundEngine.h>
#endif

namespace Elevate {
	class Event;
	class KeyPressedEvent;
	class KeyReleasedEvent;
	class MouseButtonPressedEvent;
	class MouseButtonReleasedEvent;
	class WindowCloseEvent;
	class WindowResizeEvent;
	class WindowFocusEvent;

	class Layer;
	class ImGuiLayer;

	namespace Editor
	{
		class EditorLayer;
	}

	struct ApplicationArguments
	{
		int argc;
		char** argv;

		ApplicationArguments() = default;
		ApplicationArguments(int argc, char** argv)
			: argc(argc), argv(argv) { }
	};

	class Application
	{
	public:
		friend class Elevate::Editor::EditorLayer;

		Application();
		virtual ~Application() = default;

		static void Start(int argc, char** argv);
		void Init();
		void Run();
		void Exit();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		// TODO Maybe move somewhere else
		std::unique_ptr<Framebuffer> FrameBuffer;

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
		static ApplicationArguments GetArguments();

		// GameContextState
		static const GameContextState& GetGameState();
		static void SetGameState(GameContextState newState);

	protected:
		void OnStateChange(GameContextState oldState, GameContextState newState);

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
		bool OnWindowFocusEvent(WindowFocusEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		// TODO move somewhere else (renderer?)
		std::shared_ptr<Framebuffer> m_FrameBuffer;

		GameContextState m_state = Initializing;
		ApplicationArguments m_args;

		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}