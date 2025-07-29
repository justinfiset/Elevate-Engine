#pragma once
#include <string>
#include <memory>
#include <filesystem>

#include <ElevateEngine/Renderer/Buffer.h>
#include <ElevateEngine/Renderer/VertexArray.h>
#include <ElevateEngine/Renderer/Shader/Shader.h>
#include <ElevateEngine/Renderer/RenderState.h>

namespace Elevate
{
    class Shader;

	class Cubemap
	{
	public:
		Cubemap(std::string paths[6], std::string skyboxFilePath = "");

        static Cubemap* CreateFromFile(std::string filePath);

        const void Draw();
        void Draw(std::shared_ptr<Shader> shader);

        void SetProjectionMatrix(glm::mat4 data);
        void SetViewMatrix(glm::mat4 data);

        std::string GetFilePath();

	private:
        RenderState m_renderState;
        std::shared_ptr<Shader> m_cubemapShader;

        std::shared_ptr<VertexArray> m_VertexArray;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;

        uint32_t m_textureID; // todo maybe move to the opengl texture class if not needed in other APIs

        std::string m_filePath;

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

        unsigned int s_skyboxIndices[6 * 2 *3] = {
            // Front
            0, 1, 2,
            2, 3, 0,
            // Back
            4, 5, 6,
            6, 7, 4,
            // Left
            0, 3, 7,
            7, 4, 0,
            // Right
            1, 2, 6,
            6, 5, 1,
            // Top
            3, 2, 6,
            6, 7, 3,
            // Bottom
            0, 1, 5,
            5, 4, 0
        };
	};
}