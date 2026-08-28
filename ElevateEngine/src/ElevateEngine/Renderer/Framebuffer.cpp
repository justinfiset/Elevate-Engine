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

    Framebuffer* Framebuffer::Create(uint32_t width, uint32_t height, std::vector<TextureFormat> colorFormats, bool depthAsRenderbuffer, TextureType depthUsage)
    {
        if (width == 0 || height == 0)
        {
            EE_CORE_WARN("Framebuffer::Create - Attempted to create framebuffer with 0x0 dimensions. Skipping.");
            return nullptr;
        }

        size_t colorAttachmentCount = colorFormats.size();

        std::vector<TexturePtr> colorTextures;
        colorTextures.reserve(colorAttachmentCount);
        for (uint32_t i = 0; i < colorAttachmentCount; i++)
        {
            colorTextures.push_back(CreateColorTexture(width, height, colorFormats[i]));
        }

        TexturePtr depthTex = nullptr;
        if (!depthAsRenderbuffer)
        {
            depthTex = CreateDepthTexture(width, height, TextureFormat::DEPTH, depthUsage);
        }
        Framebuffer* buffer = Framebuffer::Create(colorTextures, depthTex, depthAsRenderbuffer);
        buffer->m_width = width;
        buffer->m_height = height;
        return buffer;
    }

    Framebuffer* Framebuffer::CreateDepthOnly(uint32_t width, uint32_t height, TextureFormat depthFormat, TextureType depthUsage)
    {
        if (width == 0 || height == 0)
        {
            EE_CORE_WARN("Framebuffer::Create - Attempted to create framebuffer with 0x0 dimensions. Skipping.");
            return nullptr;
        }

        TexturePtr depthTex = CreateDepthTexture(width, height, depthFormat, depthUsage);
        Framebuffer* buffer = Framebuffer::Create({ }, depthTex, false);
        buffer->m_width = width;
        buffer->m_height = height;
        return buffer;
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

    void Framebuffer::ClearAndUse()
    {
        Use();
        Clear();
    }

    void Framebuffer::Use()
    {
        Bind();
        UseViewport();
    }

    void Framebuffer::UseViewport()
    {
        Renderer::SetViewport(0, 0, GetWidth(), GetHeight());
    }

    TexturePtr Framebuffer::CreateColorTexture(uint32_t width, uint32_t height, TextureFormat colorFormat)
    {
        TextureMetadata colorMeta = TextureMetadataBuilder()
            .Name("FramebufferColorAttachment0")
            .size(width, height)
            .Format(colorFormat)
            .Usage(TextureType::Diffuse)
            .Source(TextureSource::RenderTarget)
            .State(TextureState::Ready)
            .Filter(TextureFilter::Linear, TextureFilter::Linear)
            .Wrap(TextureWrap::ClampToEdge, TextureWrap::ClampToEdge)
            .Mipmaps(false)
            .Build();

        return Texture::CreateFromData(nullptr, colorMeta);
    }

    TexturePtr Framebuffer::CreateDepthTexture(uint32_t width, uint32_t height, TextureFormat depthFormat, TextureType depthUsage)
    {
#if defined(EE_PLATFORM_WEB)
        // Linear is not supported in web
        TextureFilter filter = TextureFilter::Nearest;
#else
        TextureFilter filter = TextureFilter::Linear;
#endif

        TextureMetadata depthMeta = TextureMetadataBuilder()
            .Name("ShadowMapDepthAttachment")
            .size(width, height)
            .Format(depthFormat)
            .Usage(depthUsage)
            .Source(TextureSource::RenderTarget)
            .State(TextureState::Ready)
            .Filter(filter, filter)
            .Wrap(TextureWrap::ClampToBorder, TextureWrap::ClampToBorder)
            .Mipmaps(false)
            .Build();

        return Texture::CreateFromData(nullptr, depthMeta);
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

    TexturePtr Framebuffer::GetColorTexture(uint32_t index) const
    {
        if (index >= m_colorAttachments.size())
        {
            EE_CORE_WARN("Framebuffer::GetColorTexture - Index out of bounds: {0}", index);
            return nullptr;
        }

        if (TexturePtr tex = m_colorAttachments.at(index).Texture)
        {
            return tex;
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
