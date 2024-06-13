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

		virtual unsigned int GetRendererID() const override { return m_RendererID;  }

		/// UNIFORMS
		// FLOATS
		virtual void SetUniform1f(std::string location, float value) const override;
		virtual void SetUniform2f(std::string location, float x, float y) const override;
		virtual void SetUniform3f(std::string location, float x, float y, float z) const override;
		virtual void SetUniform4f(std::string location, float x, float y, float z, float w) const override;

		// INT
		virtual void SetUniform1i(std::string location, int value) const override;
		virtual void SetUniform2i(std::string location, int x, int y) const override;
		virtual void SetUniform3i(std::string location, int x, int y, int z) const override;
		virtual void SetUniform4i(std::string location, int x, int y, int z, int w) const override;

		// FLOAT VECTORS
		virtual void SetUniform1fv(std::string location, int count, float* value) const override;
		virtual void SetUniform2fv(std::string location, int count, float* value) const override;
		virtual void SetUniform3fv(std::string location, int count, float* value) const override;
		virtual void SetUniform4fv(std::string location, int count, float* value) const override;

		// INT VECTORS
		virtual void SetUniform1iv(std::string location, int count, int* value) const override;
		virtual void SetUniform2iv(std::string location, int count, int* value) const override;
		virtual void SetUniform3iv(std::string location, int count, int* value) const override;
		virtual void SetUniform4iv(std::string location, int count, int* value) const override;
	private:
		uint32_t m_RendererID = 0;
	};
}