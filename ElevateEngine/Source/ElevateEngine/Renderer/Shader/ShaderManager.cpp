module;

#include <string>

module Elevate.Renderer.Shaders;

import Elevate.Foundations.CoreLogger;
import Elevate.Core.Objects.ObjectPtr;
import Elevate.Core.Assets.AssetRegistry;
import Elevate.Renderer.Scene.Lighting;

import :Shader;
import :ShaderAsset;

namespace Elevate
{
	EEObjectPtr<Shader> ShaderManager::LoadShader(const std::string& name, EEObjectPtr<Shader> shader)
	{
		if (name == EE_DEFAULT_SHADER && GetShader(EE_DEFAULT_SHADER))
		{
			CoreLogger::Error("(ShaderManager::LoadShader) : You cannot create a shader with the name : default. This is reserved for the default shader for the whole engine.");
			return nullptr;
		}

		if (name == EE_ERROR_SHADER && GetShader(EE_ERROR_SHADER))
		{
			CoreLogger::Error("(ShaderManager::LoadShader) : You cannot create a shader with the name : error. This is reserved for the default error shader for the whole engine.");
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

	void ShaderManager::_Init()
	{
		if (m_initialized)
		{
			return;
		}

		m_initialized = true;

		CoreLogger::Info("Initializing ShaderManager.");

		std::string glslPointLightCountDefine = "#define NR_POINT_LIGHTS " + std::to_string(SceneLightingLimits::MaxPointLights) + "\n";
		std::string glslSpotLightCountDefine = "#define NR_SPOT_LIGHTS " + std::to_string(SceneLightingLimits::MaxSpotLights) + "\n";
		EEObjectPtr<Shader> defaultShader = Elevate::Shader::CreateFromFiles(
			"engine://Shaders/DefaultLitShader.vert",
			"engine://Shaders/DefaultLitShader.frag",
			"",
			glslPointLightCountDefine + glslSpotLightCountDefine
		);

		if (!defaultShader)
		{
			defaultShader = Shader::CreateDefault();
		}

		if (defaultShader)
		{
			LoadShader(EE_DEFAULT_SHADER, defaultShader);

			// Create the asset corresponsing to the default shader
			auto shaderAsset = ShaderAsset::CreateBuiltIn("Default", EE_DEFAULT_SHADER_GUID, defaultShader);
			AssetRegistry::RegisterAsset(shaderAsset);

			CoreLogger::Trace("(ShaderManager) : Created the default shader.");
		}
		else
		{
			CoreLogger::Error("(ShaderManager) : Failed to create the default shader.");
		}

		LoadShader(EE_ERROR_SHADER, Shader::CreateDefaultErrorNative());

		CoreLogger::Info("Initialized ShaderManager.");
	}

	ShaderManager& ShaderManager::instance()
	{
		static ShaderManager instance;
		return instance;
	}
}