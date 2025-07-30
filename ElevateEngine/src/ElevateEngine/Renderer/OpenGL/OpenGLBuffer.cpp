#include "eepch.h"
#include "OpenGLBuffer.h"
#include "ElevateEngine/Renderer/Vertex.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ElevateEngine/Core/Log.h>

// Macro GLCheck pour OpenGL Debug // TODO REMOVE AND INCLUDE FILE
#define GLCheck(x) \
    x; \
    { GLenum err = glGetError(); if (err != GL_NO_ERROR) EE_CORE_ERROR("OpenGL Error {} at {}:{}", err, __FILE__, __LINE__); }

namespace Elevate
{
    ///////////////////////////////////////////////////////////////////////////////////////
    // VertexBuffer ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////
    OpenGLVertexBuffer::OpenGLVertexBuffer(void* vertices, uint32_t size)
    {
        GLenum errBefore = glGetError();
        if (errBefore != GL_NO_ERROR) {
            EE_CORE_ERROR("Error BEFORE glCreateBuffers: {}", errBefore);
        }

        GLCheck(glCreateBuffers(1, &m_RendererID));

        GLenum errAfter = glGetError();
        EE_CORE_TRACE("Error AFTER glCreateBuffers: {}", errAfter);

        GLCheck(glNamedBufferData(m_RendererID, size, vertices, GL_STATIC_DRAW));
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        GLCheck(glDeleteBuffers(1, &m_RendererID));
    }

    void OpenGLVertexBuffer::Bind() const
    {
        EE_CORE_INFO("BINDED VERTEX BUFFER w/ id: {}", m_RendererID);
        GLCheck(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        GLCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
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