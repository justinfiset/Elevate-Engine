#pragma once

#include "glm/glm.hpp"
#include "VertexArray.h"

namespace Elevate
{
	class RendererAPI
	{
	public:
		enum class GraphicAPI
		{
			None = 0,
			OpenGL = 1,
		};

	public:
		virtual void SetClearColor(const glm::vec4 color) const = 0;
		virtual void Clear() const = 0;
		virtual void DrawStack(const std::shared_ptr<VertexArray>& vao) const = 0;

		inline static GraphicAPI GetAPI() { return s_ActiveAPI; }
	private:
		static GraphicAPI s_ActiveAPI;
	};
}