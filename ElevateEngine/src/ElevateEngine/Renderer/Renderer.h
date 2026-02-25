#pragma once
#include "RendererAPI.h"
#include <ElevateEngine/Renderer/RenderState.h>
#include <ElevateEngine/Renderer/Commands/RenderBucket.h>

namespace Elevate
{
	class Camera;
	class Shader;

	class Scene;
	using ScenePtr = std::shared_ptr<Scene>;
	class SceneLighting;

	class LayerStack;
	class Layer;

	class Shader;
	class Texture;

	class Renderer
	{
	public:
		inline static RendererAPI::GraphicAPI GetAPI() { return RendererAPI::GetAPI(); }

		// RENDER API STATIC WRAPPER
		static void BeginFrame(const ScenePtr scene, const Camera& cam);
		// Performant function to bind a shader and to minimize API calls
		static bool BindShader(const std::shared_ptr<Shader>& shader); // Return true if the shader just changed
		static void ApplySystemUniforms(const std::shared_ptr<Shader>& shader);
		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void FlushBuffers();
		static void SetViewport(int x, int y, int width, int height);
		static void DrawArray(const VertexArray* vao, DrawPrimitiveType primitive = DrawPrimitiveType::Triangles);
		static void DrawArray(const std::shared_ptr<VertexArray>& vao, DrawPrimitiveType primitive = DrawPrimitiveType::Triangles);
		static void DrawStack();
		static void PushRenderState(const RenderState& newState);

		/// <summary>
		/// Immediatly process a RenderCommand. Do not use directly unless you know what you are donig.
		/// </summary>
		/// <param name="command">The RenderCommand to process at this exact instant.</param>
		static void Dispatch(const RenderCommand& command);
		// todo doc here
		static void Submit(RenderBucket::Type type, const RenderCommand& command);
		static void SubmitMesh(const std::shared_ptr<VertexArray>& vao, const std::shared_ptr<Material>& material, const glm::mat4& transform, RenderBucket::Type bucketType = RenderBucket::Opaque);

		// Textures
		static void BindTexture(const std::shared_ptr<Texture>& texture, uint8_t slot = 0);

	private:
		struct RendererStorage {
			glm::mat4 ViewProj;
			glm::vec3 CameraPosition;
			const SceneLighting* ActiveLighting = nullptr;
		};

		static RendererAPI* s_API;
		static RenderCommandQueue s_commands;

		// Current States
		static RendererStorage s_data;
		static RenderState s_currentState;
		static uint32_t s_currentShaderID;
		static uintptr_t s_textures[];
	};
}