#pragma once
#ifdef EE_PLATFORM_WEB
#include "../Shared/GLFW/GlfwInput.h"

namespace Elevate
{
	class WebInput : public GlfwInput { };
}

#endif
