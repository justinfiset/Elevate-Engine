module;

#include <memory>
#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>

module Elevate.Input;

import Elevate.Core.Application;

#define ELEVATE_INPUT_IMPLEMENTATION
// These implementation headers derive from types owned by this module, so they
// must stay in the module purview. MSVC warns about textual inclusion there.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 5244)
#endif
#if defined(EE_PLATFORM_WINDOWS)
#include "../../Windows/WindowsInput.h"
#elif defined(EE_PLATFORM_WEB)
#include "../../Web/WebInput.h"
#elif defined(EE_PLATFORM_LINUX)
#include "../../Linux/LinuxInput.h"
#else
#error "Unknown platform! Cannot create input manager definition on this platform."
#endif
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

namespace Elevate
{
#if defined(EE_PLATFORM_WINDOWS)
	using PlatformInput = WindowsInput;
#elif defined(EE_PLATFORM_LINUX)
	using PlatformInput = LinuxInput;
#elif defined(EE_PLATFORM_WEB)
	using PlatformInput = WebInput;
#else
#error "Unknown platform! Cannot create input manager definition on this platform."
#endif

	std::unique_ptr<Input> Input::s_Instance = std::make_unique<PlatformInput>();
}

glm::vec2 Elevate::GlfwInput::GetMousePositionImpl()
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	return { (float)xpos, (float)ypos };
}
