#include "eepch.h"
#include "ElevateEngine/Renderer/RendererAPI.h"

namespace Elevate
{
	// todo set the rendererAPI dinamicly and not staticly
	RendererAPI::GraphicAPI RendererAPI::s_ActiveAPI = RendererAPI::GraphicAPI::OpenGL;

	void RendererAPI::SubmitModel(const Model& model)
	{
		m_ModelStack[model.GetShader()].push_back(model);
	}

	void RendererAPI::RemoveModel(const Model& model)
	{
		std::vector<Model>& stack = m_ModelStack[model.GetShader()];

		auto it = std::find_if(stack.begin(), stack.end(),
			[&model](const Model& m) {
				return &m == &model; 
			}
		);

		if (it != stack.end()) // If the stack does contain the element to remove
		{
			stack.erase(it);
		}
	}

	void RendererAPI::Submitmesh(const std::shared_ptr<Shader>& shader, const Mesh& mesh)
	{
		m_MeshStack[shader].push_back(mesh);
	}
}