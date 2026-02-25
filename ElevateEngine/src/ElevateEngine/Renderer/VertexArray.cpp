#include "eepch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "OpenGL/OpenGLVertexArray.h"

namespace Elevate
{
	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		if (vertexBuffer)
		{
			m_vertexBuffers.push_back(vertexBuffer);
		}
	}

	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		if (indexBuffer)
		{
			m_indexBuffer = indexBuffer;
		}
	}

	void VertexArray::ClearVertexBuffers()
	{
		m_vertexBuffers.clear();
	}

	void VertexArray::SetSingleVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		ClearVertexBuffers();
		AddVertexBuffer(vertexBuffer);
	}

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None: return nullptr; break;
		case RendererAPI::GraphicAPI::OpenGL: return new OpenGLVertexArray();
		}

		EE_CORE_ASSERT(false, "A supported RendererAPI needs to be supported!");
		return nullptr;
	}
}
