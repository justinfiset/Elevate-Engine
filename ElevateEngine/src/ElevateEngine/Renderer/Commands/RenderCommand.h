#pragma once

#include <memory>
#include <glm/mat4x4.hpp>

#include <ElevateEngine/Renderer/RenderState.h>
#include <ElevateEngine/Renderer/VertexArray.h>

namespace Elevate
{
	struct RenderCommand
	{
		uint64_t SortingKey = 0;
		// GEOMETRY
		std::shared_ptr<VertexArray> VertexArray;;
		glm::mat4 Transform = glm::mat4(1.0f);
		// GPU STATE
		RenderState State;

		RenderCommand(const std::shared_ptr<Elevate::VertexArray>& vertexArray, const glm::mat4& transform, const RenderState& state)
			: VertexArray(vertexArray), Transform(transform), State(state), SortingKey(GetSortKey()) { }

	public:
		uint64_t GetSortKey() const;
	};
}