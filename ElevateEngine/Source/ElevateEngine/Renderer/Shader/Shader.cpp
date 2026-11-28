module;

#include <memory>
#include <string>
#include <glm/gtc/type_ptr.hpp>

module Elevate.Renderer.Shaders;

// Foundations
import Elevate.Foundations.Assert;
import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Files;
// Core
import Elevate.Core.Transform.ITransformable;
import Elevate.Core.Objects.ObjectPtr;
// Renderer
import Elevate.Renderer.Lights.Light;

namespace Elevate
{
	EEObjectPtr<Shader> Shader::CreateDefault()
	{
		return Create(std::string(Internal::ShaderHeader) + std::string(DefaultShader::GetVertexShader()), std::string(Internal::ShaderHeader) + std::string(DefaultShader::GetFragmentShader()));
	}

	EEObjectPtr<Shader> Shader::CreateDefaultError()
	{
		return Create(std::string(Internal::ShaderHeader) + std::string(DefaultShader::GetVertexShader()), std::string(Internal::ShaderHeader) + std::string(DefaultShader::GetErrorShader()));
	}

	EEObjectPtr<Shader> Shader::CreateFromFiles(const std::string& vertexSrcPath, const std::string& fragSrcPath)
	{
		std::string vertexSource = std::string(Internal::ShaderHeader) + Files::GetFileContent(vertexSrcPath);
		std::string fragmentSource = std::string(Internal::ShaderHeader) + Files::GetFileContent(fragSrcPath);
		return Create(vertexSource, fragmentSource);
	}

	EEObjectPtr<Shader> Shader::CreateFromFiles(const std::string& vertexSrcPath, const std::string& fragSrcPath, const std::string& customVertCode, const std::string& customFragCode)
	{
		std::string vertexContent = Files::GetFileContent(vertexSrcPath);
		std::string fragmentContent = Files::GetFileContent(fragSrcPath);

		if (!vertexContent.empty() && !fragmentContent.empty())
		{
			std::string vertexSource = std::string(Internal::ShaderHeader) + customVertCode + "\n" + vertexContent;
			std::string fragmentSource = std::string(Internal::ShaderHeader) + customFragCode + "\n" + fragmentContent;
			return Create(vertexSource, fragmentSource);
		}
		else
		{
			CoreLogger::Error("Error : Tried to create a shader with an empty vertex or fragment source -> Vertex : {} And Fragment : {}", vertexContent, fragmentContent);
			return CreateDefaultError();
		}
	}

	void Shader::UseLight(const Light* newLightSetting, const std::string& lightName) const
	{
		SetUniform3f(lightName + ".color", newLightSetting->GetColor());
		SetUniform1f(lightName + ".intensity", newLightSetting->GetIntensity());
	}

	void Shader::UseDirLight(const Light* light, const glm::vec3& direction)
	{
		UseLight(light, "dirLight");
		SetUniform3f("dirLight.direction", direction);
	}

	void Shader::SetCameraPosition(const glm::vec3 cameraPosition) const
	{
		SetUniform3f(ShaderUniforms::CamPos, cameraPosition);
	}

	void Shader::SetModelMatrix(const glm::mat4& modelMatrix)
	{
		SetUniformMatrix4fv(ShaderUniforms::Model, modelMatrix); // set the model matrix
	}

	void Shader::SetModelMatrix(const ITransformable& object)
	{
		SetUniformMatrix4fv(ShaderUniforms::Model, object.GetModelMatrix()); // set the model matrix
	}

	void Shader::SetProjectionViewMatrix(const glm::mat4 viewProjMatrix)
	{
		SetUniformMatrix4fv(ShaderUniforms::ViewProj, viewProjMatrix);
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
		case ShaderDataType::Float:
			SetUniform1f(location, *(float*)value);
			break;
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
			SetUniformMatrix4fv(location, (float*)value);
			break;
		default:
			Assert::That(false, "(Shader::SetUniform()) : Unsupported datatype provided.");
			break;
		}
	}
}