#include "eepch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include "glm/gtc/type_ptr.hpp"

Elevate::OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSouce)
{
	// Create an empty vertex shader handle
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Send the vertex shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const GLchar* source = vertexSource.c_str();
	glShaderSource(vertexShader, 1, &source, 0);

	// Compile the vertex shader
	glCompileShader(vertexShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(vertexShader);

		EE_CORE_ERROR(infoLog.data());
		EE_CORE_ASSERT(false, "Vertex shader compilation failure.");
		return;
	}


	// Create an empty fragment shader handle
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Send the fragment shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	source = fragmentSouce.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);

	// Compile the fragment shader
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		// TODO optimiser et ne pas utiliser les vecteurs.
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(fragmentShader);
		// Either of them. Don't leak shaders.
		glDeleteShader(vertexShader);

		EE_CORE_ERROR(infoLog.data());
		EE_CORE_ASSERT(false, "Fragment shader compilation failure.");
		return;
	}

	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.
	m_RendererID = glCreateProgram();

	// Attach our shaders to our program
	glAttachShader(m_RendererID, vertexShader);
	glAttachShader(m_RendererID, fragmentShader);

	// Link our program
	glLinkProgram(m_RendererID);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		// TODO optimiser et ne pas utiliser les vecteurs.
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(m_RendererID);
		// Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// Use the infoLog as you see fit.

		// Log and leave
		EE_CORE_ERROR(infoLog.data());
		EE_CORE_ASSERT(false, "Unable to link shaders.");
		return;
	}

	// Always detach shaders after a successful link.
	glDetachShader(m_RendererID, vertexShader);
	glDetachShader(m_RendererID, fragmentShader);
}

Elevate::OpenGLShader::~OpenGLShader()
{
	glDeleteProgram(m_RendererID);
}

void Elevate::OpenGLShader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Elevate::OpenGLShader::Unbind() const
{
	glUseProgram(0);
}

void Elevate::OpenGLShader::SetUniform1f(std::string location, float value) const
{
	glUniform1f(glGetUniformLocation(m_RendererID, location.c_str()), value);
}

void Elevate::OpenGLShader::SetUniform2f(std::string location, float x, float y) const
{
	glUniform2f(glGetUniformLocation(m_RendererID, location.c_str()), x, y);
}

void Elevate::OpenGLShader::SetUniform3f(std::string location, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(m_RendererID, location.c_str()), x, y, z);
}

void Elevate::OpenGLShader::SetUniform4f(std::string location, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(m_RendererID, location.c_str()), x, y, z, w);
}

void Elevate::OpenGLShader::SetUniform1i(std::string location, int value) const
{
	glUniform1i(glGetUniformLocation(m_RendererID, location.c_str()), value);
}

void Elevate::OpenGLShader::SetUniform2i(std::string location, int x, int y) const
{
	glUniform2i(glGetUniformLocation(m_RendererID, location.c_str()), x, y);
}

void Elevate::OpenGLShader::SetUniform3i(std::string location, int x, int y, int z) const
{
	glUniform3i(glGetUniformLocation(m_RendererID, location.c_str()), x, y, z);
}

void Elevate::OpenGLShader::SetUniform4i(std::string location, int x, int y, int z, int w) const
{
	glUniform4i(glGetUniformLocation(m_RendererID, location.c_str()), x, y, z, w);
}

void Elevate::OpenGLShader::SetUniform1fv(std::string location, int count, float* value) const
{
	glUniform1fv(glGetUniformLocation(m_RendererID, location.c_str()), count, value);
}

void Elevate::OpenGLShader::SetUniform2fv(std::string location, int count, float* value) const
{
	glUniform2fv(glGetUniformLocation(m_RendererID, location.c_str()), count, value);
}

void Elevate::OpenGLShader::SetUniform3fv(std::string location, int count, float* value) const
{
	glUniform3fv(glGetUniformLocation(m_RendererID, location.c_str()), count, value);
}

void Elevate::OpenGLShader::SetUniform4fv(std::string location, int count, float* value) const
{
	glUniform4fv(glGetUniformLocation(m_RendererID, location.c_str()), count, value);
}

void Elevate::OpenGLShader::SetUniform1iv(std::string location, int count, int* value) const
{
	glUniform1iv(glGetUniformLocation(m_RendererID, location.c_str()), count, value);
}

void Elevate::OpenGLShader::SetUniform2iv(std::string location, int count, int* value) const
{
	glUniform2iv(glGetUniformLocation(m_RendererID, location.c_str()), count, value);
}

void Elevate::OpenGLShader::SetUniform3iv(std::string location, int count, int* value) const
{
	glUniform3iv(glGetUniformLocation(m_RendererID, location.c_str()), count, value);
}

void Elevate::OpenGLShader::SetUniform4iv(std::string location, int count, int* value) const
{
	glUniform4iv(glGetUniformLocation(m_RendererID, location.c_str()), count, value);
}

void Elevate::OpenGLShader::SetUniformMatrix2fv(std::string location, glm::mat2 data) const
{
	glUniformMatrix2fv(glGetUniformLocation(m_RendererID, location.c_str()), 1, GL_FALSE, glm::value_ptr(data));
}

void Elevate::OpenGLShader::SetUniformMatrix3fv(std::string location, glm::mat3 data) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_RendererID, location.c_str()), 1, GL_FALSE, glm::value_ptr(data));
}

void Elevate::OpenGLShader::SetUniformMatrix4fv(std::string location, glm::mat4 data) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_RendererID, location.c_str()), 1, GL_FALSE, glm::value_ptr(data));
}
