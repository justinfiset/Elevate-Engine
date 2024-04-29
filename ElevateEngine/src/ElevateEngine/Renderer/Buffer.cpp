#include "eepch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "OpenGL/OpenGLBuffer.h"

namespace Elevate
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None: return nullptr; break; // TODO implement
		case RendererAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		EE_CORE_ASSERT(false, "A supported RendererAPI needs to be supported!")
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* vertices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None: return nullptr; break; // TODO implement
		case RendererAPI::OpenGL: return new OpenGLIndexBuffer(vertices, count);
		}

		EE_CORE_ASSERT(false, "A supported RendererAPI needs to be supported!")
	}
}