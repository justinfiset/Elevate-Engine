#include "eepch.h"
#include "OpenGLRendererAPI.h"
#include <glad/glad.h>

void Elevate::OpenGLRendererAPI::SetClearColor(const glm::vec4 color) const
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void Elevate::OpenGLRendererAPI::Clear() const
{
	// todo a changer on ne veux pas clear les deux en meme temps
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Elevate::OpenGLRendererAPI::DrawArray(const std::shared_ptr<VertexArray>& vao) const
{
	vao->Bind();
	glDrawElements(GL_TRIANGLES, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Elevate::OpenGLRendererAPI::DrawStack() const
{
	// TODO optimize texture binding to prevent multiple binding of the same texture
	// Batch directement en models pour que seul le model contienne les textures donc réduction de la mémoire
	for (const auto& pair : m_MeshStack)
	{
		const auto& shader = pair.first;
		pair.first->Bind(); // binding the shader
		for (const auto& mesh : pair.second) 
		{
			// bind appropriate textures
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;

			int textureCount = 0;
			for (const auto& texture : mesh.GetTextures())
			{
				// todo remove verification / just a test to see if texture is the problem for fps decrease
				if (!texture->IsBound())
				{
					// retrieve texture number (the N in diffuse_textureN)
					std::string number;
					std::string name = texture->GetType();
					if (name == "material.diffuse")
						number = std::to_string(diffuseNr++);
					else if (name == "material.specular")
						number = std::to_string(specularNr++); // transfer unsigned int to string
					else if (name == "material.normal")
						number = std::to_string(normalNr++); // transfer unsigned int to string
					else if (name == "material.height")
						number = std::to_string(heightNr++); // transfer unsigned int to string

					// now set the sampler to the correct texture unit
					shader->SetUniform1i((name /* + number*/).c_str(), textureCount);
					texture->Bind(textureCount);

					textureCount++;
				}
			}

			// todo push un vao dans un stack
			std::shared_ptr<VertexArray> vao(Elevate::VertexArray::Create());
			vao->AddVertexBuffer(mesh.GetVertexBuffer());
			vao->SetIndexBuffer(mesh.GetIndexBuffer());
			DrawArray(vao);

			// todo maybe remove le unbind car on doit rebind apres
			for (const auto& texture : mesh.GetTextures())
			{
				texture->Unbind();
			}
			glActiveTexture(GL_TEXTURE0);
		}
	}
}

// todo verif si nécessaire
void Elevate::OpenGLRendererAPI::DrawTriangles(const std::shared_ptr<VertexArray>& vao) const
{
	vao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

