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
		{ Elevate::ShaderDataType::Float2, "a_TexCord" }
	});

	//Creating the IndexBuffer (containing indices)
	m_IndexBuffer.reset(Elevate::IndexBuffer::Create(&indices[0], indices.size()));
	
	m_VertexArray.reset(VertexArray::Create());
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);

	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	//glBindVertexArray(VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//// Position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//glEnableVertexAttribArray(0);

	//// Normal attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	//glEnableVertexAttribArray(1);

	//// Texture coord attribute
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	//glEnableVertexAttribArray(2);

	//glBindVertexArray(0);
}

Elevate::Mesh* Elevate::Mesh::Create(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<std::shared_ptr<Texture>> textures)
{
	return new Mesh(vertices, indices, textures);
}

void Elevate::Mesh::Draw()
{
	// TODO add the texture binding to get the textures from the model
	m_VertexArray->Bind();
	glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
	m_VertexArray->Unbind(); // Always good to unbind before doing something else
	glActiveTexture(GL_TEXTURE0);
}
