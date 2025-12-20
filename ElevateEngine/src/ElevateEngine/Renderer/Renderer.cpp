#include "eepch.h"
#include "Renderer.h"

// todo remove or fix these includes
#include "ElevateEngine/Renderer/OpenGL/OpenGLRendererAPI.h"
#include <ElevateEngine/Scene/Scene.h>

#include <ElevateEngine/Renderer/Texture/Texture.h>

namespace Elevate
{
	RenderState Renderer::s_currentState = RenderState();
	RendererAPI* Renderer::s_API = new OpenGLRendererAPI();
	RenderCommandQueue Renderer::s_commands = RenderCommandQueue();
	uint32_t Renderer::s_currentShaderID = 0;

	// todo remove
	//void Renderer::SubmitModel(const Model& model)
	//{
	//	s_API->SubmitModel(model);
	//}

	//void Renderer::RemoveModel(const Model& model)
	//{
	//	s_API->RemoveModel(model);
	//}

	//void Renderer::SubmitMesh(const std::shared_ptr<Shader>& shader, const Mesh& mesh)
	//{
	//	s_API->Submitmesh(shader, mesh);
	//}

	//void Renderer::SubmitVertexArray(const std::shared_ptr<VertexArray>& vao)
	//{
	//	Renderer::SubmitVertexArray(vao);
	//}

	void Renderer::BindShader(const std::shared_ptr<Shader>& shader)
	{
		if (s_currentShaderID != shader->GetID())
		{
			shader->Bind();
		}
	}

	// RENDER API STATIC WRAPPER
	void Renderer::SetClearColor(const glm::vec4& color)
	{
		s_API->SetClearColor(color);
	}

	void Renderer::Clear()
	{
		s_API->Clear();
	}

	void Renderer::FlushBuffers()
	{
		s_API->FlushBuffers();
	}

	void Renderer::SetViewport(int x, int y, int width, int height)
	{
		s_API->SetViewport(x, y, width, height);
	}

	void Renderer::DrawArray(const VertexArray* vao, DrawPrimitiveType primitive)
	{
		if (vao)
		{
			s_API->DrawArray(vao, primitive);
		}
	}

	void Renderer::DrawArray(const std::shared_ptr<VertexArray>& vao, DrawPrimitiveType primitive)
	{
		DrawArray(vao.get(), primitive);
	}

	void Renderer::DrawStack()
	{
		s_commands.Sort();
		s_commands.FlushAll();
	}

	void Renderer::PushRenderState(const RenderState& newState)
	{
		// todo make a first invalid call to make sure the GPU is synced with this cache before the user does anything
		if (newState.Cullface != s_currentState.Cullface)
		{
			s_API->SetCullingState(newState.Cullface);
		}

		if (newState.DepthWrite != s_currentState.DepthWrite)
		{
			s_API->SetDepthWrittingState(newState.DepthWrite);
		}

		if (newState.DepthTest != s_currentState.DepthTest)
		{
			s_API->SetDepthTestingState(newState.DepthTest);
		}

		s_currentState = newState;
	}

	void Renderer::Dispatch(const RenderCommand& command)
	{
		// Update the renderer state if necessary
		PushRenderState(command.State);
		// Setup the Material and Shader
		if (command.MaterialInstance)
		{
			command.MaterialInstance->Apply();

			auto shader = command.MaterialInstance->GetShader();
			if (shader)
			{
				shader->SetModelMatrix(command.Transform);
			}
		}

		// Actually render the vertex array
		Renderer::DrawArray(command.VertexArray);
	}

	void Renderer::Submit(RenderBucket::Type type, const RenderCommand& command)
	{
		s_commands.Submit(type, command);
	}

	void Renderer::SubmitMesh(const std::shared_ptr<VertexArray>& vao, const std::shared_ptr<Material>& material, const glm::mat4& transform, RenderBucket::Type bucketType)
	{
		RenderCommand command;
		command.VertexArray = vao.get();
		command.MaterialInstance = material.get();
		command.Transform = transform;

		if (bucketType == RenderBucket::Transparent) {
			command.State.BlendEnable = true;
			command.State.DepthWrite = false; // Transparents usually don't write to depth
		}
		else {
			command.State.BlendEnable = false;
			command.State.DepthWrite = true;
		}

		Submit(bucketType, command);
	}

	void Renderer::BindTexture(const std::shared_ptr<Texture>& texture, uint8_t slot)
	{
		uint32_t textureID = texture ? (uint32_t) texture->GetNativeHandle() : 0;
		if (s_textures[slot] != textureID)
		{
			if (texture)
			{
				texture->Bind(slot);
			}
			else
			{

			}
			s_textures[slot] = textureID;
		}
	}
}