#pragma once

#include <vector>

#include "VertexArray.h"
#include "Buffer.h"
#include "Vertex.h"
#include "Texture.h"

namespace Elevate
{
    class Mesh {
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<std::shared_ptr<Texture>> textures);
        static Mesh* Create(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<std::shared_ptr<Texture>> textures);
        
        // TODO draw from the renderer when we submit the mesh;
        //void Draw(Shader& shader);
        inline std::shared_ptr<VertexBuffer> GetVertexBuffer() const { return m_VertexBuffer; }
        inline std::shared_ptr<IndexBuffer> GetIndexBuffer() const { return m_IndexBuffer; }

        // TODO REMOVE ASAP ONLY FOR TROUBLESHOOTING PURPOSES
        void Draw();
    private:
        //unsigned int VAO, VBO, EBO;

        std::shared_ptr<Elevate::VertexArray> m_VertexArray;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;

        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
        std::vector<std::shared_ptr<Texture>> m_Textures;
    };
}