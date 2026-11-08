module;

#include <ElevateEngine/Renderer/GraphicsAPI.h>

#include <ElevateEngine/Platform/OpenGL/Framebuffer/OpenGLFramebuffer.h>

module Elevate.Renderer.Framebuffers;

import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Assert;
import Elevate.Renderer.Renderer;
import Elevate.Renderer.RendererAPI;

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
			CoreLogger::Warn("Framebuffer::Create - Attempted to create framebuffer with 0x0 dimensions. Skipping.");
			return nullptr;
		}

		size_t colorAttachmentCount = colorFormats.size();

		std::vector<std::shared_ptr<Texture>> colorTextures;
		colorTextures.reserve(colorAttachmentCount);
		for (uint32_t i = 0; i < colorAttachmentCount; i++)
		{
			colorTextures.push_back(CreateColorTexture(width, height, colorFormats[i]));
		}

		std::shared_ptr<Texture> depthTex = nullptr;
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
			CoreLogger::Warn("Framebuffer::Create - Attempted to create framebuffer with 0x0 dimensions. Skipping.");
			return nullptr;
		}

		std::shared_ptr<Texture> depthTex = CreateDepthTexture(width, height, depthFormat, depthUsage);
		Framebuffer* buffer = Framebuffer::Create({ }, depthTex, false);
		buffer->m_width = width;
		buffer->m_height = height;
		return buffer;
	}

	Framebuffer* Framebuffer::Create(const std::vector<std::shared_ptr<Texture>>& colorTextures, std::shared_ptr<Texture> depthTexture, bool depthAsRenderbuffer)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None:
			Assert::That(false, "GraphicsAPI::None is not supported!");
			return nullptr;

		case RendererAPI::GraphicAPI::OpenGL:
			return new OpenGLFramebuffer(colorTextures, depthTexture, depthAsRenderbuffer);
		}

		Assert::That(false, "Unknown GraphicsAPI!");
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

	std::shared_ptr<Texture> Framebuffer::CreateColorTexture(uint32_t width, uint32_t height, TextureFormat colorFormat)
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

	std::shared_ptr<Texture> Framebuffer::CreateDepthTexture(uint32_t width, uint32_t height, TextureFormat depthFormat, TextureType depthUsage)
	{
#if defined(EE_PLATFORM_WEB)
		// Linear is not supported in web
		TextureFilter filter = TextureFilter::Nearest;
		TextureWrap depthWrap = TextureWrap::ClampToEdge;
#else
		TextureFilter filter = TextureFilter::Linear;
		TextureWrap depthWrap = TextureWrap::ClampToBorder;
#endif

		TextureMetadata depthMeta = TextureMetadataBuilder()
			.Name("FramebufferDepthAttachment")
			.size(width, height)
			.Format(depthFormat)
			.Usage(depthUsage)
			.Source(TextureSource::RenderTarget)
			.State(TextureState::Ready)
			.Filter(filter, filter)
			.Wrap(depthWrap, depthWrap)
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
			CoreLogger::Warn("Framebuffer::GetColorAttachmentHandle - Index out of bounds: {0}", index);
			return nullptr;
		}

		if (std::shared_ptr<Texture> tex = m_colorAttachments.at(index).Tex)
		{
			return tex->GetNativeHandle();
		}
		return nullptr;
	}

	void* Framebuffer::GetDepthAttachmentHandle() const
	{
		if (m_depthAttachment.has_value() && m_depthAttachment->Tex)
		{
			return m_depthAttachment->Tex->GetNativeHandle();
		}
		return nullptr;
	}

	std::shared_ptr<Texture> Framebuffer::GetColorTexture(uint32_t index) const
	{
		if (index >= m_colorAttachments.size())
		{
			CoreLogger::Warn("Framebuffer::GetColorTexture - Index out of bounds: {0}", index);
			return nullptr;
		}

		if (std::shared_ptr<Texture> tex = m_colorAttachments.at(index).Tex)
		{
			return tex;
		}
		return nullptr;
	}

	std::shared_ptr<Texture> Framebuffer::GetDepthTexture() const
	{
		if (m_depthAttachment.has_value())
		{
			return m_depthAttachment->Tex;
		}
		return nullptr;
	}
}
