#include "eepch.h"
#include "Renderer.h"

#include "ElevateEngine/Renderer/OpenGL/OpenGLRendererAPI.h"

namespace Elevate
{
	RendererAPI* Renderer::s_API = new OpenGLRendererAPI();

	void Renderer::BeginSceneFrame(const std::shared_ptr<Shader> shader)
	{
	}
	void Renderer::EndSceneFrame(const std::shared_ptr<Shader> shader)
	{
	}

	void Renderer::SubmitModel(const Model model)
	{
		// Sends all the individual meshes to the draw stack
		for each (Mesh mesh in model.GetMeshes())
		{
			Renderer::SubmitMesh(mesh);
		}
	}

	void Renderer::SubmitMesh(const Mesh mesh)
	{
		EE_CORE_TRACE("called");
		s_API->SubmitMesh(mesh);
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
	void Renderer::SetClearColor(const glm::vec4 color)
	{
		s_API->SetClearColor(color);
	}
	void Renderer::Clear()
	{
		s_API->Clear();
	}

	void Renderer::DrawArray(const std::shared_ptr<VertexArray>& vao)
	{
		s_API->DrawArray(vao);
	}
	void Renderer::DrawStack(std::shared_ptr<Shader> shader)
	{
		s_API->DrawStack(shader);
	}
}