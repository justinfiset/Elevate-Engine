module;

#include "Reflection.h"

#include <memory>
#include <string>

export module Elevate.Renderer.Shaders:ShaderAsset;

import Elevate.Core.Assets.Asset;
import Elevate.Core.Objects.ObjectPtr;

import :Shader;
import :ShaderManager;

export namespace Elevate
{
	class ShaderAsset : public Asset
	{
		BEGIN_OBJECT(ShaderAsset, EE_Asset("Shader", ".shader", EE_CreateAssetMenu, EE_AssetColor(0.2f, 0.6f, 0.9f)));

		EEObjectPtr<Shader> GetShader()
		{
			if (!m_compiled)
			{
				Compile();
			}

			return m_compiled;
		}

		/// @brief Gets the built-in status of the shader asset.
		bool IsBuiltIn() const { return m_isBuiltIn; }

		/// @brief Creates a new built-in shader asset.
		static std::shared_ptr<ShaderAsset> CreateBuiltIn(const std::string& name, EEObjectPtr<Shader> compiled)
		{
			auto asset = std::make_shared<ShaderAsset>();
			asset->m_compiled = compiled;
			asset->m_isBuiltIn = true;
			return asset;
		}

	private:
		/// @brief Compiles the shader asset into a Shader object.
		void Compile()
		{
			m_compiled = ShaderManager::LoadShader(GetName(), m_vertexPath, m_fragmentPath);
		}

	private:
		std::string m_vertexPath = "";
		PROPERTY(m_vertexPath);

		std::string m_fragmentPath = "";
		PROPERTY(m_fragmentPath);

		/// @brief Indicates whether this shader asset is a built-in shader or not. Built-in shaders are not loaded from disk and are instead compiled from source code embedded in the engine.
		bool m_isBuiltIn = false;

		EEObjectPtr<Shader> m_compiled{ nullptr };

		END_OBJECT();
	};
}
