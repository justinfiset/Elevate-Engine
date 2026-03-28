#pragma once
#ifdef EE_PLATFORM_LINUX
#include <Platform/Shared/GLFW/GlfwWindow.h>

namespace Elevate
{
	class LinuxWindow : public GlfwWindow 
	{
	public:
		LinuxWindow(const WindowProps& props) : GlfwWindow(props) {}
	};
}

#endif