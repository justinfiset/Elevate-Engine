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
	EE_CORE_TRACE("OpenGL version : {}", glGetString(GL_VERSION));
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
	if (profile & GL_CONTEXT_CORE_PROFILE_BIT) {
		EE_CORE_INFO("OpenGL Context Profile: Core");
	}
	if (profile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT) {
		EE_CORE_INFO("OpenGL Context Profile: Compatibility");
	}

	// Hides faces that are not in the right direction
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	EE_CORE_INFO("OpenGL Renderer Initialized: {0}, {1}", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
}
	
void Elevate::OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}
