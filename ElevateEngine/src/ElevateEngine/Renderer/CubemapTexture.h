#pragma once

#include <string>

namespace Elevate
{
	class CubemapTexture
	{
	public:
		~CubemapTexture() { };

		virtual void Bind(int index = 0) const = 0;
		virtual void Unbind(int index = 0) const = 0;

		static CubemapTexture* Create(std::string path);

		inline uint32_t GetID() const { return m_textureID; }
	protected:
		uint32_t m_textureID; // todo maybe move to the opengl texture class if not needed in other APIs
	};
}