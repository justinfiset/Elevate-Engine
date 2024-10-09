#include "eepch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "ElevateEngine/Core/Assert.h"

Elevate::OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
	: m_WindowHandle(windowHandle)
{
	EE_CORE_ASSERT(windowHandle, "Window handle is null");
}

void Elevate::OpenGLContext::Init()
{
	glfwMakeContextCurrent(m_WindowHandle);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	EE_CORE_ASSERT(status, "Failed to initialize Glad.");
	glEnable(GL_DEPTH_TEST);

	// todo reenable when ready
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	// TODO voir si nécessaire
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	EE_CORE_INFO("OpenGL Renderer: {0}, {1}, {2}", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
}
	
void Elevate::OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}
