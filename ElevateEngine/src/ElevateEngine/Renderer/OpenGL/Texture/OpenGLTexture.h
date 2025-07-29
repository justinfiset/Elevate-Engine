#pragma once

#include <ElevateEngine/Renderer/Texture/Texture.h>

namespace Elevate
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(unsigned char* data, TextureMetadata& meta);
		
		virtual void Bind(uint32_t index = 0) override;
		virtual void Unbind() override;
		virtual bool IsBound() const override;
		virtual void SetDataImpl(unsigned char* data) override;
		virtual void* GetNativeHandle() const override;
	private:
		bool m_isBound = false;
		uint32_t m_textureID;
	};
}