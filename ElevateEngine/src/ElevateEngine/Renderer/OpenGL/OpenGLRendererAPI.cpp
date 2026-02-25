#include "eepch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ElevateEngine/Renderer/GLDebug.h>
#include <ElevateEngine/Renderer/Renderer.h>

namespace Elevate
{
	inline GLenum OpenGLRendererAPI::DrawPrimitiveTypeToOpenGL(DrawPrimitiveType type) const
	{
		switch (type)
		{
			case DrawPrimitiveType::Points:        return GL_POINTS;
			case DrawPrimitiveType::Lines:         return GL_LINES;
			case DrawPrimitiveType::LineStrip:     return GL_LINE_STRIP;
			case DrawPrimitiveType::Triangles:     return GL_TRIANGLES;
			case DrawPrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;
			case DrawPrimitiveType::TriangleFan:   return GL_TRIANGLE_FAN;
			case DrawPrimitiveType::Patches:       return GL_PATCHES;
		}
		return GL_TRIANGLES;
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) const
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::SetViewport(int x, int y, int width, int height) const
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::FlushBuffers() const
	{
		glFlush();
	}

	void OpenGLRendererAPI::DrawArray(const VertexArray* vao, DrawPrimitiveType primitive) const
	{
		if (!glfwGetCurrentContext()) {
			EE_CORE_ERROR("No OpenGL context active!");
			return;
		}

		if (!vao)
		{
			EE_CORE_ERROR("VAO or IndexBuffer is null!");
			return;
		}

		vao->Bind();

		if (vao->GetIndexBuffer())
		{
			if (vao->GetIndexBuffer()->GetCount() == 0) 
			{
				EE_CORE_ERROR("IndexBuffer has 0 indices!");
				return;
			}

			GLCheck(glDrawElements(DrawPrimitiveTypeToOpenGL(primitive), vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));
		}
		else
		{
			for (const auto& vbo : vao->GetVertexBuffers())
			{
				uint32_t vertexCount = vbo->GetSize() / vbo->GetLayout().GetStride();
				GLCheck(glDrawArrays(DrawPrimitiveTypeToOpenGL(primitive), 0, vertexCount));
			}
		}
	}

	void OpenGLRendererAPI::SetCullingState(bool enabled) const
	{
		if(enabled) glEnable(GL_CULL_FACE);
		else        glDisable(GL_CULL_FACE);
	}
	void OpenGLRendererAPI::SetDepthWrittingState(bool enabled) const
	{
		glDepthMask(enabled ? GL_TRUE : GL_FALSE);
	}
	void OpenGLRendererAPI::SetDepthTestingState(bool enabled) const
	{
		if (enabled) glEnable(GL_DEPTH_TEST);
		else        glDisable(GL_DEPTH_TEST);
	}
}
