#pragma once

#include "ElevateEngine/Renderer/Buffer.h"

namespace Elevate
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(void* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		virtual const BufferLayout& GetLayout() const override { return m_Layout; }

		virtual void Resize(uint32_t size) override;

	private:
		BufferLayout m_Layout;
		unsigned int m_RendererID;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(void* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual unsigned int GetCount() const { return m_Count; }

		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		unsigned int m_Count;
		unsigned int m_RendererID;
	};
}
