#pragma once
#include "ElevateEngine/Renderer/Buffer.h"
#include "ElevateEngine/Renderer/VertexArray.h"

namespace Elevate
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer> GetIndexBuffer() const { return m_IndexBuffer; }

		void LinkAttribute(uint32_t layout, uint32_t size, uint32_t type, bool normalized, uint32_t stride, const void* offset) const;

	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;

		unsigned int m_RendererID;
	};
}