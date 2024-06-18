#include "eepch.h"
#include "WindowsInput.h"

#include "ElevateEngine/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Elevate
{
	// TODO make dinaymic instead of hard coded
	Input* Input::s_Instance = new WindowsInput();
}

glm::vec2 Elevate::WindowsInput::GetMousePositionImpl()
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	return { (float)xpos, (float)ypos };	
}