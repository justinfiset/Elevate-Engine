module;

#include <cstdint>
#include <glm/glm.hpp>

export module Elevate.Renderer.RendererAPI;

import Elevate.Renderer.VertexArray;
import Elevate.Renderer.Commands.RenderCommand;

export namespace Elevate
{
	enum class DrawPrimitiveType
	{
		Points,
		Lines,
		LineStrip,
		LineLoop,      // OpenGL only
		Triangles,
		TriangleStrip,
		TriangleFan,   // OpenGL only
		Patches
	};

	class RendererAPI
	{
	public:
		enum class GraphicAPI
		{
			None = 0,
			OpenGL = 1,
		};

	public:
		virtual ~RendererAPI() = default;

		virtual void SetClearColor(const glm::vec4& color) const = 0;
		virtual void SetViewport(int x, int y, int width, int height) const = 0;

		virtual void Clear() const = 0;
		virtual void ClearDepth() const = 0;
		virtual void FlushBuffers() const = 0;

		virtual void ClearTextureBindings() const = 0;
		virtual void UnbindTexture(uint8_t slot = 0) const = 0;

		virtual void DrawArray(const VertexArray* vao, DrawPrimitiveType primitive = DrawPrimitiveType::Triangles) const = 0;
		virtual void SetCullingState(CullFace cullSetting) const = 0;
		virtual void SetDepthWrittingState(bool enabled) const = 0;
		virtual void SetDepthTestingState(bool enabled) const = 0;
		virtual void SetBlendingState(BlendModeType mode) const = 0;
		virtual void SetDepthFunction(DepthFunction depthFunc) const = 0;

		static GraphicAPI GetAPI() { return s_ActiveAPI; }

	private:
		static GraphicAPI s_ActiveAPI;
	};
}
