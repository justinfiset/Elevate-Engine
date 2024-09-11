#include "eepch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

Elevate::OpenGLFrameBuffer::OpenGLFrameBuffer(float width, float height)
{
	// Creating and binding
	glGenFramebuffers(1, &m_FrameBufferId);
	Bind();

	// TODO USE FUNCTION CODE BELLOW TO PREVENT CODE REPETITION
	// Creating the texture
	glGenTextures(1, &m_TextureId);
	glBindTexture(GL_TEXTURE_2D, m_TextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureId, 0);

	// TODO SEPARER DANS UNE AUTRE CLASSE??????, VOIR SI ON ÉVITERAIS DU CODE EN DOUBLE
	// On peut aussi utiliser la fonction resize ci-dessous pour éviter la répétition
	// render buffer
	glGenRenderbuffers(1, &m_RenderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferId);

	// checking for errors
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		EE_CORE_ERROR("FrameBuffer is not complete");
	}

	Unbind();
	// UNBIND TEXTURE AND RENDER, MAYBE MOVE ELSEWHERE
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Elevate::OpenGLFrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);
}

void Elevate::OpenGLFrameBuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Elevate::OpenGLFrameBuffer::Rescale(float width, float height) const
{
	glBindTexture(GL_TEXTURE_2D, m_TextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureId, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferId);
}
