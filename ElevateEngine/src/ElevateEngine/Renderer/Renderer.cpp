#include "eepch.h"
#include "Renderer.h"

// todo remove or fix these includes
#include "ElevateEngine/Renderer/OpenGL/OpenGLRendererAPI.h"
#include <ElevateEngine/Scene/Scene.h>

#include <ElevateEngine/Renderer/Camera.h>
#include <ElevateEngine/Renderer/Texture/Texture.h>
#include <ElevateEngine/Renderer/Material.h>

namespace Elevate
{
	Renderer::RendererStorage Renderer::s_data = RendererStorage();
	RenderState Renderer::s_currentState = RenderState();
	RendererAPI* Renderer::s_API = new OpenGLRendererAPI();
	RenderCommandQueue Renderer::s_commands = RenderCommandQueue();
	uint32_t Renderer::s_currentShaderID = 0;
	uintptr_t Renderer::s_textures[16];

	void Renderer::BeginFrame(const ScenePtr scene, const Camera& cam)
	{
		s_currentShaderID = 0;
		s_data.CameraPosition = cam.gameObject->GetPosition();
		s_data.ViewProj = cam.GenViewProjectionMatrix();
		s_data.ActiveLighting = scene->GetSceneLighting();
	}

	bool Renderer::BindShader(const std::shared_ptr<Shader>& shader)
	{
		uint32_t id = shader->GetID();
		if (s_currentShaderID != id)
		{
			shader->Bind();
			s_currentShaderID = id;
			return true;
		}
		return false;
	}

	void Renderer::ApplySystemUniforms(const std::shared_ptr<Shader>& shader)
	{
		// If the binded shader changed
		if (BindShader(shader))
		{
			shader->SetProjectionViewMatrix(s_data.ViewProj);
			shader->SetCameraPosition(s_data.CameraPosition);
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
		uint32_t prevshader = s_currentShaderID;
		if (command.MaterialInstance)
		{
			auto shader = command.MaterialInstance->GetShader();
			if (shader)
			{
				ApplySystemUniforms(shader);
				shader->SetModelMatrix(command.Transform);
				s_data.ActiveLighting->UploadToShader(shader);
				command.MaterialInstance->Apply();
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

		if (bucketType == RenderBucket::Transparent)
		{
			command.State.BlendEnable = true;
			command.State.DepthWrite = false; // Transparents usually don't write to depth
		}
		else
		{
			command.State.BlendEnable = false;
			command.State.DepthWrite = true;
		}

		Submit(bucketType, command);
	}

	void Renderer::BindTexture(const std::shared_ptr<Texture>& texture, uint8_t slot)
	{
		// todo optimize and make sure EVERY texture uses this
		//uintptr_t textureID = texture ? reinterpret_cast<uintptr_t>(texture->GetNativeHandle()) : 0;
		//if (s_textures[slot] != textureID)
		//{
		//	if (texture)
		//	{
		//		texture->Bind(slot);
		//	}
		//	s_textures[slot] = textureID;
		//}
		texture->Bind(slot);
	}
}