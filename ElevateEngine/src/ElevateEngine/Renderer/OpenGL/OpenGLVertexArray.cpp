#include "eepch.h"
#include "OpenGLVertexArray.h"
#include "ElevateEngine/Renderer/Buffer.h"

#include <glad/glad.h>
#include <ElevateEngine/Core/Log.h>

#include <ElevateEngine/Renderer/GLDebug.h>

Elevate::OpenGLVertexArray::OpenGLVertexArray()
{
	GLCheck(glCreateVertexArrays(1, &m_RendererID));
}

Elevate::OpenGLVertexArray::~OpenGLVertexArray()
{
	GLCheck(glDeleteVertexArrays(1, &m_RendererID));
}

void Elevate::OpenGLVertexArray::Bind() const
{
	GLCheck(glBindVertexArray(m_RendererID));
}

void Elevate::OpenGLVertexArray::Unbind() const
{
	GLCheck(glBindVertexArray(0));
}

void Elevate::OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	EE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer layout is empty, assertion failed");

	Bind();               // bind VAO
	vertexBuffer->Bind(); // bind VBO

	m_VertexBuffers.push_back(vertexBuffer);

	uint32_t index = 0;
	const auto& layout = vertexBuffer->GetLayout();
	for (const auto& element : layout)
	{
		// TODO REMOVES THE LOGS FROM HERE
		EE_CORE_TRACE("Attrib {} | Count={} | Type={} | Stride={} | Offset={}",
			index, element.Count, GetDataAPIType(element.Type), layout.GetStride(), element.Offset);

		LinkAttribute(
			index, 
			element.Count, GetDataAPIType(element.Type),
			element.Normalized, 
			layout.GetStride(), 
			(const void*)(uintptr_t)element.Offset
		);
		
		index++;
	}
}

void Elevate::OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	Bind();
	indexBuffer->Bind();
	m_IndexBuffer = indexBuffer;
}

void Elevate::OpenGLVertexArray::LinkAttribute(uint32_t layout, uint32_t size, uint32_t type, bool normalized, uint32_t stride, const void* offset) const
{
	GLCheck(glEnableVertexAttribArray(layout));
	GLCheck(glVertexAttribPointer(layout, size, type, normalized ? GL_TRUE : GL_FALSE, stride, offset));
}