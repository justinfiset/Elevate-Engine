#pragma once

#ifdef EE_PLATFORM_LINUX
#include "Platform/GLFW/GlfwInput.h"

namespace Elevate
{
	class LinuxInput : public GlfwInput { };
}
#endif