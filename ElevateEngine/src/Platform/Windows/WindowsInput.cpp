#include "eepch.h"
#include "WindowsInput.h"

#include "ElevateEngine/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Hammer
{
	Input* Input::s_Instance = new WindowsInput();
}

std::pair<float, float> Hammer::WindowsInput::GetMousePositionImpl()
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	return { (float)xpos, (float)ypos };	
}

float Hammer::WindowsInput::GetMouseXImpl()
{
	auto [x, y] = GetMousePositionImpl();
	return x;
}

float Hammer::WindowsInput::GetMouseYImpl()
{
	auto [x, y] = GetMousePositionImpl();
	return y;
}