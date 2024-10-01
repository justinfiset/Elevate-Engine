#include "eepch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

Elevate::OpenGLFrameBuffer::OpenGLFrameBuffer(uint32_t width, uint32_t height)
	: m_Width(width), m_Height(height), m_FrameBufferId(0), m_TextureId(0), m_RenderBufferId(0)
{
	// Creating and binding
	glGenFramebuffers(1, &m_FrameBufferId);
	Bind();

	// TODO REMOVE FOR TESTING PRUPUSES ONLY
	//glEnable(GL_MULTISAMPLE);

	// TODO USE FUNCTION CODE BELLOW TO PREVENT CODE REPETITION
	// Create color texture
	glGenTextures(1, &m_TextureId); // Bind texture
	glBindTexture(GL_TEXTURE_2D, m_TextureId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // TODO REMOVE
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	// todo maybe use GL_NEAREST INSTEAD OF LINEAR // set en bas aussi au besoin si on modifie
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind teture
	// Bind the texture with the Frame Buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureId, 0);

	// TODO SEPARER DANS UNE AUTRE CLASSE??????, VOIR SI ON ÉVITERAIS DU CODE EN DOUBLE
	// On peut aussi utiliser la fonction resize ci-dessous pour éviter la répétition
	
	// render buffer
	glGenRenderbuffers(1, &m_RenderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0); // Unbind
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferId);

	// checking for completness (erros) 
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		EE_CORE_ERROR("FrameBuffer is not complete");
	}

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

void Elevate::OpenGLFrameBuffer::Clear() const
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Elevate::OpenGLFrameBuffer::Rescale(uint32_t width, uint32_t height)
{
	Bind();

	m_Width = width;
	m_Height = height;

	// Redimensionner la texture liée au framebuffer
	glBindTexture(GL_TEXTURE_2D, m_TextureId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture

	// Attacher la texture redimensionnée au framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureId, 0);

	// Redimensionner le renderbuffer pour le depth/stencil
	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0); // Unbind renderbuffer

	// Vérification du framebuffer
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		EE_CORE_ERROR("FrameBuffer is not complete after resizing");
	}

	Unbind();
}

