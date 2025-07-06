#include "eepch.h"
#include "Renderer.h"

#include "ElevateEngine/Renderer/OpenGL/OpenGLRendererAPI.h"
#include <ElevateEngine/Scene/Scene.h>

namespace Elevate
{
	RendererAPI* Renderer::s_API = new OpenGLRendererAPI();
	std::unordered_set<Shader*> Renderer::s_pendingShaders;

	void Renderer::SubmitShaderForSetup(std::shared_ptr<Shader> shader)
	{
		s_pendingShaders.emplace(shader.get());
	}

	void Renderer::SetupShaders(Scene* scene)
	{
		if (scene)
		{
			for (auto* shader : s_pendingShaders)
			{
				scene->GetSceneLighting()->UploadToShader(shader);
			}
			s_pendingShaders.clear();
		}
	}

	void Renderer::SubmitModel(const Model& model)
	{
		s_API->SubmitModel(model);
	}

	void Renderer::RemoveModel(const Model& model)
	{
		s_API->RemoveModel(model);
	}

	void Renderer::SubmitMesh(const std::shared_ptr<Shader>& shader, const Mesh& mesh)
	{
		s_API->Submitmesh(shader, mesh);
	}

	void Renderer::SubmitVertexArray(const std::shared_ptr<VertexArray>& vao)
	{
		Renderer::DrawArray(vao);
	}

	void Renderer::SubmitTrianglesArray(const std::shared_ptr<VertexArray>& vao)
	{
		s_API->DrawTriangles(vao);
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

	void Renderer::DrawArray(const std::shared_ptr<VertexArray>& vao)
	{
		s_API->DrawArray(vao);
	}
	void Renderer::DrawStack()
	{
		s_API->DrawStack();
	}
}