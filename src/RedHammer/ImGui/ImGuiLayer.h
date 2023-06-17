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

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};
}