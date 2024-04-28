#pragma once

#include "Core.h"

#include "ElevateEngine/Core/LayerStack.h"
#include "ElevateEngine/Events/Event.h"
#include "ElevateEngine/Events/ApplicationEvent.h"

#include "ElevateEngine/Core/Window.h"
#include "ElevateEngine/ImGui/ImGuiLayer.h"

#include "ElevateEngine/Renderer/Shader.h"

namespace Elevate {
	class EE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEve�nt(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
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

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
	
		std::unique_ptr<Shader> m_Shader;

		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}