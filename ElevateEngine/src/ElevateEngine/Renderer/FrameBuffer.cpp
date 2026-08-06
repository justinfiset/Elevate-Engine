#include "eepch.h"
#include "Framebuffer.h"

#include <ElevateEngine/Core/Log.h>
#include <ElevateEngine/Core/Assert.h>
#include <ElevateEngine/Renderer/GraphicsAPI.h>
#include <ElevateEngine/Renderer/Renderer.h>

#include <ElevateEngine/Renderer/OpenGL/OpenGLFramebuffer.h>

namespace Elevate
{
    uint32_t Framebuffer::GetWidth() const
    {
        return m_width;
    }

    uint32_t Framebuffer::GetHeight() const
    {
        return m_height;
    }

    Framebuffer* Framebuffer::Create(uint32_t width, uint32_t height)
    {
        switch (Renderer::GetAPI())
        {
           case RendererAPI::GraphicAPI::None:
                EE_CORE_ASSERT(false, "GraphicsAPI::None is not supported!");
                return nullptr;

            case RendererAPI::GraphicAPI::OpenGL:
            {
                TextureMetadata colorMeta = TextureMetadataBuilder()
                    .Name("FramebufferColorAttachment0")
                    .size(width, height)
                    .Format(TextureFormat::RGBA)
                    .Usage(TextureType::Diffuse)
                    .Source(TextureSource::RenderTarget)
                    .State(TextureState::Ready)
                    .Filter(TextureFilter::Linear, TextureFilter::Linear)
                    .Wrap(TextureWrap::ClampToEdge, TextureWrap::ClampToEdge)
                    .Mipmaps(false)
                    .Build();

                TexturePtr colorTex = Texture::CreateFromData(nullptr, colorMeta);
                Framebuffer* buffer = new OpenGLFrameBuffer({ colorTex }, nullptr, true);
                buffer->m_width = width;
                buffer->m_height = height;
                return buffer;
            }
        }

        EE_CORE_ASSERT(false, "Unknown GraphicsAPI!");
        return nullptr;
    }

    Framebuffer* Framebuffer::CreateDepthOnly(uint32_t width, uint32_t height)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::GraphicAPI::None:
                EE_CORE_ASSERT(false, "GraphicsAPI::None is not supported!");
                return nullptr;

            case RendererAPI::GraphicAPI::OpenGL:
            {
                TextureMetadata depthMeta = TextureMetadataBuilder()
                    .Name("ShadowMapDepthAttachment")
                    .size(width, height)
                    .Format(TextureFormat::DEPTH)
                    .Usage(TextureType::ShadowMap)
                    .Source(TextureSource::RenderTarget)
                    .State(TextureState::Ready)
                    .Filter(TextureFilter::Linear, TextureFilter::Linear)
                    .Wrap(TextureWrap::ClampToBorder, TextureWrap::ClampToBorder)
                    .Mipmaps(false)
                    .Build();

                TexturePtr depthTex = Texture::CreateFromData(nullptr, depthMeta);
                Framebuffer* buffer = new OpenGLFrameBuffer({}, depthTex, false);
                buffer->m_width = width;
                buffer->m_height = height;
                return buffer;
            }
        }

        EE_CORE_ASSERT(false, "Unknown GraphicsAPI!");
        return nullptr;
    }

    Framebuffer* Framebuffer::Create(const std::vector<TexturePtr>& colorTextures, TexturePtr depthTexture, bool depthAsRenderbuffer)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::GraphicAPI::None:
            EE_CORE_ASSERT(false, "GraphicsAPI::None is not supported!");
            return nullptr;

        case RendererAPI::GraphicAPI::OpenGL:
            return new OpenGLFrameBuffer(colorTextures, depthTexture, depthAsRenderbuffer);
        }

        EE_CORE_ASSERT(false, "Unknown GraphicsAPI!");
        return nullptr;
    }

	void Framebuffer::Clear() const
	{
		Renderer::SetClearColor(m_clearColor);
		Renderer::Clear();
	}

	void* Framebuffer::GetColorAttachmentHandle(uint32_t index) const
	{
		if (index >= m_colorAttachments.size())
		{
			EE_CORE_WARN("Framebuffer::GetColorAttachmentHandle - Index out of bounds: {0}", index);
			return nullptr;
		}

		if (TexturePtr tex = m_colorAttachments.at(index).Texture)
		{
			return tex->GetNativeHandle();
		}
		return nullptr;
	}

	void* Framebuffer::GetDepthAttachmentHandle() const
	{
		if (m_depthAttachment.has_value() && m_depthAttachment->Texture)
		{
			return m_depthAttachment->Texture->GetNativeHandle();
		}
		return nullptr;
	}

    TexturePtr Framebuffer::GetDepthTexture() const
    {
        if (m_depthAttachment.has_value())
        {
            return m_depthAttachment->Texture;
        }
        return nullptr;
    }
}
