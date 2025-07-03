#pragma once

#include "ElevateEngine/Renderer/RendererAPI.h"

namespace Elevate
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) const override;
		virtual void SetViewport(int x, int y, int width, int height) const override;
		virtual void Clear() const override;
		virtual void FlushBuffers() const override;

		virtual void DrawArray(const std::shared_ptr<VertexArray>& vao) const override;
		virtual void DrawStack() const override;
		virtual void DrawTriangles(const std::shared_ptr<VertexArray>& vao) const override;
	};
}