#pragma once

#include <string>
#include "glm/glm.hpp"

// TODO cleanup some of the imports
#include "ElevateEngine/Renderer/Material.h"
#include "ElevateEngine/Renderer/Light/Light.h"
#include "ElevateEngine/Renderer/Light/DirectionalLight.h"
#include "ElevateEngine/Core/GameObject.h" // find a way to remove from here
#include "ElevateEngine/Renderer/Camera.h"


// the list of uniform names used by the shader
// TODO vérif si uniquement avec opengl ou non
#define EE_SHADER_MODEL "model"
#define EE_SHADER_VIEWPROJ "viewProj"
#define EE_SHADER_CAMPOS "camPos"

namespace Elevate
{
	class Shader;
	using ShaderPtr = std::shared_ptr<Shader>;

	class Shader
	{
	public:
		~Shader() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual bool IsBound() const = 0;
		virtual uint32_t GetHashCode() const = 0;

		static ShaderPtr Create(std::string vertexSource, std::string fragmentSouce);
		static ShaderPtr CreateFromFiles(std::string vertexSrcPath, std::string fragSrcPath);
		static ShaderPtr CreateFromFiles(std::string vertexSrcPath, std::string fragSrcPath, std::string customVertCode, std::string customFragCode);

		// Lights
		void UseLight(Light* newLightSetting, std::string lightName);
		void UseDirLight(DirectionalLight* newDirLight);

		// Materials
		void UseMaterial(Material* newMaterial);

		// Camera
		inline void UpdateCamera(Camera& cam)
		{
			// TODO check if there is any better way to do this
			SetUniform3f(EE_SHADER_CAMPOS, cam.gameObject->GetPosition());
		}

		inline void SetModelMatrix(glm::mat4& modelMatrix)
		{
			SetUniformMatrix4fv(EE_SHADER_MODEL, modelMatrix); // set the model matrix
		}
		inline void SetModelMatrix(GameObject& object)
		{
			SetUniformMatrix4fv(EE_SHADER_MODEL, object.GetModelMatrix()); // set the model matrix
		}

		inline void SetProjectionViewMatrix(glm::mat4 viewProjMatrix)
		{
			SetUniformMatrix4fv(EE_SHADER_VIEWPROJ, viewProjMatrix);
		}

		inline void SetProjectionViewMatrix(Camera& cam)
		{
			SetUniformMatrix4fv(EE_SHADER_VIEWPROJ, cam.GenViewProjectionMatrix());
		}

		/// UNIFORMS
		// FLOATS
		virtual void SetUniform1f(std::string location, float value) const = 0;
		virtual void SetUniform2f(std::string location, float x, float y) const = 0;
		virtual void SetUniform3f(std::string location, float x, float y, float z) const = 0;
		virtual void SetUniform4f(std::string location, float x, float y, float z, float w) const = 0;
		// with glm vectors
		inline void SetUniform2f(std::string location, glm::vec2 value) const 
		{
			SetUniform2f(location, value.x, value.y);
		}
		inline void SetUniform3f(std::string location, glm::vec3 value) const 
		{
			SetUniform3f(location, value.x, value.y, value.z);
		}
		inline void SetUniform4f(std::string location, glm::vec4 value) const 
		{
			SetUniform4f(location, value.x, value.y, value.z, value.w);
		}

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

		// MATRIX
		virtual void SetUniformMatrix2fv(std::string location, glm::mat2 data) const = 0;
		virtual void SetUniformMatrix3fv(std::string location, glm::mat3 data) const = 0;
		virtual void SetUniformMatrix4fv(std::string location, glm::mat4 data) const = 0;

		// TODO CHECK IF NEEDED FOR OTHER APIS
		virtual unsigned int GetRendererID() const = 0;
	};
}

