#include "eepch.h"
#include "ShaderManager.h"

namespace Elevate {
	ShaderPtr ShaderManager::LoadShader(std::string name, ShaderPtr shader)
	{
		if (instance().m_Shaders.count(name) > 0) 
		{
			EE_CORE_TRACE("ALREADY HAS {0}", name);
			return instance().m_Shaders[name];
		}
		else
		{
			EE_CORE_TRACE("SETTING {0}", name);
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
		EE_CORE_TRACE("GETTING {0}", name);
		return (instance().m_Shaders.count(name) > 0) ? instance().m_Shaders[name] : nullptr;
	}
}