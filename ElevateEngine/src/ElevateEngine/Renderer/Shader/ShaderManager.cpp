#include "eepch.h"
#include "ShaderManager.h"

#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Renderer/Shader/Shader.h>

namespace Elevate {
	ShaderPtr ShaderManager::LoadShader(const std::string& name, ShaderPtr shader)
	{
		if (name == EE_DEFAULT_SHADER && GetShader(EE_DEFAULT_SHADER))
		{
			EE_CORE_ERROR("(ShaderManager::LoadShader) : You cannot create a shader with the name : default. This is reservez for the default shader for the whole engine.");
			return nullptr;
		}

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

	ShaderPtr ShaderManager::LoadShader(const std::string& name, const std::string& vertexSrcPath, const std::string& fragSrcPath)
	{
		return LoadShader(name, Shader::CreateFromFiles(vertexSrcPath, fragSrcPath));
	}

	ShaderPtr ShaderManager::LoadShader(const std::string& name, const std::string& vertexSrcPath, const std::string& fragSrcPath, const std::string& customVertCode, const std::string& customFragCode)
	{
		return LoadShader(name, Shader::CreateFromFiles(vertexSrcPath, fragSrcPath, customVertCode, customFragCode));
	}

	ShaderPtr ShaderManager::GetShader(const std::string& name)
	{
		return (instance().m_Shaders.count(name) > 0) ? instance().m_Shaders[name] : nullptr;
	}

	void ShaderManager::Init()
	{
		EE_CORE_INFO("Initializing ShaderManager.");
		// Create a default shader.
		EE_CORE_TRACE("(ShaderManager) : Creating default shader.");
		LoadShader(EE_DEFAULT_SHADER, Shader::CreateDefault());
		EE_CORE_INFO("Initialized ShaderManager.");
	}

	ShaderManager& ShaderManager::instance()
	{
		static ShaderManager instance;
		if (!instance.m_initialized)
		{
			instance.m_initialized = true;
			instance.Init();
		}
		return instance;
	}
}