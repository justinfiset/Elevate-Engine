#pragma once
#include "RendererAPI.h"
#include "Mesh.h"
#include "Model.h"
#include <ElevateEngine/Renderer/Shader/Shader.h>
#include <ElevateEngine/Renderer/RenderState.h>

namespace Elevate
{
	class Renderer
	{
	public:
		//static void BeginSceneFrame(const std::shared_ptr<Shader> shader);
		//static void EndSceneFrame(const std::shared_ptr<Shader> shader);
		static void SubmitShaderForSetup(std::shared_ptr<Shader> shader);
		static void SetupShaders(Scene* scene);

		static void SubmitModel(const Model& model);
		static void RemoveModel(const Model& model);
		static void SubmitMesh(const std::shared_ptr<Shader>& shader, const Mesh& mesh);

		// TODO REMOVE AS ONLY CALLING THE DRAWARRAY
		static void SubmitVertexArray(const std::shared_ptr<VertexArray>& vao);

		inline static RendererAPI::GraphicAPI GetAPI() { return RendererAPI::GetAPI(); }

		// RENDER API STATIC WRAPPER
		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void FlushBuffers();
		static void SetViewport(int x, int y, int width, int height);
		static void DrawArray(const std::shared_ptr<VertexArray>& vao, DrawPrimitiveType primitive = DrawPrimitiveType::Triangles);
		static void DrawStack();
		static void PushRenderState(const RenderState& newState);
	private:
		static std::unordered_set<Shader*> s_pendingShaders;
		static RendererAPI* s_API;
		static RenderState s_currentState;
	};
}