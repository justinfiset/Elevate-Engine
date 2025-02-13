#pragma once
#include <ElevateEngine/Renderer/Texture/Texture.h>
#include <unordered_map>

namespace Elevate 
{
	class TextureManager
	{
	public:
		static TexturePtr LoadTexture(TexturePtr texture);
		static TexturePtr LoadTexture(std::string path);

		static TexturePtr GetTexture(std::string path);

	private:
		TextureManager() = default;

		static TextureManager& instance()
		{
			static TextureManager instance;
			return instance;
		}

		std::unordered_map<std::string, TexturePtr> m_Textures;
	};
}