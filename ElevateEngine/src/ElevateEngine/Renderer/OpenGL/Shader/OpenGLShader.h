#pragma once

#include "ElevateEngine/Renderer/Shader.h"

namespace Elevate
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSouce);
		virtual ~OpenGLShader();

		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		uint32_t m_RendererID = 0;
	};
}