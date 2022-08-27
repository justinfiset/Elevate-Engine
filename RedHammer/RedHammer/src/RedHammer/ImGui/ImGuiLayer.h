#pragma once

#include "RedHammer/Layer.h"

#include "RedHammer/Events/Event.h"
#include "RedHammer/Events/KeyEvent.h"
#include "RedHammer/Events/MouseEvent.h"
#include "RedHammer/Events/ApplicationEvent.h"

namespace Hammer
{
	class RH_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);

		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);

		bool OnWindowResizeEvent(WindowResizeEvent& e);

	
	private:
		float m_Time = 0.0f;
	};
}