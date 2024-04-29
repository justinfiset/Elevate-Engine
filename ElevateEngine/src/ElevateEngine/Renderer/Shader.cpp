#include "eepch.h"
#include "Shader.h"

#include "Renderer.h"

#include "OpenGL/Shader/OpenGLShader.h"

namespace Elevate 
{
	Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSouce)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None : EE_CORE_ASSERT(false, "Renderer none is not supported");
		case RendererAPI::OpenGL: return new OpenGLShader(vertexSource, fragmentSouce);
		}
	}
}