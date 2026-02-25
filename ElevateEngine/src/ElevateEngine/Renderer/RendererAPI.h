#pragma once

#include "glm/glm.hpp"
#include "VertexArray.h"
#include <ElevateEngine/Renderer/Shader/Shader.h>

#include "Mesh.h"
#include "Model.h"

namespace Elevate
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
		virtual void SetClearColor(const glm::vec4& color) const = 0;
		virtual void SetViewport(int x, int y, int width, int height) const = 0;
		virtual void Clear() const = 0;
		virtual void FlushBuffers() const = 0;

		virtual void DrawArray(const std::shared_ptr<VertexArray>& vao, DrawPrimitiveType primitive = DrawPrimitiveType::Triangles) const = 0;
		virtual void DrawStack() const = 0;

		void SubmitModel(const Model& model);
		void RemoveModel(const Model& model);
		// TODO MAKE A REMOVE FROM DRAW STACK
		void Submitmesh(const std::shared_ptr<Shader>& shader, const Mesh& mesh);
		//static void SubmitVertexArray(const std::shared_ptr<VertexArray>& vao);
		//static void SubmitTrianglesArray(const std::shared_ptr<VertexArray>& vao);

		virtual void SetCullingState(bool enabled) const = 0;
		virtual void SetDepthWrittingState(bool enabled) const = 0;
		virtual void SetDepthTestingState(bool enabled) const = 0;

		inline static GraphicAPI GetAPI() { return s_ActiveAPI; }
		
	protected:
		std::unordered_map<std::shared_ptr<Shader>, std::vector<Model>> m_ModelStack;
		std::unordered_map<std::shared_ptr<Shader>, std::vector<Mesh>> m_MeshStack;
	private:
		static GraphicAPI s_ActiveAPI;
	};
}