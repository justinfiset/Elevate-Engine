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
		case RendererAPI::None: return nullptr; break; // TODO implement
		case RendererAPI::OpenGL: return new OpenGLVertexArray();
		}

		EE_CORE_ASSERT(false, "A supported RendererAPI needs to be supported!")
	}
}
