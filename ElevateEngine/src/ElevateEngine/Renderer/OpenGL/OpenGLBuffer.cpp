#include "eepch.h"
#include "OpenGLBuffer.h"
#include "ElevateEngine/Renderer/Vertex.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ElevateEngine/Core/Log.h>

#include <ElevateEngine/Renderer/GLDebug.h>
#include <ElevateEngine/Renderer/GraphicsContext.h>

namespace Elevate
{
    ///////////////////////////////////////////////////////////////////////////////////////
    // VertexBuffer ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////
    OpenGLVertexBuffer::OpenGLVertexBuffer(void* vertices, uint32_t size)
        : VertexBuffer(vertices, size)
    {
        GLCheck(glCreateBuffers(1, &m_rendererID));
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

    void OpenGLVertexBuffer::SetData(void* newData, uint32_t size)
    {
        if (m_rendererID != 0)
        {
            data = newData;
            SetSize(size);

            if (newData)
            {
                GLCheck(glNamedBufferData(m_rendererID, size, data, GL_DYNAMIC_DRAW));
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
            GLCheck(glNamedBufferData(m_rendererID, newSize, data, GL_DYNAMIC_DRAW));
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
    OpenGLIndexBuffer::OpenGLIndexBuffer(void* indices, uint32_t count)
        : m_count(count)
    {
        GLCheck(glCreateBuffers(1, &m_rendererID));
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