#pragma once

#include "ElevateEngine/Core/Layers/Layer.h"
#include "ElevateEngine/Events/Event.h"	
#include "ElevateEngine/Events/KeyEvent.h"
#include "ElevateEngine/Events/MouseEvent.h"
#include "ElevateEngine/Events/ApplicationEvent.h"

namespace Elevate
{
	class EE_API ImGuiLayer : public Layer
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