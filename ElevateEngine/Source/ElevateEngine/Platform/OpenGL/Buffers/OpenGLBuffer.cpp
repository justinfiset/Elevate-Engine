
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iterator>
#include <new>

#include "OpenGLBuffer.h"

#include <ElevateEngine/Platform/OpenGL/GraphicsAPI.h>
#include <ElevateEngine/Renderer/RendererBackend.h>
#include <GLFW/glfw3.h>
#include <ElevateEngine/Platform/OpenGL/GLDebug.h>

import Elevate.Foundations.Assert;
import Elevate.Foundations.CoreLogger;
import Elevate.Renderer.Buffers;
import Elevate.Renderer.Renderer;
import Elevate.Renderer.RendererAPI;
import Elevate.Renderer.Vertex;
import Elevate.Renderer.GraphicsContext;

namespace Elevate
{
	inline void CreateBuffers(int32_t n, uint32_t* ids)
	{
#ifdef EE_SUPPORTS_DSA
		glCreateBuffers(n, ids);
#else
		glGenBuffers(n, ids);
#endif
	}

	inline void NamedBufferData(uint32_t buffer, uint32_t target, size_t size, const void* data, uint32_t usage)
	{
#ifdef EE_SUPPORTS_DSA
		glNamedBufferData(buffer, size, data, usage);
#else
		glBindBuffer(target, buffer);
		glBufferData(target, size, data, usage);
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////
	// VertexBuffer ///////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	OpenGLVertexBuffer::OpenGLVertexBuffer(const void* vertices, uint32_t size)
		: VertexBuffer(vertices, size)
	{
		GLCheck(CreateBuffers(1, &m_rendererID));
		SetData(vertices, size);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		// Check if the OpenGl context is still active before performing actions
		if (GraphicsContext::CanUseContext())
		{
			GLCheck(glDeleteBuffers(1, &m_rendererID));
		}
	}

	void OpenGLVertexBuffer::Bind() const
	{
		GLCheck(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		GLCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void OpenGLVertexBuffer::SetData(const void* newData, uint32_t size)
	{
		if (m_rendererID != 0)
		{
			data = newData;
			SetSize(size);

			if (newData)
			{
				GLCheck(NamedBufferData(m_rendererID, GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
			}
		}
		else
		{
			CoreLogger::Error("ERROR : (OpenGLVertexBuffer::SetData) Buffer is not initialized yet.");
		}
	}

	void OpenGLVertexBuffer::Resize(uint32_t newSize)
	{
		if (m_rendererID != 0 && data)
		{
			GLCheck(NamedBufferData(m_rendererID, GL_ARRAY_BUFFER, newSize, data, GL_DYNAMIC_DRAW));
			SetSize(newSize);
		}
		else
		{
			CoreLogger::Error("ERROR : (OpenGLVertexBuffer::Resize) You must initialize the VertexBuffer before resizing it.");
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////
	// IndexBuffer ////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	OpenGLIndexBuffer::OpenGLIndexBuffer(const void* indices, uint32_t count)
		: m_count(count)
	{
		GLCheck(CreateBuffers(1, &m_rendererID));
#ifdef EE_SUPPORTS_DSA
		GLCheck(glNamedBufferData(m_rendererID, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
#else
		// ELEMENT_ARRAY_BUFFER bindings are VAO state. Creating an index buffer must
		// not replace the element binding of whichever VAO happens to be bound,
		// otherwise publishing a mesh can corrupt the fullscreen quad.
		GLCheck(glBindVertexArray(0));
		GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
		GLCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
		GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
#endif
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		// Check if the OpenGl context is still active before performing actions
		if (GraphicsContext::CanUseContext())
		{
			GLCheck(glDeleteBuffers(1, &m_rendererID));
		}
	}

	void OpenGLIndexBuffer::Bind() const
	{
		GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}

namespace Elevate::RendererBackend
{
	void RegisterBufferFactories()
	{
		VertexBuffer::SetFactory([](const std::byte* vertices, uint32_t size) -> VertexBuffer* {
			switch (Renderer::GetAPI())
			{
			case RendererAPI::GraphicAPI::None: return nullptr;
			case RendererAPI::GraphicAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
			}

			Assert::That(false, "A supported RendererAPI needs to be supported!");
			return nullptr;
		});

		IndexBuffer::SetFactory([](const std::byte* indices, uint32_t count) -> IndexBuffer* {
			switch (Renderer::GetAPI())
			{
			case RendererAPI::GraphicAPI::None: return nullptr;
			case RendererAPI::GraphicAPI::OpenGL: return new OpenGLIndexBuffer(indices, count);
			}

			Assert::That(false, "A supported RendererAPI needs to be supported!");
			return nullptr;
		});
	}
}