#include "eepch.h"
#include "Mesh.h"
#include <glad/glad.h>

// todo remove
#include "Shader.h"

Elevate::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<std::shared_ptr<Texture>> textures)
	: m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
{ 
	// Creating the Layout and the VertexBuffer
	m_VertexBuffer.reset(Elevate::VertexBuffer::Create(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex)));
	// Creating the layout sent to the shader and the layout of the buffer
	m_VertexBuffer->SetLayout({ // The layout is based on the Vertex struct (see Vertex.h)
		{ Elevate::ShaderDataType::Float3, "a_Position" },
		{ Elevate::ShaderDataType::Float3, "a_Normal" },
		{ Elevate::ShaderDataType::Float2, "a_TexCord" }
	});

	//Creating the IndexBuffer (containing indices)
	m_IndexBuffer.reset(Elevate::IndexBuffer::Create(&indices[0], indices.size()));
	
	m_VertexArray.reset(VertexArray::Create());
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);

	m_VertexArray->Unbind();
}

Elevate::Mesh* Elevate::Mesh::Create(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<std::shared_ptr<Texture>> textures)
{
	return new Mesh(vertices, indices, textures);
}

void Elevate::Mesh::Draw(std::shared_ptr<Shader> shader)
{
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = m_Textures[i]->GetType();
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to string
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to string
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to string

		// now set the sampler to the correct texture unit
		shader->SetUniform1i((name + number).c_str(), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, m_Textures[i]->GetID());
	}

	m_VertexArray->Bind();
	glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
	m_VertexArray->Unbind(); // Always good to unbind before doing something else
	glActiveTexture(GL_TEXTURE0);
}
