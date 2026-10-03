#include "eepch.h"
#include "ShaderManager.h"

import Elevate.Foundations;
#include <ElevateEngine/Foundations/Core.h> // todo remove this and switch

#include <ElevateEngine/Renderer/Shader/Shader.h>
#include <ElevateEngine/Renderer/Light/SceneLighting.h>

namespace Elevate {
	EEObjectPtr<Shader> ShaderManager::LoadShader(const std::string& name, EEObjectPtr<Shader> shader)
	{
		if (name == EE_DEFAULT_SHADER && GetShader(EE_DEFAULT_SHADER))
		{
			CoreLogger::Error("(ShaderManager::LoadShader) : You cannot create a shader with the name : default. This is reservez for the default shader for the whole engine.");
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

	EEObjectPtr<Shader> ShaderManager::LoadShader(const std::string& name, const std::string& vertexSrcPath, const std::string& fragSrcPath)
	{
		return LoadShader(name, Shader::CreateFromFiles(vertexSrcPath, fragSrcPath));
	}

	EEObjectPtr<Shader> ShaderManager::LoadShader(const std::string& name, const std::string& vertexSrcPath, const std::string& fragSrcPath, const std::string& customVertCode, const std::string& customFragCode)
	{
		return LoadShader(name, Shader::CreateFromFiles(vertexSrcPath, fragSrcPath, customVertCode, customFragCode));
	}

	EEObjectPtr<Shader> ShaderManager::GetShader(const std::string& name)
	{
		return (instance().m_Shaders.count(name) > 0) ? instance().m_Shaders[name] : nullptr;
	}

	void ShaderManager::Init()
	{
		CoreLogger::Info("Initializing ShaderManager.");
		// Create a default shader.
		CoreLogger::Trace("(ShaderManager) : Creating default shader.");

		std::string glslPointLightCountDefine = "#define NR_POINT_LIGHTS " + std::to_string(MAX_POINTLIGHT) + "\n";
		std::string glslSpotLightCountDefine = "#define NR_SPOT_LIGHTS " + std::to_string(MAX_SPOTLIGHT) + "\n";
		EEObjectPtr<Shader> defaultShader = Elevate::Shader::CreateFromFiles(
			"engine://Shaders/DefaultLitShader.vert",
			"engine://Shaders/DefaultLitShader.frag",
			EE_SHADER_HEADER,
			EE_SHADER_HEADER + glslPointLightCountDefine + glslSpotLightCountDefine
		);
		if (!defaultShader) {
			defaultShader = Shader::CreateDefault();
		}

		if (defaultShader)
		{
			LoadShader(EE_DEFAULT_SHADER, defaultShader);
		}
		else
		{
			CoreLogger::Error("(ShaderManager) : Failed to create the default shader.");
		}

		CoreLogger::Info("Initialized ShaderManager.");
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