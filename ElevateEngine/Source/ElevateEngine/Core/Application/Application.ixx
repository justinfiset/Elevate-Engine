module;

#include <memory>

export module Elevate.Core.Application;

export import Elevate.Core.Application.GameContext;

import Elevate.Foundations.Services;

import Elevate.Core.Window;
import Elevate.Core.Events;
import Elevate.Core.Layers;
import Elevate.Physics;

#ifdef EE_USES_WWISE
import Elevate.Audio.Wwise.SoundEngine;
#endif

export namespace Elevate {
	class Application;
	using ApplicationFactory = Application * (*)();

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
			: argc(argc), argv(argv) {
		}
	};

	class Application
	{
	public:
		friend class Elevate::Editor::EditorLayer;

		Application();
		virtual ~Application() = default;

		static void Start(int argc, char** argv, ApplicationFactory factory);
		void Init();
		void EngineFrame();
		void Run();
		void Exit();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
		static ApplicationArguments GetArguments();

		// GameContextState
		static const GameContextState& GetGameState();
		static void SetGameState(GameContextState newState);

		ServiceProvider& GetServices() { return *m_Provider; }

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

		// Phyisc
		std::unique_ptr<PhysicsSystem> m_PhysicsSystem;

		GameContextState m_state = Initializing;
		ApplicationArguments m_args;

		std::unique_ptr<ServiceProvider> m_Provider;

		static Application* s_Instance;
	};
}