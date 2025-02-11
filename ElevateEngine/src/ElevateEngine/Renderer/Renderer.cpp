#include "eepch.h"
#include "Renderer.h"

#include "ElevateEngine/Renderer/OpenGL/OpenGLRendererAPI.h"

namespace Elevate
{
	RendererAPI* Renderer::s_API = new OpenGLRendererAPI();

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
	void Renderer::SetClearColor(const glm::vec4 color)
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

	void Renderer::SetViewPort(int x, int y, int width, int height)
	{
		s_API->SetViewPort(x, y, width, height);
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