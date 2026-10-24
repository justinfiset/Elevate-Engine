module;

#include <cstdint>

#include <ElevateEngine/Platform/OpenGL/Buffers/OpenGLBuffer.h>

module Elevate.Renderer.Buffers;

import Elevate.Foundations.Assert;
import Elevate.Renderer.Renderer;

namespace Elevate
{
	VertexBuffer* VertexBuffer::Create(const void* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None: return nullptr; break;
		case RendererAPI::GraphicAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		Assert::That(false, "A supported RendererAPI needs to be supported!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(const void* vertices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None: return nullptr; break;
		case RendererAPI::GraphicAPI::OpenGL: return new OpenGLIndexBuffer(vertices, count);
		}

		Assert::That(false, "A supported RendererAPI needs to be supported!");
		return nullptr;
	}
}