#include "eepch.h"
#include "FrameBuffer.h"

#include "OpenGL/OpenGLFrameBuffer.h"
#include <ElevateEngine/Renderer/Renderer.h>

namespace Elevate
{
	FrameBuffer* FrameBuffer::Create(uint32_t width, uint32_t height)
	{
		TextureMetadata meta = TextureMetadataBuilder()
			.Size(width, height)
			.Filter(TextureFilter::Nearest, TextureFilter::Nearest)
			.Format(TextureFormat::RGBA)
			.Mipmaps(false)
			.Source(TextureSource::RenderTarget)
			.State(TextureState::Ready)
			.Usage(TextureType::Diffuse)
			.Wrap(TextureWrap::ClampToEdge, TextureWrap::ClampToEdge)
			.Build();

		return new OpenGLFrameBuffer(Texture::CreateFromData(nullptr, meta));
	}

	void FrameBuffer::Clear() const
	{
		Renderer::SetClearColor(m_clearColor);
		Renderer::Clear();
	}
}
