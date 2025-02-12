#include "eepch.h"
#include "Shader.h"

#include <ElevateEngine/Renderer/Renderer.h>

#include <ElevateEngine/Renderer/OpenGL/Shader/OpenGLShader.h>
#include "ElevateEngine/Files/FileUtility.h"

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

	void Shader::UseMaterial(MaterialPtr newMaterial)
	{
		SetUniform3f("material.ambient", newMaterial->GetAmbiant());
		SetUniform3f("material.diffuse", newMaterial->GetDiffuse());
		SetUniform3f("material.specular", newMaterial->GetSpecular());
		SetUniform1f("material.shininess", newMaterial->GetShininess());
	}
}