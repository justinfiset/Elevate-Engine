#include "eepch.h"
#include "Mesh.h"
#include <glad/glad.h>

Elevate::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<std::shared_ptr<Texture>> textures)
	: m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
{ 
	// Creating the Layout and the VertexBuffer
	m_VertexBuffer.reset(Elevate::VertexBuffer::Create(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex)));
	// Creating the layout sent to the shader and the layout of the buffer
	m_VertexBuffer->SetLayout({ // The layout is based on the Vertex struct (see Vertex.h)
		{ Elevate::ShaderDataType::Float3, "a_Position" },
		{ Elevate::ShaderDataType::Float3, "a_Normal" },
		{ Elevate::ShaderDataType::Float2, "a_TexCord" },
		{ Elevate::ShaderDataType::Float3, "a_Tangent" },
		{ Elevate::ShaderDataType::Float3, "a_Bitangent" }
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

// TODO optimiser la gestions des texture et retirer la gestion de opengl d'ici
void Elevate::Mesh::Draw(std::shared_ptr<Shader> shader)
{
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;


	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = m_Textures[i]->GetType();
		if (name == "material.diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "material.specular")
			number = std::to_string(specularNr++); // transfer unsigned int to string
		else if (name == "material.normal")
			number = std::to_string(normalNr++); // transfer unsigned int to string
		else if (name == "material.height")
			number = std::to_string(heightNr++); // transfer unsigned int to string

		// now set the sampler to the correct texture unit
		shader->SetUniform1i((name /* + number*/).c_str(), i);

		// and finally bind the texture
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		glBindTexture(GL_TEXTURE_2D, m_Textures[i]->GetID());
		m_Textures[i]->Bind(i);
	}

	m_VertexArray->Bind();
	glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
	m_VertexArray->Unbind(); // Always good to unbind before doing something else

	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		m_Textures[i]->Unbind(i);
	}
	glActiveTexture(GL_TEXTURE0);
}
