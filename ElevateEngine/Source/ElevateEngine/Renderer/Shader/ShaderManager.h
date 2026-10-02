#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include <ElevateEngine/Core/EEObjectPtr.h>

#define EE_DEFAULT_SHADER "default"

namespace Elevate {
	class Shader;

	class ShaderManager {
	public:
		static EEObjectPtr<Shader> LoadShader(const std::string& name, EEObjectPtr<Shader> shader);
		static EEObjectPtr<Shader> LoadShader(const std::string& name, const std::string& vertexSrcPath, const std::string& fragSrcPath);
		static EEObjectPtr<Shader> LoadShader(const std::string& name, const std::string& vertexSrcPath, const std::string&, const std::string& customVertCode, const std::string& customFragCode);

		static EEObjectPtr<Shader> GetShader(const std::string& name);

	private:
		ShaderManager() = default;
		void Init();
		static ShaderManager& instance();

		std::unordered_map<std::string, EEObjectPtr<Shader>> m_Shaders;

		bool m_initialized = false;
	};
}