#pragma once

#include "ElevateEngine/Renderer/RendererAPI.h"

namespace Elevate
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		inline GLenum DrawPrimitiveTypeToOpenGL(DrawPrimitiveType type) const;

		virtual void SetClearColor(const glm::vec4& color) const override;
		virtual void SetViewport(int x, int y, int width, int height) const override;
		virtual void Clear() const override;
		virtual void FlushBuffers() const override;

		virtual void DrawArray(const std::shared_ptr<VertexArray>& vao, DrawPrimitiveType primitive = DrawPrimitiveType::Triangles) const override;
		virtual void DrawStack() const override;

		virtual void SetCullingState(bool enabled) const override;
		virtual void SetDepthWrittingState(bool enabled) const override;
		virtual void SetDepthTestingState(bool enabled) const override;
	};
}