#pragma once

#include <memory>
#include <glm/mat4x4.hpp>

#include <ElevateEngine/Renderer/RenderState.h>
#include <ElevateEngine/Renderer/VertexArray.h>

namespace Elevate
{
	class Shader;
	class Material;

	struct RenderCommand
	{
		uint64_t SortingKey = 0;
		// GEOMETRY
		Material* MaterialInstance;
		VertexArray* VertexArray;;
		glm::mat4 Transform = glm::mat4(1.0f);
		// GPU STATE
		RenderState State;

		RenderCommand() = default;
		RenderCommand(const std::shared_ptr<Material>& material, const std::shared_ptr<Elevate::VertexArray>& vertexArray, const glm::mat4& transform, const RenderState& state)
			: MaterialInstance(material.get()), VertexArray(vertexArray.get()), Transform(transform), State(state), SortingKey(GetSortKey()) { }

	public:
		uint64_t GetSortKey() const;
	};
}