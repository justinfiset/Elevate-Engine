#include "GlfwInput.h"

#include <GLFW/glfw3.h>

import Elevate.Core.Application;

glm::vec2 Elevate::GlfwInput::GetMousePositionImpl()
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	return { (float)xpos, (float)ypos };
}