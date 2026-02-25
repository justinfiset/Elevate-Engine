#include "eepch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "OpenGL/OpenGLBuffer.h"

namespace Elevate
{
	VertexBuffer* VertexBuffer::Create(void* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None: return nullptr; break;
		case RendererAPI::GraphicAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		EE_CORE_ASSERT(false, "A supported RendererAPI needs to be supported!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(void* vertices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None: return nullptr; break;
		case RendererAPI::GraphicAPI::OpenGL: return new OpenGLIndexBuffer(vertices, count);
		}

		EE_CORE_ASSERT(false, "A supported RendererAPI needs to be supported!");
		return nullptr;
	}
}