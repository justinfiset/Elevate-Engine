#include "eepch.h"
#include "Shader.h"

#include "Renderer.h"

#include "OpenGL/Shader/OpenGLShader.h"
#include "ElevateEngine/Files/FileUtility.h"

namespace Elevate 
{
	Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSouce)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None : EE_CORE_ASSERT(false, "Renderer none is not supported");
		case RendererAPI::GraphicAPI::OpenGL: return new OpenGLShader(vertexSource, fragmentSouce);
		}
	}

	Shader* Shader::CreateFromFiles(std::string vertexSrcPath, std::string fragSrcPath)
	{
		std::string vertexSource = File::GetFileContent(vertexSrcPath);
		std::string fragmentSource = File::GetFileContent(fragSrcPath);
		return Create(vertexSource, fragmentSource);
	}
}