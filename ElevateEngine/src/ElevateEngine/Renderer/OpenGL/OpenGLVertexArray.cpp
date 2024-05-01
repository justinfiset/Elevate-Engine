#include "eepch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>


Elevate::OpenGLVertexArray::OpenGLVertexArray()
{
	glCreateVertexArrays(1, &m_RendererID);
	glBindVertexArray(m_RendererID);
}

void Elevate::OpenGLVertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void Elevate::OpenGLVertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void Elevate::OpenGLVertexArray::LinkAttribute(int32_t layout, uint32_t numComponent, uint32_t type, uint32_t stride, void* offset) const
{
	// TODO bind vbo and unbind after?
	glVertexAttribPointer(layout, numComponent, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
}
