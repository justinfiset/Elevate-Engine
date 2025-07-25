#include "eepch.h"
#include "Mesh.h"

#include <glad/glad.h>

namespace Elevate
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<std::shared_ptr<Texture>> textures)
		: m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
	{
		// Creating the Layout and the VertexBuffer
		m_VertexBuffer.reset(VertexBuffer::Create(&m_Vertices[0], (uint32_t)m_Vertices.size() * sizeof(Vertex)));
		// Creating the layout sent to the shader and the layout of the buffer
		m_VertexBuffer->SetLayout({ // The layout is based on the Vertex struct (see Vertex.h)
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCord" },
			{ ShaderDataType::Float3, "a_Tangent" },
			{ ShaderDataType::Float3, "a_Bitangent" }
			});

		//Creating the IndexBuffer (containing indices)
		m_IndexBuffer.reset(IndexBuffer::Create(&indices[0], (uint32_t)indices.size()));

		m_VertexArray.reset(VertexArray::Create());
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		m_VertexArray->Unbind();
	}

	Mesh* Mesh::Create(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<std::shared_ptr<Texture>> textures)
	{
		return new Mesh(vertices, indices, textures);
	}

	// TODO optimiser la gestions des texture et retirer la gestion de opengl d'ici
	void Mesh::Draw(std::shared_ptr<Shader> shader)
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

	Mesh Mesh::GenerateCube(float size)
	{
		return GenerateCubephere(size, 1);
	}

	Mesh Mesh::GenerateUVSphere(float radius, int latitudes, int longitudes)
	{
		if (longitudes < 3) longitudes = 3;
		if (latitudes < 2) latitudes = 2;

		const float pi = std::acos(-1.0f);
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		
		float latStep = pi / latitudes;
		float longStep = 2 * pi / longitudes;

		for (int lat = 0; lat <= latitudes; ++lat)
		{
			float phi = (pi / 2) - (lat * latStep); // From 90* down (starting from north pole basicly)
			const float xy = radius * cosf(phi);
			const float z = radius * sinf(phi);

			for (int lon = 0; lon <= longitudes; ++lon)
			{
				float theta = lon * longStep;

				Vertex v;
				v.Position = { 
					xy * cosf(theta),
					xy * sinf(theta),
					z
				};
				v.TexCoords = { 
					(float) lon / longitudes,
					(float) lat / latitudes 
				};
				v.Normal = glm::normalize(v.Position);
				vertices.push_back(v);
			}
		}

		for (int lat = 0; lat < latitudes; ++lat)
		{
			for (int lon = 0; lon < longitudes; ++lon)
			{
				const uint32_t curr = lat * (longitudes + 1) + lon;
				const uint32_t next = curr + longitudes + 1;

				indices.insert(indices.end(), {
					curr, next, curr + 1,
					next, next + 1, curr + 1
				});
			}
		}

		return Mesh(vertices, indices, {});
	}

	Mesh Mesh::GenerateCubephere(float radius, int subdivision)
	{
		return Mesh();
	}

	Mesh Mesh::GenerateQuad(float size)
	{
		return GeneratePlane(size, 1);
	}

	Mesh Mesh::GeneratePlane(float size, int resolution)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		float step = size / resolution;
		
		for (int i = 0; i <= resolution; i++)
		{
			for (int j = 0; j <= resolution; j++)
			{
				Vertex v;
				v.Position = { j * step, 0.0f, i * step };
				v.Normal = { 0.0f, 1.0f, 0.0f };
				v.TexCoords = { (float) j / resolution, (float) i / resolution };
				vertices.push_back(v);
			}
		}

		for (int i = 0; i < resolution; i++)
		{
			for (int j = 0; j < resolution; j++)
			{
				int row1 = i * (resolution + 1);
				int row2 = (i + 1) * (resolution + 1);

				uint32_t v0 = row1 + j;
				uint32_t v1 = row1 + j + 1;
				uint32_t v2 = row2 + j;
				uint32_t v3 = row2 + j + 1;

				// Creating the two triangles
				indices.push_back(v0);
				indices.push_back(v2);
				indices.push_back(v3);

				indices.push_back(v0);
				indices.push_back(v3);
				indices.push_back(v1);
			}
		}

		return Mesh(vertices, indices, {});
	}
}
