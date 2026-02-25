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

		virtual void SetLayout(const BufferLayout& layout) override { m_layout = layout; }
		virtual const BufferLayout& GetLayout() const override { return m_layout; }

		virtual void SetData(void* newData, uint32_t size) override;
		virtual void Resize(uint32_t size) override;

	private:
		BufferLayout m_layout;
		unsigned int m_rendererID;
		void* data;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(void* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual unsigned int GetCount() const { return m_count; }

		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		unsigned int m_count;
		unsigned int m_rendererID;
	};
}
