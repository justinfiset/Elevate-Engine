#pragma once
#include "RendererAPI.h"
#include <ElevateEngine/Renderer/RenderState.h>
#include <ElevateEngine/Renderer/Commands/RenderBucket.h>

namespace Elevate
{
	class Shader;
	class Scene;
	class Shader;
	class LayerStack;
	class Layer;

	class Renderer
	{
	public:
		//static void BeginSceneFrame(const std::shared_ptr<Shader> shader);
		//static void EndSceneFrame(const std::shared_ptr<Shader> shader);
		//static void SubmitShaderForSetup(std::shared_ptr<Shader> shader);
		//static void SetupShaders(Scene* scene);

		//static void SubmitModel(const Model& model);
		//static void RemoveModel(const Model& model);
		//static void SubmitMesh(const std::shared_ptr<Shader>& shader, const Mesh& mesh);

		// TODO REMOVE AS ONLY CALLING THE DRAWARRAY
		//static void SubmitVertexArray(const std::shared_ptr<VertexArray>& vao);

		inline static RendererAPI::GraphicAPI GetAPI() { return RendererAPI::GetAPI(); }

		// RENDER API STATIC WRAPPER
		// Performant function to bind a shader and to minimize API calls
		static void BindShader(const std::shared_ptr<Shader>& shader);
		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void FlushBuffers();
		static void SetViewport(int x, int y, int width, int height);
		static void DrawArray(const std::shared_ptr<VertexArray>& vao, DrawPrimitiveType primitive = DrawPrimitiveType::Triangles);
		static void DrawStack();
		static void PushRenderState(const RenderState& newState);
		static void SubmitDrawCall(RenderBucket::Type bucketType, std::shared_ptr<VertexArray>& array,/* std::shared_ptr<Material>& material,*/ const glm::mat4& transform, const RenderState& state);
		static void Dispatch(RenderCommand& command);
	
	private:
		//static std::unordered_set<Shader*> s_pendingShaders;
		static RendererAPI* s_API;

		// Current States
		static RenderState s_currentState;
		static uint32_t s_currentShaderID;
	};
}