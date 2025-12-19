#pragma once

#include <string>
#include "glm/glm.hpp"

// TODO cleanup some of the imports
#include "ElevateEngine/Renderer/Material.h"

// the list of uniform names used by the shader
// TODO v�rif si uniquement avec opengl ou non
#define EE_SHADER_MODEL "model"
#define EE_SHADER_VIEWPROJ "viewProj"
#define EE_SHADER_CAMPOS "camPos"

namespace Elevate
{
	class GameObject;
	class Light;
	class DirectionalLight;
	class Camera;

	class Shader
	{
	public:
		~Shader() {}

		uint32_t GetID() const
		{
			// return the pointer to this object as a uint32_t
			return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(this) >> 4);
		}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual bool IsBound() const = 0;
		virtual uint32_t GetHashCode() const = 0;

		static std::shared_ptr<Shader> CreateDefault(); // Safe fallback in case of shader creation failure
		static std::shared_ptr<Shader> CreateDefaultError(); // Safe fallback with an error pattern shader

		static std::shared_ptr<Shader> Create(const std::string& vertexSource, const std::string& fragmentSouce);
		static std::shared_ptr<Shader> CreateFromFiles(const std::string& vertexSrcPath, const std::string& fragSrcPath);
		static std::shared_ptr<Shader> CreateFromFiles(const std::string& vertexSrcPath, const std::string& fragSrcPath, const std::string& customVertCode, const std::string& customFragCode);

		// Lights
		void UseLight(Light* newLightSetting, const std::string& lightName);
		void UseDirLight(DirectionalLight* newDirLight);

		// Camera
		void UpdateCamera();
		void UpdateCamera(Camera& cam);
			
		void SetModelMatrix(const glm::mat4& modelMatrix);
		void SetModelMatrix(const GameObject& object);

		void SetProjectionViewMatrix(const glm::mat4 viewProjMatrix);

		void SetProjectionViewMatrix(const Camera& cam);

		/// UNIFORMS
		
		/// <summary>
		/// Sets any uniform of any type. Dispatchs the uniform using the right setter function.
		/// </summary>
		/// <param name="location">The name of the uniform.</param>
		/// <param name="value">The pointer to the data.</param>
		virtual void SetUniform(const std::string& location, ShaderDataType type, void* value);

		// FLOATS
		virtual void SetUniform1f(const std::string& location, float value) const = 0;
		virtual void SetUniform2f(const std::string& location, float x, float y) const = 0;
		virtual void SetUniform3f(const std::string& location, float x, float y, float z) const = 0;
		virtual void SetUniform4f(const std::string& location, float x, float y, float z, float w) const = 0;
		// with glm vectors
		inline void SetUniform2f(const std::string& location, glm::vec2 value) const 
		{
			SetUniform2f(location, value.x, value.y);
		}
		inline void SetUniform3f(const std::string& location, glm::vec3 value) const 
		{
			SetUniform3f(location, value.x, value.y, value.z);
		}
		inline void SetUniform4f(const std::string& location, glm::vec4 value) const 
		{
			SetUniform4f(location, value.x, value.y, value.z, value.w);
		}

		// INT
		virtual void SetUniform1i(const std::string& location, int value) const = 0;
		virtual void SetUniform2i(const std::string& location, int x, int y) const = 0;
		virtual void SetUniform3i(const std::string& location, int x, int y, int z) const = 0;
		virtual void SetUniform4i(const std::string& location, int x, int y, int z, int w) const = 0;

		// FLOAT VECTORS
		virtual void SetUniform1fv(const std::string& location, int count, float* value) const = 0;
		virtual void SetUniform2fv(const std::string& location, int count, float* value) const = 0;
		virtual void SetUniform3fv(const std::string& location, int count, float* value) const = 0;
		virtual void SetUniform4fv(const std::string& location, int count, float* value) const = 0;

		// INT VECTORS
		virtual void SetUniform1iv(const std::string& location, int count, int* value) const = 0;
		virtual void SetUniform2iv(const std::string& location, int count, int* value) const = 0;
		virtual void SetUniform3iv(const std::string& location, int count, int* value) const = 0;
		virtual void SetUniform4iv(const std::string& location, int count, int* value) const = 0;

		// MATRIX
		virtual void SetUniformMatrix2fv(const std::string& location, float* data) const = 0;
		void SetUniformMatrix2fv(const std::string& location, glm::mat2 data) const;
		virtual void SetUniformMatrix3fv(const std::string& location, float* data) const = 0;
		void SetUniformMatrix3fv(const std::string& location, glm::mat3 data) const;
		virtual void SetUniformMatrix4fv(const std::string& location, float* data) const = 0;
		void SetUniformMatrix4fv(const std::string& location, glm::mat4 data) const;

		// TODO CHECK IF NEEDED FOR OTHER APIS
		virtual unsigned int GetRendererID() const = 0;

		inline bool IsInitialized() { return m_isInitialized; }
	protected:
		inline void SetInitializationStatus(bool initialized) { m_isInitialized = initialized; }
		bool m_isInitialized = false;
	};

	using ShaderPtr = std::shared_ptr<Shader>;

	class DefaultShader
	{
	public:
		static constexpr std::string_view GetVertexShader() {
			return R"(
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 viewProj;

out vec4 o_Color;

void main()
{
	gl_Position = viewProj * vec4(a_Position, 1.0);
	o_Color = a_Color;
}
)";
		}	

		static constexpr std::string_view GetFragmentShader() {
			return R"(
#version 330 core
out vec4 FragColor;
in vec3 ourColor;

void main()
{
	FragColor = vec4(ourColor, 1.0);
}
)";
		}

		static constexpr std::string_view GetErrorShader() {
			return R"(
#version 330 core
out vec4 FragColor;
uniform float time;

void main()
{
	// Create an obvious "error" pattern
	vec2 uv = gl_FragCoord.xy / 100.0;
	float pattern = sin(uv.x * 10.0 + time) * cos(uv.y * 10.0 + time);
	FragColor = vec4(1.0, 0.0, 0.0, abs(pattern)); // Flashing red
}
)";
		}
	};
}

