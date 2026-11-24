#include <ElevateEngine/Platform/OpenGL/Buffers/OpenGLBuffer.h>
#include <ElevateEngine/Renderer/RendererBackend.h>

import Elevate.Foundations.Assert;
import Elevate.Renderer.Buffers;
import Elevate.Renderer.Renderer;
import Elevate.Renderer.RendererAPI;

namespace Elevate::RendererBackend
{
	void RegisterBufferFactories()
	{
		VertexBuffer::SetFactory([](const void* vertices, uint32_t size) -> VertexBuffer* {
			switch (Renderer::GetAPI())
			{
			case RendererAPI::GraphicAPI::None: return nullptr;
			case RendererAPI::GraphicAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
			}

			Assert::That(false, "A supported RendererAPI needs to be supported!");
			return nullptr;
		});

		IndexBuffer::SetFactory([](const void* vertices, uint32_t count) -> IndexBuffer* {
			switch (Renderer::GetAPI())
			{
			case RendererAPI::GraphicAPI::None: return nullptr;
			case RendererAPI::GraphicAPI::OpenGL: return new OpenGLIndexBuffer(vertices, count);
			}

			Assert::That(false, "A supported RendererAPI needs to be supported!");
			return nullptr;
		});
	}
}
