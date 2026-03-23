#include "eepch.h"
#include "OpenGLContext.h"

#include <ElevateEngine/Renderer/GraphicsAPI.h>
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

#ifndef EE_PLATFORM_WEB
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    EE_CORE_ASSERT(status, "Failed to initialize Glad.");

    int profile = 0;
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
#endif

    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

    EE_CORE_TRACE("OpenGL version : {}", version ? version : "Unknown");
    EE_CORE_INFO("OpenGL Renderer Initialized: {}, {}", 
        vendor ? vendor : "Unknown", 
        renderer ? renderer : "Unknown");
}
	
void Elevate::OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}
