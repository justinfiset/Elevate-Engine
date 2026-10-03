#include "eepch.h"
#include <ElevateEngine/Renderer/GraphicsAPI.h>

import Elevate.Foundations;

#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <ElevateEngine/Renderer/Renderer.h>

Elevate::OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
	: m_WindowHandle(windowHandle)
{
	Assert::That(windowHandle, "Window handle is null");
}

void Elevate::OpenGLContext::Init()
{
    glfwMakeContextCurrent(m_WindowHandle);

#ifndef EE_PLATFORM_WEB
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    Assert::That(status, "Failed to initialize Glad.");

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

    CoreLogger::Trace("OpenGL version : {}", version ? version : "Unknown");
    CoreLogger::Info("OpenGL Renderer Initialized: {}, {}", 
        vendor ? vendor : "Unknown", 
        renderer ? renderer : "Unknown");
}
	
void Elevate::OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}
