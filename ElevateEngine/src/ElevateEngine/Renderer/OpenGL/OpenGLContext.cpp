#include "eepch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ElevateEngine/Core/Assert.h"
#include <ElevateEngine/Renderer/Renderer.h>

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

	int profile = 0;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

	// Hides faces that are not in the right direction
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	EE_CORE_TRACE("OpenGL version : {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	EE_CORE_INFO("OpenGL Renderer Initialized: {}, {}",
		reinterpret_cast<const char*>(glGetString(GL_VENDOR)),
		reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
}
	
void Elevate::OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}
