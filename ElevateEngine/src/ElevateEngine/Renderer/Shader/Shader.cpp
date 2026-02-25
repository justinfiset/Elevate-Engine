#include "eepch.h"
#include "Shader.h"

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
	std::shared_ptr<Shader> Shader::CreateDefault()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None: EE_CORE_ASSERT(false, "Renderer none is not supported");
		case RendererAPI::GraphicAPI::OpenGL: return std::make_shared<OpenGLShader>(std::string(DefaultShader::GetVertexShader()), std::string(DefaultShader::GetFragmentShader()));
		}
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::CreateDefaultError()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None: EE_CORE_ASSERT(false, "Renderer none is not supported");
		case RendererAPI::GraphicAPI::OpenGL: return std::make_shared<OpenGLShader>(std::string(DefaultShader::GetVertexShader()), std::string(DefaultShader::GetErrorShader()));
		}
		return nullptr;
	}

	ShaderPtr Shader::Create(const std::string vertexSource, const std::string fragmentSouce)
	{
		std::shared_ptr<Shader> shader = nullptr;
		
		if (!vertexSource.empty() && !fragmentSouce.empty())
		{
			switch (Renderer::GetAPI())
			{
			case RendererAPI::GraphicAPI::None: EE_CORE_ASSERT(false, "Renderer none is not supported");
			case RendererAPI::GraphicAPI::OpenGL: shader = std::make_shared<OpenGLShader>(vertexSource, fragmentSouce);
			}
		}
		else
		{
			EE_CORE_ERROR("Error : Tried to create a shader with an empty vertex or fragment source -> Vertex : {} And Fragment : {}", vertexSource, fragmentSouce);
			return CreateDefaultError();
		}

		if (!shader->IsInitialized())
		{
			shader.reset(); // Free the memory of that unused shader and reset to nullptr
			EE_CORE_ERROR("Error : Could not create a valid shader. Fallback to default shader.");
			return CreateDefaultError();
		}

		return shader;
	}

	ShaderPtr Shader::CreateFromFiles(std::string vertexSrcPath, std::string fragSrcPath)
	{
		std::string vertexSource = File::GetFileContent(vertexSrcPath);
		std::string fragmentSource = File::GetFileContent(fragSrcPath);
		return Create(vertexSource, fragmentSource);
	}

	ShaderPtr Shader::CreateFromFiles(std::string vertexSrcPath, std::string fragSrcPath, std::string customVertCode, std::string customFragCode)
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

	void Shader::UseLight(Light* newLightSetting, std::string lightName)
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

	void Shader::UpdateCamera()
	{
		Camera* cam = CameraManager::GetCurrent();
		if (cam)
		{
			UpdateCamera(*cam);
		}
	}

	void Shader::UpdateCamera(Camera& cam)
	{
		if (cam.gameObject)
		{
			SetUniform3f(EE_SHADER_CAMPOS, cam.gameObject->GetPosition());
			SetProjectionViewMatrix(cam);
		}
	}

	void Shader::SetModelMatrix(glm::mat4& modelMatrix)
	{
		SetUniformMatrix4fv(EE_SHADER_MODEL, modelMatrix); // set the model matrix
	}

	void Shader::SetModelMatrix(GameObject& object)
	{
		SetUniformMatrix4fv(EE_SHADER_MODEL, object.GetModelMatrix()); // set the model matrix
	}

	void Shader::SetProjectionViewMatrix(glm::mat4 viewProjMatrix)
	{
		SetUniformMatrix4fv(EE_SHADER_VIEWPROJ, viewProjMatrix);
	}

	void Shader::SetProjectionViewMatrix(Camera& cam)
	{
		SetUniformMatrix4fv(EE_SHADER_VIEWPROJ, cam.GenViewProjectionMatrix());
	}

	void Shader::UseMaterial(MaterialPtr newMaterial)
	{
		SetUniform3f("material.ambient", newMaterial->GetAmbiant());
		SetUniform3f("material.diffuse", newMaterial->GetDiffuse());
		SetUniform3f("material.specular", newMaterial->GetSpecular());
		SetUniform1f("material.shininess", newMaterial->GetShininess());
	}
}