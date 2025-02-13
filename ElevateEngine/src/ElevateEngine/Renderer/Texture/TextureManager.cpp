#include "eepch.h"
#include "TextureManager.h"

namespace Elevate {
	TexturePtr Elevate::TextureManager::LoadTexture(TexturePtr texture)
	{
		// TODO MAKE SURE TO HAVE THE ABSOLUTE PATH TO PREVENT MULTIPLE INCLUDES OR THE SAME TEXTURE WITH DIFF PATHS!!!
		if (instance().m_Textures.count(texture->GetPath()) > 0)
		{
			return instance().m_Textures[texture->GetPath()];
		}
		else
		{
			instance().m_Textures[texture->GetPath()] = texture;
			return texture;
		}
	}

	TexturePtr TextureManager::LoadTexture(std::string path)
	{
		return LoadTexture(Texture::Create(path));
	}

	TexturePtr TextureManager::GetTexture(std::string path)
	{
		return (instance().m_Textures.count(path) > 0) ? instance().m_Textures[path] : nullptr;
	}
}