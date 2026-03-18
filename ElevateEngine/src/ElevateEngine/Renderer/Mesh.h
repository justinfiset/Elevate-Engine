#pragma once

#include <stdint.h>
#include <memory>
#include <vector>

#include <ElevateEngine/Renderer/Vertex.h>
#include <ElevateEngine/Renderer/Texture/Texture.h>

namespace Elevate
{
	class VertexBuffer;
	class IndexBuffer;
	class VertexArray;

	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
		std::vector<TexturePtr> Textures;

		MeshData() = default;
		MeshData(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<TexturePtr>& textures)
			: Vertices(vertices), Indices(indices), Textures(textures) { }
	};

	enum class PrimitiveType : uint8_t
	{
		Cube,
		UVSphere,
		Cubesphere,
		Icosphere,
		Cylinder,
		Capsule,
		Plane,
		Quad,
		Torus,
	};

	class Mesh {
	public:
		Mesh() = default;
		Mesh(const MeshData& data);
		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<std::shared_ptr<Texture>>& textures);

		static Mesh* Create(const MeshData& data);
		static Mesh* Create(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::vector<std::shared_ptr<Texture>>& textures);

		std::shared_ptr<VertexBuffer> GetVertexBuffer() const { return m_VertexBuffer; }
		std::shared_ptr<IndexBuffer> GetIndexBuffer() const { return m_IndexBuffer; }
		std::shared_ptr<VertexArray> GetVertexArray() const { return m_VertexArray; }
		const std::vector<std::shared_ptr<Texture>>& GetTextures() const { return m_data.Textures; }

		static Mesh GenerateCube(float size = 1.0f);
		static Mesh GenerateUVSphere(float radius, int latitudes, int longitudes);
		static Mesh GenerateCubephere(float radius, int subdivision);
		static Mesh GenerateQuad(float size = 1.0f);
		static Mesh GeneratePlane(float size, int resolution);

		static Mesh CombineMeshes(std::vector<Mesh>& meshes);

		/*
		TODO: GENERATIONS FOR THESE PRIMITIVES : 
		Cubesphere,
		Icosphere,
		Cylinder,
		Capsule,
		Torus,
		*/
	private:
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;

		MeshData m_data;
	};
}