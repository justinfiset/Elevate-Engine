#include "eepch.h"
#include "OpenGLVertexArray.h"
#include "ElevateEngine/Renderer/Buffer.h"

#include <glad/glad.h>


Elevate::OpenGLVertexArray::OpenGLVertexArray()
{
	glCreateVertexArrays(1, &m_RendererID);
}

Elevate::OpenGLVertexArray::~OpenGLVertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void Elevate::OpenGLVertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void Elevate::OpenGLVertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void Elevate::OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	EE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer layout is empty, assertion failed");
	
	glBindVertexArray(m_RendererID);
	vertexBuffer->Bind();

	uint32_t index = 0;
	const auto& layout = vertexBuffer->GetLayout();
	for (const auto& element : layout)
	{
		LinkAttribute(index, element.Count, GetShaderDataAPIType(element.Type),
			element.Normalized, layout.GetStride(), (const void*)element.Offset);
		index++;
	}

	vertexBuffer->Unbind();
	m_VertexBuffers.push_back(vertexBuffer);
}

void Elevate::OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	glBindVertexArray(m_RendererID);
	indexBuffer->Bind();
	m_IndexBuffer = indexBuffer;
}

void Elevate::OpenGLVertexArray::LinkAttribute(uint32_t layout, uint32_t size, uint32_t type, bool normalized, uint32_t stride, const void* offset) const
{
	glEnableVertexAttribArray(layout);
	glVertexAttribPointer(layout, size, type, normalized ? GL_TRUE : GL_FALSE, stride, offset);
}
