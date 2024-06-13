#pragma once

#include <string>

namespace Elevate
{
	class Shader
	{
	public:
		~Shader() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(const std::string& vertexSource, const std::string& fragmentSouce);
		static Shader* CreateFromFiles(std::string vertexSrcPath, std::string fragSrcPath);

		/// UNIFORMS
		// FLOATS
		virtual void SetUniform1f(std::string location, float value) const = 0;
		virtual void SetUniform2f(std::string location, float x, float y) const = 0;
		virtual void SetUniform3f(std::string location, float x, float y, float z) const = 0;
		virtual void SetUniform4f(std::string location, float x, float y, float z, float w) const = 0;

		// INT
		virtual void SetUniform1i(std::string location, int value) const = 0;
		virtual void SetUniform2i(std::string location, int x, int y) const = 0;
		virtual void SetUniform3i(std::string location, int x, int y, int z) const = 0;
		virtual void SetUniform4i(std::string location, int x, int y, int z, int w) const = 0;

		// FLOAT VECTORS
		virtual void SetUniform1fv(std::string location, int count, float* value) const = 0;
		virtual void SetUniform2fv(std::string location, int count, float* value) const = 0;
		virtual void SetUniform3fv(std::string location, int count, float* value) const = 0;
		virtual void SetUniform4fv(std::string location, int count, float* value) const = 0;

		// INT VECTORS
		virtual void SetUniform1iv(std::string location, int count, int* value) const = 0;
		virtual void SetUniform2iv(std::string location, int count, int* value) const = 0;
		virtual void SetUniform3iv(std::string location, int count, int* value) const = 0;
		virtual void SetUniform4iv(std::string location, int count, int* value) const = 0;

		/*
#define glUniformMatrix2fv glad_glUniformMatrix2fv
		typedef void (APIENTRYP PFNGLUNIFORMMATRIX3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
		GLAPI PFNGLUNIFORMMATRIX3FVPROC glad_glUniformMatrix3fv;
#define glUniformMatrix3fv glad_glUniformMatrix3fv
		typedef void (APIENTRYP PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
		GLAPI PFNGLUNIFORMMATRIX4FVPROC glad_glUniformMatrix4fv;
#define glUniformMatrix4fv glad_glUniformMatrix4fv
		*/

		// TODO CHECK IF NEEDED FOR OTHER APIS
		virtual unsigned int GetRendererID() const = 0;
	};
}

