#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#define EE_DEFAULT_SHADER "default"

namespace Elevate {
	class Shader;
	using ShaderPtr = std::shared_ptr<Shader>;

	class ShaderManager {
	public:
		static ShaderPtr LoadShader(const std::string& name, ShaderPtr shader);
		static ShaderPtr LoadShader(const std::string& name, const std::string& vertexSrcPath, const std::string& fragSrcPath);
		static ShaderPtr LoadShader(const std::string& name, const std::string& vertexSrcPath, const std::string&, const std::string& customVertCode, const std::string& customFragCode);

		static ShaderPtr GetShader(const std::string& name);

	private:
		ShaderManager() = default;
		void Init();
		static ShaderManager& instance();

		std::unordered_map<std::string, ShaderPtr> m_Shaders;

		bool m_initialized = false;
	};
}