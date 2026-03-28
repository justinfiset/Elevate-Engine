#include "GlfwWindow.h"

#include <ElevateEngine/Core/Assert.h>
#include <ElevateEngine/Events/ApplicationEvent.h>
#include <ElevateEngine/Events/MouseEvent.h>
#include <ElevateEngine/Events/KeyEvent.h>
#include <ElevateEngine/Inputs/Input.h>
#include <ElevateEngine/Renderer/OpenGL/OpenGLContext.h>
#include <ElevateEngine/Renderer/Renderer.h>

static void Elevate_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	auto* dataPtr = static_cast<Elevate::WindowData*>(glfwGetWindowUserPointer(window));
	if (!dataPtr)
		return;

	if (action == GLFW_PRESS)
	{
		Elevate::MouseButtonPressedEvent event(button);
		dataPtr->EventCallback(event);
	}
	else if (action == GLFW_RELEASE)
	{
		Elevate::MouseButtonReleasedEvent event(button);
		dataPtr->EventCallback(event);
	}
}

namespace Elevate
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		EE_CORE_ERROR("GLFW Error: ({0}): {1}", error, description);
	}

	GlfwWindow::GlfwWindow(const WindowProps& props)
	{
		Init(props);
	}

	GlfwWindow::~GlfwWindow()
	{
		Shutdown();
	}

	double GlfwWindow::GetTime() const
	{
		return glfwGetTime();
	}

	void GlfwWindow::Init(const WindowProps& props)
	{
		Window::Init(props);

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			EE_CORE_ASSERT(success, "Could not initialize GLFW.");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}
		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		
		SetVSync(props.VSync);

		// Set GLFW callbacks
		glfwSetWindowUserPointer(m_Window, &m_Data);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			void* userPtr = glfwGetWindowUserPointer(window);
			WindowData* dataPtr = static_cast<WindowData*>(userPtr);
			dataPtr->Width = width;
			dataPtr->Height = height;
			WindowResizeEvent event(width, height);
			dataPtr->EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			void* userPtr = glfwGetWindowUserPointer(window);
			WindowData* dataPtr = static_cast<WindowData*>(userPtr);
			WindowCloseEvent event;
			dataPtr->EventCallback(event);
		});

		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused)
		{
			void* userPtr = glfwGetWindowUserPointer(window);
			WindowData* dataPtr = static_cast<WindowData*>(userPtr);
			WindowFocusEvent event(focused);
			dataPtr->EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			void* userPtr = glfwGetWindowUserPointer(window);
			WindowData* dataPtr = static_cast<WindowData*>(userPtr);

			switch(action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					dataPtr->EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					dataPtr->EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					dataPtr->EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			void* userPtr = glfwGetWindowUserPointer(window);
			WindowData* dataPtr = static_cast<WindowData*>(userPtr);
			KeyTypedEvent event(keycode);
			dataPtr->EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, Elevate_MouseButtonCallback);

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			void* userPtr = glfwGetWindowUserPointer(window);
			WindowData* dataPtr = static_cast<WindowData*>(userPtr);
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			dataPtr->EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			void* userPtr = glfwGetWindowUserPointer(window);
			WindowData* dataPtr = static_cast<WindowData*>(userPtr);
			MouseMovedEvent event((float)xPos, (float)yPos);
			dataPtr->EventCallback(event);
		});
	}

	void GlfwWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void GlfwWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void GlfwWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}	

	bool GlfwWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}
