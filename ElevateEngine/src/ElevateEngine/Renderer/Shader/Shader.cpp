#include "eepch.h"
#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include <ElevateEngine/Renderer/Renderer.h>

#include "ElevateEngine/Renderer/Light/Light.h"
#include "ElevateEngine/Renderer/Light/DirectionalLight.h"
#include <ElevateEngine/Renderer/OpenGL/Shader/OpenGLShader.h>
#include "ElevateEngine/Files/FileUtility.h"
#include "ElevateEngine/Core/GameObject.h"
#include <ElevateEngine/Renderer/Camera.h>
#include <ElevateEngine/Renderer/Camera/CameraManager.h>

namespace Elevate 
{
	std::shared_ptr<Shader> Shader::CreateDefaultNative()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None: EE_CORE_ASSERT(false, "Renderer none is not supported");
		case RendererAPI::GraphicAPI::OpenGL: return std::make_shared<OpenGLShader>(EE_SHADER_HEADER + std::string(DefaultShader::GetVertexShader()), EE_SHADER_HEADER + std::string(DefaultShader::GetFragmentShader()));
		}
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::CreateDefaultErrorNative()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None: EE_CORE_ASSERT(false, "Renderer none is not supported");
		case RendererAPI::GraphicAPI::OpenGL: return std::make_shared<OpenGLShader>(EE_SHADER_HEADER + std::string(DefaultShader::GetVertexShader()), EE_SHADER_HEADER + std::string(DefaultShader::GetErrorShader()));
		}
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::CreateDefault()
	{
		return Create(EE_SHADER_HEADER + std::string(DefaultShader::GetVertexShader()), EE_SHADER_HEADER + std::string(DefaultShader::GetFragmentShader()));
	}

	std::shared_ptr<Shader> Shader::CreateDefaultError()
	{
		return Create(EE_SHADER_HEADER + std::string(DefaultShader::GetVertexShader()), EE_SHADER_HEADER + std::string(DefaultShader::GetErrorShader()));
	}

	ShaderPtr Shader::Create(const std::string& vertexSource, const std::string& fragmentSouce)
	{
		std::shared_ptr<Shader> shader = nullptr;
		
		if (!vertexSource.empty() && !fragmentSouce.empty())
		{
			switch (Renderer::GetAPI())
			{
			case RendererAPI::GraphicAPI::None: EE_CORE_ASSERT(false, "Renderer none is not supported"); break;
			case RendererAPI::GraphicAPI::OpenGL: shader = std::make_shared<OpenGLShader>(vertexSource, fragmentSouce);
			}
		}
		else
		{
			EE_CORE_ERROR("Error : Tried to create a shader with an empty vertex or fragment source -> Vertex : {} And Fragment : {}", vertexSource, fragmentSouce);
			return CreateDefaultErrorNative();
		}

		if (!shader->IsInitialized())
		{
			shader.reset(); // Free the memory of that unused shader and reset to nullptr
			EE_CORE_ERROR("Error : Could not create a valid shader. Fallback to default shader.");
			return CreateDefaultErrorNative();
		}

		shader->m_layout = shader->ExtractReflectionData();

		EE_CORE_TRACE("Shader layout has {} elements.", shader->m_layout.GetElements().size());
		return shader;
	}

	ShaderPtr Shader::CreateFromFiles(const std::string& vertexSrcPath, const std::string& fragSrcPath)
	{
		std::string vertexSource = File::GetFileContent(vertexSrcPath);
		std::string fragmentSource = File::GetFileContent(fragSrcPath);
		return Create(vertexSource, fragmentSource);
	}

	ShaderPtr Shader::CreateFromFiles(const std::string& vertexSrcPath, const std::string& fragSrcPath, const std::string& customVertCode, const std::string& customFragCode)
	{
		std::string vertexContent = File::GetFileContent(vertexSrcPath);
		std::string fragmentContent = File::GetFileContent(fragSrcPath);

		if (!vertexContent.empty() && !fragmentContent.empty())
		{
			std::string vertexSource = customVertCode + "\n" + vertexContent;
			std::string fragmentSource = customFragCode + "\n" + fragmentContent;
			return Create(vertexSource, fragmentSource);
		}
		else
		{
			EE_CORE_ERROR("Error : Tried to create a shader with an empty vertex or fragment source -> Vertex : {} And Fragment : {}", vertexContent, fragmentContent);
			return CreateDefaultError();
		}
	}

	void Shader::UseLight(Light* newLightSetting, const std::string& lightName)
	{
		SetUniform3f(lightName + ".ambient", newLightSetting->GetAmbientColor());
		SetUniform3f(lightName + ".diffuse", newLightSetting->GetDiffuseColor());
		SetUniform3f(lightName + ".specular", newLightSetting->GetSpecularColor());
		SetUniform1f(lightName + ".intensity", newLightSetting->GetIntensity());
	}

	void Shader::UseDirLight(DirectionalLight* newDirLight)
	{
		UseLight(newDirLight, "dirLight");
		SetUniform3f("dirLight.direction", newDirLight->CalculateDirection());
	}

	void Shader::SetCameraPosition(const glm::vec3 cameraPosition) const
	{
		SetUniform3f(EE_SHADER_CAMPOS, cameraPosition);
	}

	void Shader::SetModelMatrix(const glm::mat4& modelMatrix)
	{
		SetUniformMatrix4fv(EE_SHADER_MODEL, modelMatrix); // set the model matrix
	}

	void Shader::SetModelMatrix(const GameObject& object)
	{
		SetUniformMatrix4fv(EE_SHADER_MODEL, object.GetModelMatrix()); // set the model matrix
	}

	void Shader::SetProjectionViewMatrix(const glm::mat4 viewProjMatrix)
	{
		SetUniformMatrix4fv(EE_SHADER_VIEWPROJ, viewProjMatrix);
	}

	void Shader::SetProjectionViewMatrix(const Camera& cam)
	{
		SetUniformMatrix4fv(EE_SHADER_VIEWPROJ, cam.GenViewProjectionMatrix());
	}

	void Elevate::Shader::SetUniformMatrix2fv(const std::string& location, glm::mat2 data) const
	{
		SetUniformMatrix2fv(location, glm::value_ptr(data));
	}

	void Elevate::Shader::SetUniformMatrix3fv(const std::string& location, glm::mat3 data) const
	{
		SetUniformMatrix3fv(location, glm::value_ptr(data));
	}

	void Elevate::Shader::SetUniformMatrix4fv(const std::string& location, glm::mat4 data) const
	{
		SetUniformMatrix4fv(location, glm::value_ptr(data));
	}

	void Shader::SetUniform(const std::string& location, ShaderDataType type, void* value)
	{
		switch (type)
		{
			// todo impl all of the types here based on the functions we have in the shader part
			// todo remove the need to cast to float* and directly use the SetUniform2fv functions (create them if necessary)
		case ShaderDataType::Float:  SetUniform1f(location, *(float*)value); break;
		case ShaderDataType::Float2: 
		{
			float* data = (float*)value;
			SetUniform2f(location, data[0], data[1]);
			break;
		}
		case ShaderDataType::Float3:
		{
			float* data = (float*)value;
			SetUniform3f(location, data[0], data[1], data[2]);
			break;
		}
		case ShaderDataType::Float4:
		{
			float* data = (float*)value;
			SetUniform4f(location, data[0], data[1], data[2], data[3]);
			break;
		}
		case ShaderDataType::Int:
			SetUniform1i(location, *(int*)value);
			break;
		case ShaderDataType::Mat4:
			SetUniformMatrix4fv(location, (float*) value);
			break;
		default:
			EE_CORE_ASSERT(false, "(Shader::SetUniform()) : Unsupported datatype provided.");
			break;
		}
	}
}