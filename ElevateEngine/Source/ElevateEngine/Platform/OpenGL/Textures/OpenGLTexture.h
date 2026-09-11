#pragma once

#include <ElevateEngine/Renderer/Texture/Texture.h>

namespace Elevate
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const TextureMetadata& meta);
		OpenGLTexture(const void*, const TextureMetadata& meta);

		virtual void SetDataImpl(const void*) override;
		virtual void* GetNativeHandle() const override;

	private:
		virtual void Bind(uint32_t index = 0) override;
		virtual void Unbind() override;

	private:
		uint32_t m_textureID;
	};
}