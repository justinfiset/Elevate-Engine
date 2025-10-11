#pragma once

#include <memory>
#include "ElevateEngine/Renderer/Buffer.h"
namespace Elevate
{
	class VertexArray
	{
	public:
		~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		virtual void ClearVertexBuffers();
		void SetSingleVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);

		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_vertexBuffers; }
		const std::shared_ptr<IndexBuffer> GetIndexBuffer() const { return m_indexBuffer; }

		static VertexArray* Create();
	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
	};
}