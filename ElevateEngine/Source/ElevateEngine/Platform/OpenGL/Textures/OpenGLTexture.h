#pragma once

#include <cstdint>

import Elevate.Renderer.Texture;

namespace Elevate
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const TextureMetadata& meta);
		OpenGLTexture(const void*, const TextureMetadata& meta);

		void EnsureCreated();

		void SetDataImpl(const void*) override;
		void* GetNativeHandle() const override;

	private:
		void Bind(uint32_t index = 0) override;
		void Unbind() override;

	private:
		uint32_t m_textureID = 0;
	};
}