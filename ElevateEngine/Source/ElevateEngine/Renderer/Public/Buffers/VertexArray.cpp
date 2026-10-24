module;

#include <ElevateEngine/Platform/OpenGL/Buffers/OpenGLVertexArray.h>

module Elevate.Renderer.VertexArray;

import Elevate.Foundations;
import Elevate.Renderer.RendererAPI;

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
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::GraphicAPI::None: return nullptr; break;
		case RendererAPI::GraphicAPI::OpenGL: return new OpenGLVertexArray();
		}

		Assert::That(false, "A supported RendererAPI needs to be supported!");
		return nullptr;
	}
}
