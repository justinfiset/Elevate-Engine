module;

#include <ElevateEngine/Core/Scene/SceneEnTTOperators.h>

module Elevate.Renderer.VertexArray;

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
}
