#include <ElevateEngine/Platform/OpenGL/Buffers/OpenGLVertexArray.h>
#include <ElevateEngine/Renderer/RendererBackend.h>

import Elevate.Foundations.Assert;
import Elevate.Renderer.RendererAPI;
import Elevate.Renderer.VertexArray;

namespace Elevate::RendererBackend
{
	void RegisterVertexArrayFactory()
	{
		VertexArray::SetFactory([]() -> VertexArray* {
			switch (RendererAPI::GetAPI())
			{
			case RendererAPI::GraphicAPI::None: return nullptr;
			case RendererAPI::GraphicAPI::OpenGL: return new OpenGLVertexArray();
			}

			Assert::That(false, "A supported RendererAPI needs to be supported!");
			return nullptr;
		});
	}
}
