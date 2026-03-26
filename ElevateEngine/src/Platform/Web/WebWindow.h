#pragma once
#ifdef EE_PLATFORM_WEB
#include <Platform/GLFW/GlfwWindow.h>

namespace Elevate
{
	class WebWindow : public GlfwWindow 
	{
	public:
		WebWindow(const WindowProps& props) : GlfwWindow(props) {}
	};
}

#endif