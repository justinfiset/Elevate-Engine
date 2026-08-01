#include "eepch.h"
#include "OpenGLFrameBuffer.h"

#include <ElevateEngine/Core/Assert.h>
#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Renderer/GLDebug.h>
#include <ElevateEngine/Renderer/GraphicsAPI.h>

namespace Elevate
{
	constexpr GLenum ToOpenGL(TextureFilter filter) {
		switch (filter) {
		case TextureFilter::Nearest: return GL_NEAREST;
		case TextureFilter::Linear:  return GL_LINEAR;
		default:                     return GL_NEAREST;
		}
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(const std::vector<TexturePtr>& colorTextures, TexturePtr depthTexture, bool depthAsRenderbuffer)
	{
		GLCheck(glGenFramebuffers(1, &m_frameBufferId));
		Bind();

		if (!colorTextures.empty())
		{
			std::vector<GLenum> drawBuffers;

			for (size_t i = 0; i < colorTextures.size(); ++i)
			{
				auto& tex = colorTextures[i];
				EE_CORE_ASSERT(tex->GetWidth() > 0 && tex->GetHeight() > 0, "Framebuffer texture dimensions must be positive");

				GLuint texId = static_cast<GLuint>(reinterpret_cast<intptr_t>(tex->GetNativeHandle()));
				GLenum attachmentPoint = GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(i);

				GLCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentPoint, GL_TEXTURE_2D, texId, 0));

				m_colorAttachments.emplace_back(tex, static_cast<uint32_t>(i));
				drawBuffers.push_back(attachmentPoint);
			}

			GLCheck(glDrawBuffers(static_cast<GLsizei>(drawBuffers.size()), drawBuffers.data()));
		}
		else // If there is no color buffer :
		{
			GLCheck(glDrawBuffer(GL_NONE));
			GLCheck(glReadBuffer(GL_NONE));
		}

		if (depthTexture)
		{
			GLuint depthId = static_cast<GLuint>(reinterpret_cast<intptr_t>(depthTexture->GetNativeHandle()));
			// Use the texture format to check if we only use depth and/or stencil
			GLenum attachmentType = (depthTexture->GetMetadata().Format == TextureFormat::DEPTHSTENCIL)
				? GL_DEPTH_STENCIL_ATTACHMENT
				: GL_DEPTH_ATTACHMENT;

			GLCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, depthId, 0));
			if (depthTexture->GetMetadata().WrapS == TextureWrap::ClampToBorder)
			{
				GLCheck(glBindTexture(GL_TEXTURE_2D, depthId));
				float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				GLCheck(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));
				GLCheck(glBindTexture(GL_TEXTURE_2D, 0));
			}

			m_depthAttachment = FramebufferDepthAttachment{ depthTexture, false };
		}
		else if (depthAsRenderbuffer)
		{
			uint32_t w = colorTextures.empty() ? 1280 : colorTextures[0]->GetWidth();
			uint32_t h = colorTextures.empty() ? 720 : colorTextures[0]->GetHeight();

			GLCheck(glGenRenderbuffers(1, &m_renderBufferId));
			GLCheck(glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferId));
			GLCheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h));
			GLCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBufferId));
			GLCheck(glBindRenderbuffer(GL_RENDERBUFFER, 0));

			m_depthAttachment = FramebufferDepthAttachment{ nullptr, true };
		}

		CheckCompleteness();
		Unbind();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		// todo use this
		//GLCheck(glDeleteFramebuffers(1, &m_frameBufferId));
		//if (m_renderBufferId != 0)
		//{
		//	GLCheck(glDeleteRenderbuffers(1, &m_renderBufferId));
		//}
	}

	void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
	}

	void OpenGLFrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::BlitFramebufferToScreen(uint32_t screenWidth, uint32_t screenHeight) const
	{
		if (m_colorAttachments.empty())
		{
			EE_CORE_WARN("Cannot blit a framebuffer without color attachments to screen!");
			return;
		}

		GLCheck(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBufferId)); // Read from this framebuffer
		GLCheck(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0)); // Draw to the main framebuffer

		uint32_t srcW = !m_colorAttachments.empty() ? m_colorAttachments[0].Texture->GetWidth() : 1280;
		uint32_t srcH = !m_colorAttachments.empty() ? m_colorAttachments[0].Texture->GetHeight() : 720;

		// Use the correct filter
		bool isMagnifying = (screenWidth >= srcW) && (screenHeight >= srcH);
		auto filterType = isMagnifying
			? m_colorAttachments[0].Texture->GetMetadata().MagFilter
			: m_colorAttachments[0].Texture->GetMetadata().MinFilter;
		GLenum filter = ToOpenGL(filterType);
		if (filter != GL_NEAREST && filter != GL_LINEAR)
		{
			filter = GL_LINEAR; // Fallback
		}

		GLCheck(glBlitFramebuffer(
			0, 0, srcW, srcH,
			0, 0, screenWidth, screenHeight,
			GL_COLOR_BUFFER_BIT, 
			filter
		));

		GLCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void OpenGLFrameBuffer::Rescale(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0) return;

		Bind();

		for (auto& attachment : m_colorAttachments)
		{
			if (auto& tex = attachment.Texture)
			{
				if (tex->GetWidth() == width && tex->GetHeight() == height) continue;

				TextureMetadata meta = tex->GetMetadata();
				meta.Width = width;
				meta.Height = height;
				tex->SetData(nullptr, meta);

				GLuint texId = static_cast<GLuint>(reinterpret_cast<intptr_t>(tex->GetNativeHandle()));
				GLCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment.Index, GL_TEXTURE_2D, texId, 0));
			}
		}

		if (m_depthAttachment.has_value())
		{
			if (m_depthAttachment->Texture)
			{
				auto& depthTex = m_depthAttachment->Texture;
				TextureMetadata meta = depthTex->GetMetadata();
				meta.Width = width;
				meta.Height = height;
				depthTex->SetData(nullptr, meta);

				GLuint depthId = static_cast<GLuint>(reinterpret_cast<intptr_t>(depthTex->GetNativeHandle()));
				GLenum attachmentType = (meta.Format == TextureFormat::DEPTHSTENCIL)
					? GL_DEPTH_STENCIL_ATTACHMENT
					: GL_DEPTH_ATTACHMENT;

				GLCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, depthId, 0));
			}
			else if (m_depthAttachment->IsRenderbuffer && m_renderBufferId != 0)
			{
				GLCheck(glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferId));
				GLCheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
				GLCheck(glBindRenderbuffer(GL_RENDERBUFFER, 0));
			}
		}

		CheckCompleteness();
		Unbind();
	}

	bool OpenGLFrameBuffer::CheckCompleteness() const
	{
		uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			const char* msg = GetFramebufferStatusString(status);
			EE_CORE_ERROR("Framebuffer error: {}", msg);
			return false;
		}
		return true;
	}

	const char* OpenGLFrameBuffer::GetFramebufferStatusString(uint32_t status) const
	{
		switch (status)
		{
		case GL_FRAMEBUFFER_COMPLETE: return "COMPLETE";
		case GL_FRAMEBUFFER_UNDEFINED: return "UNDEFINED";
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: return "INCOMPLETE_ATTACHMENT";
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return "MISSING_ATTACHMENT";
		case GL_FRAMEBUFFER_UNSUPPORTED: return "UNSUPPORTED_FORMAT";
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: return "INCOMPLETE_MULTISAMPLE";
		default: return "UNKNOWN_ERROR";
		}
	}
}
