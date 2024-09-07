#pragma once
#include <string>
#include "CubemapTexture.h"
#include <memory>
#include <filesystem>


#include "Buffer.h"
#include "VertexArray.h"
#include "Shader.h"

namespace Elevate
{
	class Cubemap
	{
	public:
		Cubemap(std::string paths[6]);

        static Cubemap* CreateFromFile(std::string filePath);

        const void Draw();
        void Draw(std::shared_ptr<Shader> shader);

        void SetProjectionMatrix(glm::mat4 data);
        void SetViewMatrix(glm::mat4 data);
	private:
        std::shared_ptr<Shader> m_cubemapShader;

		std::shared_ptr<CubemapTexture> m_textures[6];

        std::shared_ptr<Elevate::VertexArray> m_VertexArray;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;

        uint32_t m_textureID; // todo maybe move to the opengl texture class if not needed in other APIs

        float s_skyboxVertices[3 * 8] = {
            // Positions         
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f
        };

        // Les indices pour dessiner le cube en utilisant des triangles
        unsigned int s_skyboxIndices[6 * 2 *3] = {
            // Face avant
            0, 1, 2,
            2, 3, 0,
            // Face arri�re
            4, 5, 6,
            6, 7, 4,
            // Face gauche
            0, 3, 7,
            7, 4, 0,
            // Face droite
            1, 2, 6,
            6, 5, 1,
            // Face sup�rieure
            3, 2, 6,
            6, 7, 3,
            // Face inf�rieure
            0, 1, 5,
            5, 4, 0
        };
	};
}