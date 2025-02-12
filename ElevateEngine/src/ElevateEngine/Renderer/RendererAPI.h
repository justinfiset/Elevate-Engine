#pragma once

#include "glm/glm.hpp"
#include "VertexArray.h"
#include <ElevateEngine/Renderer/Shader/Shader.h>

#include "Mesh.h"
#include "Model.h"

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
		virtual void SetViewPort(int x, int y, int width, int height) const = 0;
		virtual void Clear() const = 0;
		virtual void FlushBuffers() const = 0;

		virtual void DrawArray(const std::shared_ptr<VertexArray>& vao) const = 0;
		virtual void DrawTriangles(const std::shared_ptr<VertexArray>& vao) const = 0;
		virtual void DrawStack() const = 0;

		void SubmitModel(const Model& model);
		void RemoveModel(const Model& model);
		// TODO MAKE A REMOVE FROM DRAW STACK
		void Submitmesh(const std::shared_ptr<Shader>& shader, const Mesh& mesh);
		//static void SubmitVertexArray(const std::shared_ptr<VertexArray>& vao);
		//static void SubmitTrianglesArray(const std::shared_ptr<VertexArray>& vao);

		inline static GraphicAPI GetAPI() { return s_ActiveAPI; }
		
	protected:
		std::unordered_map<std::shared_ptr<Shader>, std::vector<Model>> m_ModelStack;
		std::unordered_map<std::shared_ptr<Shader>, std::vector<Mesh>> m_MeshStack;
	private:
		static GraphicAPI s_ActiveAPI;
	};
}