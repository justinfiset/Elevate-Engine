#pragma once
#include "RendererAPI.h"
#include "Mesh.h"
#include "Model.h"
#include "Shader.h"

namespace Elevate
{
	class Renderer
	{
	public:
		static void BeginSceneFrame(const std::shared_ptr<Shader> shader);
		static void EndSceneFrame(const std::shared_ptr<Shader> shader);

		// TODO can we make it const?
		static void SubmitModel(const Model model);
		static void SubmitMesh(const Mesh mesh);
		static void SubmitVertexArray(const std::shared_ptr<VertexArray>& vao);
		static void SubmitTrianglesArray(const std::shared_ptr<VertexArray>& vao);

		inline static RendererAPI::GraphicAPI GetAPI() { return RendererAPI::GetAPI(); }
		// RENDER API STATIC WRAPPER
		static void SetClearColor(const glm::vec4 color);
		static void Clear();
		static void DrawArray(const std::shared_ptr<VertexArray>& vao);
		static void DrawStack(std::shared_ptr<Shader> shader);
	private:
		static RendererAPI* s_API;
	};
}