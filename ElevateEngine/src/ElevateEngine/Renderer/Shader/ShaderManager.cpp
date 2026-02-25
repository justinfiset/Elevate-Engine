#include "eepch.h"
#include "ShaderManager.h"

#include <ElevateEngine/Renderer/Shader/Shader.h>

namespace Elevate {
	ShaderPtr ShaderManager::LoadShader(std::string name, ShaderPtr shader)
	{
		if (instance().m_Shaders.count(name) > 0) 
		{
			return instance().m_Shaders[name];
		}
		else
		{
			instance().m_Shaders[name] = shader;
			return shader;
		}
	}

	ShaderPtr ShaderManager::LoadShader(std::string name, std::string vertexSrcPath, std::string fragSrcPath)
	{
		return LoadShader(name, Shader::CreateFromFiles(vertexSrcPath, fragSrcPath));
	}

	ShaderPtr ShaderManager::LoadShader(std::string name, std::string vertexSrcPath, std::string fragSrcPath, std::string customVertCode, std::string customFragCode)
	{
		return LoadShader(name, Shader::CreateFromFiles(vertexSrcPath, fragSrcPath, customVertCode, customFragCode));
	}

	ShaderPtr ShaderManager::GetShader(std::string name)
	{
		return (instance().m_Shaders.count(name) > 0) ? instance().m_Shaders[name] : nullptr;
	}
}