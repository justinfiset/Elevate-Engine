#pragma once
#include "RendererAPIr.h"

namespace Elevate
{
	class Renderer
	{
	public:
		static void BeginSceneFrame();
		static void EndSceneFrame();

		static void SubmitMeshes(const std::shared_ptr<VertexArray>& vao);

		inline static RendererAPI::GraphicAPI GetAPI() { return RendererAPI::GetAPI(); }
		// RENDER API STATIC WRAPPER
		static void SetClearColor(const glm::vec4 color);
		static void Clear();
		static void DrawStack(const std::shared_ptr<VertexArray>& vao);

	private:
		static RendererAPI* s_API;
	};
}