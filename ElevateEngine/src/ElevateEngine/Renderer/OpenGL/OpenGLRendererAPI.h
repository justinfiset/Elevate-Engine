#pragma once

#include "ElevateEngine/Renderer/RendererAPI.h"

// TODO REMOVE C'EST POUR DEBUG ONLY
#define GLCheck(x) \
    x; \
    { GLenum err = glGetError(); if (err != GL_NO_ERROR) EE_CORE_ERROR("OpenGL Error {} at {}:{}", err, __FILE__, __LINE__); }

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

		virtual void SetCullingState(bool enabled) const override;
		virtual void SetDepthWrittingState(bool enabled) const override;
		virtual void SetDepthTestingState(bool enabled) const override;
	};
}