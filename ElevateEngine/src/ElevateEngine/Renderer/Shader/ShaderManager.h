#pragma once
#include <ElevateEngine/Renderer/Shader/Shader.h>

namespace Elevate {
	class ShaderManager {
	public:
		static ShaderPtr LoadShader(std::string name, ShaderPtr shader);
		static ShaderPtr LoadShader(std::string name, std::string vertexSrcPath, std::string fragSrcPath);
		static ShaderPtr LoadShader(std::string name, std::string vertexSrcPath, std::string fragSrcPath, std::string customVertCode, std::string customFragCode);

		static ShaderPtr GetShader(std::string name);

	private:
		ShaderManager() = default;

		static ShaderManager& instance()
		{
			static ShaderManager instance;
			return instance;
		}

		std::unordered_map<std::string, ShaderPtr> m_Shaders;
	};
}