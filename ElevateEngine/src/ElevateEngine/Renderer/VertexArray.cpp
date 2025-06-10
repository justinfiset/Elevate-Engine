#include "eepch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "OpenGL/OpenGLVertexArray.h"

namespace Elevate
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::GraphicAPI::None: return nullptr; break;
		case RendererAPI::GraphicAPI::OpenGL: return new OpenGLVertexArray();
		}

		EE_CORE_ASSERT(false, "A supported RendererAPI needs to be supported!");
	}
}
