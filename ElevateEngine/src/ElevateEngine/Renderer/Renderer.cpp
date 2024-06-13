#include "eepch.h"
#include "Renderer.h"

#include "OpenGLRendererAPI.h"

namespace Elevate
{
	RendererAPI* Renderer::s_API = new OpenGLRendererAPI();

	void Renderer::BeginSceneFrame()
	{

	}
	void Renderer::EndSceneFrame()
	{

	}
	void Renderer::SubmitMeshes(const std::shared_ptr<VertexArray>& vao)
	{
		Renderer::DrawStack(vao);
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
	void Renderer::DrawStack(const std::shared_ptr<VertexArray>& vao)
	{
		s_API->DrawStack(vao);
	}
}