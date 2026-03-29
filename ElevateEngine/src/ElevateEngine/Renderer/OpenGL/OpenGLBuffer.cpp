#include "eepch.h"
#include "OpenGLBuffer.h"
#include "ElevateEngine/Renderer/Vertex.h"

#include <ElevateEngine/Renderer/GraphicsAPI.h>
#include <GLFW/glfw3.h>
#include <ElevateEngine/Core/Log.h>

#include <ElevateEngine/Renderer/GLDebug.h>
#include <ElevateEngine/Renderer/GraphicsContext.h>

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

	inline void NamedBufferData(uint32_t buffer, uint32_t target, size_t size, const void* data, uint32_t usage) {
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
			EE_CORE_ERROR("ERROR : (OpenGLVertexBuffer::SetData) Buffer is not initialized yet.");
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
			EE_CORE_ERROR("ERROR : (OpenGLVertexBuffer::Resize) You must initialize the VertexBuffer before resizing it.");
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////
	// IndexBuffer ////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	OpenGLIndexBuffer::OpenGLIndexBuffer(const void* indices, uint32_t count)
		: m_count(count)
	{
		GLCheck(CreateBuffers(1, &m_rendererID));
		GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
		GLCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
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