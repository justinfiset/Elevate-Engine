#include <memory>
#include <string>

#include <ElevateEngine/Platform/OpenGL/Shaders/OpenGLShader.h>
#include <ElevateEngine/Renderer/RendererBackend.h>

import Elevate.Foundations.Assert;
import Elevate.Foundations.CoreLogger;
import Elevate.Core.Objects.ObjectPtr;
import Elevate.Renderer.Renderer;
import Elevate.Renderer.Shaders;

namespace Elevate::RendererBackend
{
	void RegisterShaderFactories()
	{
		Shader::SetNativeFactories(
			[]() -> std::shared_ptr<Shader> {
				return std::make_shared<OpenGLShader>(
					std::string(Internal::ShaderHeader) + std::string(DefaultShader::GetVertexShader()),
					std::string(Internal::ShaderHeader) + std::string(DefaultShader::GetFragmentShader())
				);
			},
			[]() -> std::shared_ptr<Shader> {
				return std::make_shared<OpenGLShader>(
					std::string(Internal::ShaderHeader) + std::string(DefaultShader::GetVertexShader()),
					std::string(Internal::ShaderHeader) + std::string(DefaultShader::GetErrorShader())
				);
			}
		);

		Shader::SetFactory([](const std::string& vertexSource, const std::string& fragmentSource) -> std::shared_ptr<Shader> {
			switch (Renderer::GetAPI())
			{
			case RendererAPI::GraphicAPI::None:
				Assert::That(false, "Renderer none is not supported");
				return nullptr;
			case RendererAPI::GraphicAPI::OpenGL:
				return std::make_shared<OpenGLShader>(vertexSource, fragmentSource);
			}
			return nullptr;
		});
	}
}
