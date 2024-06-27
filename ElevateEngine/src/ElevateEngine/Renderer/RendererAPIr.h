#pragma once

#include "glm/glm.hpp"
#include "VertexArray.h"
#include "Shader.h"
#include "Mesh.h"

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
		
		virtual void DrawArray(const std::shared_ptr<VertexArray>& vao) const = 0;
		virtual void DrawStack(const std::shared_ptr<Shader> shader) const = 0;

		inline void SubmitMesh(const Mesh mesh) { m_MeshStack.push_back(mesh); }

		inline static GraphicAPI GetAPI() { return s_ActiveAPI; }
		
	protected:
		std::vector<Mesh> m_MeshStack;
	private:
		static GraphicAPI s_ActiveAPI;
	};
}