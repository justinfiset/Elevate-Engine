#include "eepch.h"
#include "OpenGLBuffer.h"
#include "ElevateEngine/Renderer/Vertex.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ElevateEngine/Core/Log.h>

#include <ElevateEngine/Renderer/GLDebug.h>

namespace Elevate
{
    ///////////////////////////////////////////////////////////////////////////////////////
    // VertexBuffer ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////
    OpenGLVertexBuffer::OpenGLVertexBuffer(void* vertices, uint32_t size)
        : VertexBuffer(vertices, size)
    {
        GLCheck(glCreateBuffers(1, &m_RendererID));
        GLCheck(glNamedBufferData(m_RendererID, size, vertices, GL_DYNAMIC_DRAW));
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        GLCheck(glDeleteBuffers(1, &m_RendererID));
    }

    void OpenGLVertexBuffer::Bind() const
    {
        GLCheck(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        GLCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    void OpenGLVertexBuffer::Resize(uint32_t newSize)
    {
        if (m_RendererID != 0)
        {
            GLCheck(glNamedBufferData(m_RendererID, newSize, nullptr, GL_DYNAMIC_DRAW));
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
        : m_Count(count)
    {
        GLCheck(glCreateBuffers(1, &m_RendererID));
        GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
        GLCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        GLCheck(glDeleteBuffers(1, &m_RendererID));
    }

    void OpenGLIndexBuffer::Bind() const
    {
        GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
}