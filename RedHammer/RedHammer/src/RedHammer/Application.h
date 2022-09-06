#pragma once

#include "Core.h"

#include "RedHammer/LayerStack.h"
#include "RedHammer/Events/Event.h"
#include "RedHammer/Events/ApplicationEvent.h"

#include "RedHammer/Window.h"
#include "RedHammer/ImGui/ImGuiLayer.h"

namespace Hammer {
	class RH_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEve­nt(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		LayerStack m_LayerStack;

		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}