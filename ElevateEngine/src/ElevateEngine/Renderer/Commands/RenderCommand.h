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
		uint64_t m_SortingKey = 0;
		// GEOMETRY
		Material* m_MaterialInstance;
		VertexArray* m_VertexArray;;
		glm::mat4 Transform = glm::mat4(1.0f);
		// GPU STATE
		RenderState m_State;

		RenderCommand() = default;
		RenderCommand(const std::shared_ptr<Material>& material, const std::shared_ptr<Elevate::VertexArray>& vertexArray, const glm::mat4& transform, const RenderState& state)
			: m_MaterialInstance(material.get()), m_VertexArray(vertexArray.get()), Transform(transform), m_State(state), m_SortingKey(GetSortKey()) { }

	public:
		uint64_t GetSortKey() const;
	};
}