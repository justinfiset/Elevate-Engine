#include "eepch.h"
#include "OpenGLFrameBuffer.h"
#include "ElevateEngine/Renderer/Renderer.h"

#include <glad/glad.h>

Elevate::OpenGLFrameBuffer::OpenGLFrameBuffer(TexturePtr tex) : FrameBuffer(tex)
{
	EE_CORE_ASSERT(tex->GetWidth() > 0 && tex->GetHeight() > 0, "Framebuffer dimensions must be positive");

	// Creating and binding
	glGenFramebuffers(1, &m_FrameBufferId);
	Bind();

	m_TextureId = static_cast<GLuint>(reinterpret_cast<intptr_t>(tex->GetNativeHandle()));

	// Bind the texture with the Frame Buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureId, 0);
	
	// render buffer
	glGenRenderbuffers(1, &m_RenderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, tex->GetWidth(), tex->GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferId);

	CheckCompleteness();
	Unbind();
}

void Elevate::OpenGLFrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);
}

void Elevate::OpenGLFrameBuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Elevate::OpenGLFrameBuffer::Rescale(uint32_t width, uint32_t height)
{
	if (width == m_texture->GetWidth() && height == m_texture->GetHeight()) return;
	else if (width <= 0 || height <= 0) return;

	Bind();

	m_texture->Bind();

	TextureMetadata meta = m_texture->GetMetadata();
	meta.Width = width;
	meta.Height = height;

	m_texture->SetData(nullptr, meta);

	// Attacher la texture redimensionnée au framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureId, 0);

	// Redimensionner le renderbuffer pour le depth/stencil
	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0); // Unbind renderbuffer

	CheckCompleteness();
	Unbind();
}

bool Elevate::OpenGLFrameBuffer::CheckCompleteness() const
{
	uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		const char* msg = GetFramebufferStatusString(status);
		EE_CORE_ERROR("Framebuffer error: {}", msg);
		return false;
	}
	return true;
}

const char* Elevate::OpenGLFrameBuffer::GetFramebufferStatusString(uint32_t status) const
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

