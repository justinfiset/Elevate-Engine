#pragma once

#include "RendererAPIr.h"

namespace Elevate
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4 color) const override;
		virtual void Clear() const override;

		virtual void DrawArray(const std::shared_ptr<VertexArray>& vao) const override;
		virtual void DrawStack(const std::shared_ptr<Shader> shader) const override;
	};
}