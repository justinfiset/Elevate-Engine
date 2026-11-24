module;


#include <string>
#include <unordered_map>
#include <memory>

export module Elevate.Renderer.Shaders:ShaderManager;

import :Fwd;

// Foundations
import Elevate.Foundations.Guid;
// Core
import Elevate.Core.Objects.ObjectPtr;

export namespace Elevate
{
	inline constexpr const char* EE_DEFAULT_SHADER = "default";
	inline constexpr const char* EE_ERROR_SHADER = "error";

	inline const Guid EE_DEFAULT_SHADER_GUID = Guid::FromString("00000000-0000-0000-0000-000000000001");
	inline const Guid EE_ERROR_SHADER_GUID = Guid::FromString("00000000-0000-0000-0000-000000000002");

	class ShaderManager
	{
	public:
		ShaderManager() = default;

		static void Init()
		{
			instance()._Init();
		}

		static EEObjectPtr<Shader> LoadShader(const std::string& name, EEObjectPtr<Shader> shader);
		static EEObjectPtr<Shader> LoadShader(const std::string& name, const std::string& vertexSrcPath, const std::string& fragSrcPath);
		static EEObjectPtr<Shader> LoadShader(const std::string& name, const std::string& vertexSrcPath, const std::string&, const std::string& customVertCode, const std::string& customFragCode);

		static EEObjectPtr<Shader> GetShader(const std::string& name);

	private:
		void _Init();

		static ShaderManager& instance(); // Temporary : remove once passed to the dependency injection

		std::unordered_map<std::string, EEObjectPtr<Shader>> m_Shaders;

		bool m_initialized = false;
	};
}