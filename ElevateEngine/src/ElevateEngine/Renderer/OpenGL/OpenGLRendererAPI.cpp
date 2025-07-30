#include "eepch.h"
#include "OpenGLRendererAPI.h"
#include <glad/glad.h>

#include <ElevateEngine/Renderer/GLDebug.h>

namespace Elevate
{
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) const
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::SetViewport(int x, int y, int width, int height) const
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::FlushBuffers() const
	{
		glFlush();
	}

	void OpenGLRendererAPI::DrawArray(const std::shared_ptr<VertexArray>& vao) const
	{
		vao->Bind();
		GLCheck(glDrawElements(GL_TRIANGLES, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLRendererAPI::DrawStack() const
	{
		// TODO EITHER IMPL OR REMOVE
		// TODO optimize texture binding to prevent multiple binding of the same texture
		// Batch directement en 
		//for (const auto& pair : m_MeshStack)
		//{
		//	const auto& shader = pair.first;
		//	pair.first->Bind(); // binding the shader
		//	for (const auto& mesh : pair.second)
		//	{
		//		// bind appropriate textures
		//		unsigned int diffuseNr = 1;
		//		unsigned int specularNr = 1;
		//		unsigned int normalNr = 1;
		//		unsigned int heightNr = 1;

		//		int textureCount = 0;
		//		for (const auto& texture : mesh.GetTextures())
		//		{
		//			// todo remove verification / just a test to see if texture is the problem for fps decrease
		//			if (!texture->IsBound())
		//			{
		//				// retrieve texture number (the N in diffuse_textureN)
		//				std::string number;
		//				std::string name = texture->GetType();
		//				if (name == "material.diffuse")
		//					number = std::to_string(diffuseNr++);
		//				else if (name == "material.specular")
		//					number = std::to_string(specularNr++); // transfer unsigned int to string
		//				else if (name == "material.normal")
		//					number = std::to_string(normalNr++); // transfer unsigned int to string
		//				else if (name == "material.height")
		//					number = std::to_string(heightNr++); // transfer unsigned int to string

		//				// now set the sampler to the correct texture unit
		//				shader->SetUniform1i((name /* + number*/).c_str(), textureCount);
		//				texture->Bind(textureCount);

		//				textureCount++;
		//			}
		//		}

		//		DrawArray(mesh.GetVertexArray());

		//		// todo maybe remove le unbind car on doit rebind apres
		//		for (const auto& texture : mesh.GetTextures())
		//		{
		//			texture->Unbind();
		//		}
		//		glActiveTexture(GL_TEXTURE0);
		//	}
		//}
	}

	void OpenGLRendererAPI::SetCullingState(bool enabled) const
	{
		if(enabled) glEnable(GL_CULL_FACE);
		else        glDisable(GL_CULL_FACE);
	}
	void OpenGLRendererAPI::SetDepthWrittingState(bool enabled) const
	{
		glDepthMask(enabled ? GL_TRUE : GL_FALSE);
	}
	void OpenGLRendererAPI::SetDepthTestingState(bool enabled) const
	{
		if (enabled) glEnable(GL_DEPTH_TEST);
		else        glDisable(GL_DEPTH_TEST);
	}
}
