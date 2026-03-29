#include "eepch.h"

#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <ElevateEngine/Renderer/GraphicsAPI.h>

#include "glm/gtc/type_ptr.hpp"

#include <ElevateEngine/Renderer/OpenGL/Shader/OpenGLShader.h>
#include <ElevateEngine/Renderer/GLDebug.h>
#include <ElevateEngine/Core/Assert.h>

namespace Elevate
{
	OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSouce)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = GLCheck(glCreateShader(GL_VERTEX_SHADER));

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertexSource.c_str();
		GLCheck(glShaderSource(vertexShader, 1, &source, 0));

		// Compile the vertex shader
		GLCheck(glCompileShader(vertexShader));

		GLint isCompiled = 0;
		GLCheck(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled));

		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			EE_CORE_ERROR("{}", infoLog.data());
			EE_CORE_ASSERT(false, "Vertex shader compilation failure.");
			return;
		}


		// Create an empty fragment shader handle
		GLuint fragmentShader = GLCheck(glCreateShader(GL_FRAGMENT_SHADER));

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragmentSouce.c_str();
		GLCheck(glShaderSource(fragmentShader, 1, &source, 0));

		// Compile the fragment shader
		GLCheck(glCompileShader(fragmentShader));

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

			EE_CORE_ERROR("{}", infoLog.data());
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
			EE_CORE_ERROR("{}", infoLog.data());
			EE_CORE_ERROR("Error : Unable to link shaders.");
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);

		SetInitializationStatus(true);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	uint32_t OpenGLShader::GetID() const
	{
		return m_RendererID;
	}

	void OpenGLShader::Bind() const
	{
		GLCheck(glUseProgram(m_RendererID));
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	uint32_t OpenGLShader::GetHashCode() const
	{
		return m_RendererID;
	}

	void OpenGLShader::SetUniform1f(const std::string& location, float value) const
	{
		GLCheck(glUniform1f(glGetUniformLocation(m_RendererID, location.c_str()), value));
	}

	void OpenGLShader::SetUniform2f(const std::string& location, float x, float y) const
	{
		GLCheck(glUniform2f(glGetUniformLocation(m_RendererID, location.c_str()), x, y));
	}

	void OpenGLShader::SetUniform3f(const std::string& location, float x, float y, float z) const
	{
		GLCheck(glUniform3f(glGetUniformLocation(m_RendererID, location.c_str()), x, y, z));
	}

	void OpenGLShader::SetUniform4f(const std::string& location, float x, float y, float z, float w) const
	{
		GLCheck(glUniform4f(glGetUniformLocation(m_RendererID, location.c_str()), x, y, z, w));
	}

	void OpenGLShader::SetUniform1i(const std::string& location, int value) const
	{
		GLCheck(glUniform1i(glGetUniformLocation(m_RendererID, location.c_str()), value));
	}

	void OpenGLShader::SetUniform2i(const std::string& location, int x, int y) const
	{
		GLCheck(glUniform2i(glGetUniformLocation(m_RendererID, location.c_str()), x, y));
	}

	void OpenGLShader::SetUniform3i(const std::string& location, int x, int y, int z) const
	{
		GLCheck(glUniform3i(glGetUniformLocation(m_RendererID, location.c_str()), x, y, z));
	}

	void OpenGLShader::SetUniform4i(const std::string& location, int x, int y, int z, int w) const
	{
		GLCheck(glUniform4i(glGetUniformLocation(m_RendererID, location.c_str()), x, y, z, w));
	}

	void OpenGLShader::SetUniform1fv(const std::string& location, int count, float* value) const
	{
		GLCheck(glUniform1fv(glGetUniformLocation(m_RendererID, location.c_str()), count, value));
	}

	void OpenGLShader::SetUniform2fv(const std::string& location, int count, float* value) const
	{
		GLCheck(glUniform2fv(glGetUniformLocation(m_RendererID, location.c_str()), count, value));
	}

	void OpenGLShader::SetUniform3fv(const std::string& location, int count, float* value) const
	{
		GLCheck(glUniform3fv(glGetUniformLocation(m_RendererID, location.c_str()), count, value));
	}

	void OpenGLShader::SetUniform4fv(const std::string& location, int count, float* value) const
	{
		GLCheck(glUniform4fv(glGetUniformLocation(m_RendererID, location.c_str()), count, value));
	}

	void OpenGLShader::SetUniform1iv(const std::string& location, int count, int* value) const
	{
		GLCheck(glUniform1iv(glGetUniformLocation(m_RendererID, location.c_str()), count, value));
	}

	void OpenGLShader::SetUniform2iv(const std::string& location, int count, int* value) const
	{
		GLCheck(glUniform2iv(glGetUniformLocation(m_RendererID, location.c_str()), count, value));
	}

	void OpenGLShader::SetUniform3iv(const std::string& location, int count, int* value) const
	{
		GLCheck(glUniform3iv(glGetUniformLocation(m_RendererID, location.c_str()), count, value));
	}

	void OpenGLShader::SetUniform4iv(const std::string& location, int count, int* value) const
	{
		GLCheck(glUniform4iv(glGetUniformLocation(m_RendererID, location.c_str()), count, value));
	}

	void OpenGLShader::SetUniformMatrix2fv(const std::string& location, float* data) const
	{
		GLCheck(glUniformMatrix2fv(glGetUniformLocation(m_RendererID, location.c_str()), 1, GL_FALSE, data));
	}

	void OpenGLShader::SetUniformMatrix3fv(const std::string& location, float* data) const
	{
		GLCheck(glUniformMatrix3fv(glGetUniformLocation(m_RendererID, location.c_str()), 1, GL_FALSE, data));
	}

	void OpenGLShader::SetUniformMatrix4fv(const std::string& location, float* data) const
	{
		//GLCheck(glUniformMatrix4fv(glGetUniformLocation(m_RendererID, location.c_str()), 1, GL_FALSE, data));
		// todo remove this debug code 
		int loc = glGetUniformLocation(m_RendererID, location.c_str());
		if (loc == -1) {
			EE_CORE_ERROR("Uniform '{0}' introuvable dans le shader ID {1}!", location, m_RendererID);
		}
		glUniformMatrix4fv(loc, 1, GL_FALSE, data);
	}

	static EngineDataType GLTypeToEngineDataType(GLenum type)
	{
		switch (type)
		{
		case GL_FLOAT:              return EngineDataType::Float;
		case GL_FLOAT_VEC2:         return EngineDataType::Float2;
		case GL_FLOAT_VEC3:         return EngineDataType::Float3;
		case GL_FLOAT_VEC4:         return EngineDataType::Float4;
		case GL_INT:                return EngineDataType::Int;
		case GL_INT_VEC2:           return EngineDataType::Int2;
		case GL_INT_VEC3:           return EngineDataType::Int3;
		case GL_INT_VEC4:           return EngineDataType::Int4;
		case GL_BOOL:               return EngineDataType::Bool;
		case GL_FLOAT_MAT3:         return EngineDataType::Mat3;
		case GL_FLOAT_MAT4:         return EngineDataType::Mat4;
		case GL_SAMPLER_2D:         return EngineDataType::Sampler2D;
		}
		return EngineDataType::Unknown;
	}

	BufferLayout OpenGLShader::ExtractReflectionData() const
	{
		GLint uniformCount = 0;
		glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORMS, &uniformCount);

		std::vector<BufferElement> elements;
		elements.reserve(uniformCount);

		GLint maxNameLength = 0;
		glGetProgramiv(m_RendererID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);
		std::vector<GLchar> nameBuffer(maxNameLength);

		EE_CORE_TRACE("Starting reflection for shader ID {}", m_RendererID);
		for (GLint i = 0; i < uniformCount; i++)
		{
			GLsizei length;
			GLint size;
			GLenum type;
			glGetActiveUniform(m_RendererID, i, maxNameLength, &length, &size, &type, nameBuffer.data());
			std::string name(nameBuffer.data(), length);
			elements.push_back({ GLTypeToEngineDataType(type), name });
			EE_CORE_TRACE(" - Found uniform {}", name);
		}
		EE_CORE_TRACE("Completed shader reflection.");
		return BufferLayout(elements);
	}
}