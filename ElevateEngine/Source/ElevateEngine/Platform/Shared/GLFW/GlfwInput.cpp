module;


#include <memory>
#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>

module Elevate.Input;

import Elevate.Core.Application;

#define ELEVATE_INPUT_IMPLEMENTATION
#if defined(EE_PLATFORM_WINDOWS)
#include <ElevateEngine/Platform/Windows/WindowsInput.h>
#elif defined(EE_PLATFORM_WEB)
#include <ElevateEngine/Platform/Web/WebInput.h>
#elif defined(EE_PLATFORM_LINUX)
#include <ElevateEngine/Platform/Linux/LinuxInput.h>
#else
#error "Unknown platform! Cannot create input manager definition on this platform."
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
