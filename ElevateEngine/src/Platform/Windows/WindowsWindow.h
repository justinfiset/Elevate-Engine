#pragma once
#ifdef EE_PLATFORM_WINDOWS
#include <Platform/GLFW/GlfwWindow.h>

namespace Elevate
{
	class WindowsWindow : public GlfwWindow 
	{
	public:
		WindowsWindow(const WindowProps& props) : GlfwWindow(props) {}
	};
}

#endif