#pragma once

#ifdef EE_PLATFORM_LINUX
#include <ElevateEngine/Platform/Shared/GLFW/GlfwInput.h>

namespace Elevate
{
	class LinuxInput : public GlfwInput { };
}
#endif