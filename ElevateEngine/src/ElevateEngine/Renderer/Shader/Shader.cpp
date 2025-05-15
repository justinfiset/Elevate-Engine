#include "eepch.h"
#include "Shader.h"

#include <ElevateEngine/Renderer/Renderer.h>

#include <ElevateEngine/Renderer/OpenGL/Shader/OpenGLShader.h>
#include "ElevateEngine/Files/FileUtility.h"
#include "ElevateEngine/Core/GameObject.h"

namespace Elevate 
{
	ShaderPtr Shader::Create(const std::string vertexSource, const std::string fragmentSouce)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None : EE_CORE_ASSERT(false, "Renderer none is not supported");
		case RendererAPI::GraphicAPI::OpenGL: return std::make_shared<OpenGLShader>(vertexSource, fragmentSouce);
		}
	}

	ShaderPtr Shader::CreateFromFiles(std::string vertexSrcPath, std::string fragSrcPath)
	{
		std::string vertexSource = File::GetFileContent(vertexSrcPath);
		std::string fragmentSource = File::GetFileContent(fragSrcPath);
		return Create(vertexSource, fragmentSource);
	}

	ShaderPtr Shader::CreateFromFiles(std::string vertexSrcPath, std::string fragSrcPath, std::string customVertCode, std::string customFragCode)
	{
		std::string vertexSource = customVertCode + "\n" + File::GetFileContent(vertexSrcPath);
		std::string fragmentSource = customFragCode + "\n" + File::GetFileContent(fragSrcPath);
		return Create(vertexSource, fragmentSource);
	}

	void Shader::UseLight(Light* newLightSetting, std::string lightName)
	{
		SetUniform3f("dirLight.ambient", newLightSetting->GetAmbientColor());
		SetUniform3f("dirLight.diffuse", newLightSetting->GetDiffuseColor());
		SetUniform3f("dirLight.specular", newLightSetting->GetSpecularColor());
	}

	void Shader::UseDirLight(DirectionalLight* newDirLight)
	{
		UseLight(newDirLight, "directional");
		SetUniform3f("dirLight.direction", newDirLight->CalculateDirection());
	}

	void Shader::UpdateCamera(Camera& cam)
	{
		SetUniform3f(EE_SHADER_CAMPOS, cam.gameObject->GetPosition());
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