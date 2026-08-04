#pragma once
#include "RendererAPI.h"

#include <memory>

#include <ElevateEngine/Renderer/RenderState.h>
#include <ElevateEngine/Renderer/Commands/RenderBucket.h>

namespace Elevate
{
	class Camera;
	class Shader;
	class Texture;
	class Cubemap;
	class Scene;
	using ScenePtr = std::shared_ptr<Scene>;
	class SceneLighting;

	class Framebuffer;

	class Renderer
	{
	public:
		inline static RendererAPI::GraphicAPI GetAPI() { return RendererAPI::GetAPI(); }

		static void Init(uint32_t width, uint32_t height);
	private:
		static void InitShadowRenderer();

	public:

		// RENDER API STATIC WRAPPER
		static void BeginFrame(const ScenePtr scene, const Camera& cam);
		static void RenderFrame();
		static void Present(uint32_t width, uint32_t height);

		// Performant function to bind a shader and to minimize API calls
		static bool BindShader(const std::shared_ptr<Shader>& shader); // Return true if the shader just changed
		static void ApplySystemUniforms(const std::shared_ptr<Shader>& shader);
		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void ClearDepth();
		static void FlushBuffers();
		static void SetViewport(int x, int y, int width, int height);
		static void DrawArray(const VertexArray* vao, DrawPrimitiveType primitive = DrawPrimitiveType::Triangles);
		static void DrawArray(const std::shared_ptr<VertexArray>& vao, DrawPrimitiveType primitive = DrawPrimitiveType::Triangles);
		static void PushRenderState(const RenderState& newState);

		static Framebuffer& GetMainFramebuffer();

		/// <summary>
		/// Immediatly process a RenderCommand. Do not use directly unless you know what you are donig.
		/// </summary>
		/// <param name="command">The RenderCommand to process at this exact instant.</param>
		static void Dispatch(const RenderCommand& command);
		// todo doc here
		static void Submit(RenderBucket::Type type, const RenderCommand& command);
		static void SubmitMesh(const std::shared_ptr<VertexArray>& vao, const std::shared_ptr<Material>& material, const glm::mat4& transform);

		// Textures
		static void BindTexture(const std::shared_ptr<Texture>& texture, uint8_t slot = 0);

		static void InvalidateStateCache();

	private:
		static void RenderShaowMaps();
		static void RenderSkybox();
		static void RenderGeometry();

		static void DrawStack();
		static void ClearStack();

	private:
		struct RendererStorage {
			glm::mat4 View;
			glm::mat4 Projection;
			glm::mat4 ViewProj;

			glm::vec3 CameraPosition;
			const SceneLighting* ActiveLighting = nullptr;
			const Cubemap* ActiveCubemap = nullptr;
		};

		static RendererAPI* s_API;
		static RenderCommandQueue s_commands;

		// Current States
		static RendererStorage s_data;
		static RenderState s_currentState;
		static uint32_t s_currentShaderID;
		static uintptr_t s_textures[];

		// Framebuffer
		static std::unique_ptr<Framebuffer> s_mainFramebuffer;

		// Shadow Mapping
		static std::shared_ptr<Shader> s_shadowShader;
		static std::unique_ptr<Elevate::Framebuffer> s_directionalShadowMap;
	};
}