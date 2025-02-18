#pragma once
#include "RendererAPI.h"
#include "Mesh.h"
#include "Model.h"
#include <ElevateEngine/Renderer/Shader/Shader.h>

namespace Elevate
{
	class Renderer
	{
	public:
		//static void BeginSceneFrame(const std::shared_ptr<Shader> shader);
		//static void EndSceneFrame(const std::shared_ptr<Shader> shader);

		static void SubmitModel(const Model& model);
		static void RemoveModel(const Model& model);
		static void SubmitMesh(const std::shared_ptr<Shader>& shader, const Mesh& mesh);

		static void SubmitVertexArray(const std::shared_ptr<VertexArray>& vao);
		static void SubmitTrianglesArray(const std::shared_ptr<VertexArray>& vao);

		inline static RendererAPI::GraphicAPI GetAPI() { return RendererAPI::GetAPI(); }

		// RENDER API STATIC WRAPPER
		static void SetClearColor(const glm::vec4 color);
		static void Clear();
		static void FlushBuffers();
		static void SetViewport(int x, int y, int width, int height);
		static void DrawArray(const std::shared_ptr<VertexArray>& vao);
		static void DrawStack();
	private:
		static RendererAPI* s_API;
	};
}