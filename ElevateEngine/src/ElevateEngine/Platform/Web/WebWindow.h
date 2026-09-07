#pragma once
#ifdef EE_PLATFORM_WEB
#include <Platform/Shared/GLFW/GlfwWindow.h>

namespace Elevate
{
	class WebWindow : public GlfwWindow 
	{
	public:
		WebWindow(const WindowProps& props);
		virtual void Init(const WindowProps& props) override;

		// Web Specific Functions
		void GetCanvasSize(unsigned int& width, unsigned int& height);
	};
}

#endif