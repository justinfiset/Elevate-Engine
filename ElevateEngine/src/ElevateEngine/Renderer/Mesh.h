#pragma once

#include <vector>

#include "VertexArray.h"
#include "Buffer.h"
#include "Vertex.h"
#include <ElevateEngine/Renderer/Texture/Texture.h>
#include <ElevateEngine/Renderer/Shader/Shader.h>

namespace Elevate
{
    // TODO : CONVERT CODE TO USE THIS STRUCT
    struct MeshData {
        std::vector<Vertex> Vertices;
        std::vector<uint32_t> Indices;
        std::vector<Texture*> Textures;
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
        Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<std::shared_ptr<Texture>> textures);
        static Mesh* Create(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<std::shared_ptr<Texture>> textures);

        inline std::shared_ptr<VertexBuffer> GetVertexBuffer() const { return m_VertexBuffer; }
        inline std::shared_ptr<IndexBuffer> GetIndexBuffer() const { return m_IndexBuffer; }

        inline const std::vector<std::shared_ptr<Texture>>& GetTextures() const { return m_Textures; }

        // TODO REMOVE ASAP ONLY FOR TROUBLESHOOTING PURPOSES
        void Draw(std::shared_ptr<Shader> shader);

        static Mesh GenerateCube(float size = 1.0f);
        static Mesh GenerateUVSphere(float radius, int latitudes, int longitudes);
        static Mesh GenerateCubephere(float radius, int subdivision);
        static Mesh GenerateQuad(float size = 1.0f);
        static Mesh GeneratePlane(float size, int resolution);

        static Mesh CombineMeshes(std::vector<Mesh>& meshes)
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
                    indices.push_back(index + (uint32_t) indexOffset);
                }
                indexOffset += mesh.m_Indices.size();
                 
                for (std::shared_ptr<Texture> tex : mesh.m_Textures)
                {
                    bool found = false;
                    for (std::shared_ptr<Texture> insertedTex : textures)
                    {
                        if (insertedTex->GetID() == tex->GetID())
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

        /*
        TODO: GENERATIONS FOR THESE PRIMITIVES : 
        Cubesphere,
        Icosphere,
        Cylinder,
        Capsule,
        Torus,
        */
    private:
        std::shared_ptr<Elevate::VertexArray> m_VertexArray;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;

        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
        std::vector<std::shared_ptr<Texture>> m_Textures;
    };
}