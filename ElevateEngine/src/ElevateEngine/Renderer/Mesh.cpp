#include "eepch.h"
#include "Mesh.h"

#include <glad/glad.h>
#include <ElevateEngine/Renderer/Texture/TextureManager.h>
#include <ElevateEngine/Renderer/Renderer.h>

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

		if (m_Textures.empty())
		{
			m_Textures.push_back(TextureManager::GetDefaultTexture());
		}
	}

	Mesh* Mesh::Create(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<std::shared_ptr<Texture>> textures)
	{
		return new Mesh(vertices, indices, textures);
	}

	void Mesh::Draw(std::shared_ptr<Shader> shader)
	{
		for (unsigned int i = 0; i < m_Textures.size(); i++)
		{
			m_Textures[i]->Bind(i);
		}

		Renderer::DrawArray(m_VertexArray);

		for (unsigned int i = 0; i < m_Textures.size(); i++)
		{
			// TODO REMOVE AND FIND OUT WHY SOME TEXTURES ARE NULL

			m_Textures[i]->Unbind();
		}
	}

	Mesh Mesh::GenerateCube(float size)
	{
		const float halfSize = size * 0.5f;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		auto addFace = [&](glm::vec3 normal, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
			uint32_t startIndex = (uint32_t) vertices.size();

			Vertex a{ v0, normal, {0.0f, 0.0f} };
			Vertex b{ v1, normal, {1.0f, 0.0f} };
			Vertex c{ v2, normal, {1.0f, 1.0f} };
			Vertex d{ v3, normal, {0.0f, 1.0f} };

			vertices.push_back(a);
			vertices.push_back(b);
			vertices.push_back(c);
			vertices.push_back(d);

			indices.push_back(startIndex);
			indices.push_back(startIndex + 1);
			indices.push_back(startIndex + 2);
			indices.push_back(startIndex);
			indices.push_back(startIndex + 2);
			indices.push_back(startIndex + 3);
		};

		// Front (+Z)
		addFace({ 0, 0, 1 },
			{ -halfSize, -halfSize, halfSize },
			{ halfSize, -halfSize, halfSize },
			{ halfSize,  halfSize, halfSize },
			{ -halfSize,  halfSize, halfSize });

		// Back (-Z)
		addFace({ 0, 0, -1 },
			{ halfSize, -halfSize, -halfSize },
			{ -halfSize, -halfSize, -halfSize },
			{ -halfSize,  halfSize, -halfSize },
			{ halfSize,  halfSize, -halfSize });

		// Right (+X)
		addFace({ 1, 0, 0 },
			{ halfSize, -halfSize, halfSize },
			{ halfSize, -halfSize, -halfSize },
			{ halfSize,  halfSize, -halfSize },
			{ halfSize,  halfSize, halfSize });

		// Left (-X)
		addFace({ -1, 0, 0 },
			{ -halfSize, -halfSize, -halfSize },
			{ -halfSize, -halfSize, halfSize },
			{ -halfSize,  halfSize, halfSize },
			{ -halfSize,  halfSize, -halfSize });

		// Top (+Y)
		addFace({ 0, 1, 0 },
			{ -halfSize, halfSize, halfSize },
			{ halfSize,  halfSize, halfSize },
			{ halfSize,  halfSize, -halfSize },
			{ -halfSize, halfSize, -halfSize });

		// Bottom (-Y)
		addFace({ 0, -1, 0 },
			{ -halfSize, -halfSize, -halfSize },
			{ halfSize,  -halfSize, -halfSize },
			{ halfSize,  -halfSize, halfSize },
			{ -halfSize, -halfSize, halfSize });

		return Mesh(vertices, indices, {});
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

	Mesh Mesh::GeneratePlane(float size, int resolution) {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		float step = size / resolution;
		float offset = size * 0.5f;

		for (int i = 0; i <= resolution; i++) {
			for (int j = 0; j <= resolution; j++) {
				Vertex v;
				v.Position = { j * step - offset, 0.0f, i * step - offset };
				v.Normal = { 0.0f, 1.0f, 0.0f };
				v.TexCoords = { (float)j / resolution, (float)i / resolution };
				vertices.push_back(v);
			}
		}

		for (int i = 0; i < resolution; i++) {
			for (int j = 0; j < resolution; j++) {
				int row1 = i * (resolution + 1);
				int row2 = (i + 1) * (resolution + 1);

				uint32_t v0 = row1 + j;
				uint32_t v1 = row1 + j + 1;
				uint32_t v2 = row2 + j;
				uint32_t v3 = row2 + j + 1;

				indices.push_back(v0);
				indices.push_back(v2);
				indices.push_back(v1);

				indices.push_back(v1);
				indices.push_back(v2);
				indices.push_back(v3);
			}
		}

		return Mesh(vertices, indices, {});
	}

	Mesh Mesh::CombineMeshes(std::vector<Mesh>& meshes)
	{
		size_t indexOffset = 0;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<std::shared_ptr<Texture>> textures;

		for (const Mesh& mesh : meshes)
		{
			vertices.insert(
				vertices.end(),
				mesh.m_Vertices.begin(),
				mesh.m_Vertices.end()
			);

			for (uint32_t index : mesh.m_Indices)
			{
				indices.push_back(index + (uint32_t)indexOffset);
			}
			indexOffset += mesh.m_Vertices.size();

			for (std::shared_ptr<Texture> tex : mesh.m_Textures)
			{
				if (!tex) continue; // TODO FIND IF AND WHY A TEXTURE COULD BE NULL

				bool found = false;
				for (std::shared_ptr<Texture> insertedTex : textures)
				{
					if (insertedTex->GetNativeHandle() == tex->GetNativeHandle())
					{
						found = true;
						break;
					}
				}

				if (!found)
				{
					textures.push_back(tex);
				}
			}
		}

		return Mesh(vertices, indices, textures);
	}
}
