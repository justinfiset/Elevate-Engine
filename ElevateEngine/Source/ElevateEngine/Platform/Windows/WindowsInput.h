#pragma once

#ifdef EE_PLATFORM_WINDOWS
#include <ElevateEngine/Platform/Shared/GLFW/GlfwInput.h>

namespace Elevate
{
	class WindowsInput : public GlfwInput { };
}
#endif