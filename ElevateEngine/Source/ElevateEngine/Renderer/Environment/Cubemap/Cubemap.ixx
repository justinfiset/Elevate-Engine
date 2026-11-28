module;

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>

export module Elevate.Renderer.Cubemap;

import Elevate.Renderer.Buffers;
import Elevate.Renderer.RenderState;
import Elevate.Renderer.VertexArray;
import Elevate.Renderer.Shaders;

export namespace Elevate
{
	class Cubemap
	{
	public:
		Cubemap(std::string paths[6], std::string skyboxFilePath = "");

		static Cubemap* CreateFromFile(const std::string& filePath);

		const RenderState& GetRenderState() const { return m_renderState; }
		const std::shared_ptr<Shader>& GetShader() const { return m_cubemapShader; }
		const std::shared_ptr<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		uint32_t GetTextureID() const { return m_textureID; }

		std::string GetFilePath() const;

	private:
		RenderState m_renderState;
		std::shared_ptr<Shader> m_cubemapShader;

		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;

		uint32_t m_textureID = 0;
		std::string m_filePath;

		float s_skyboxVertices[3 * 8] = {
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f
		};

		unsigned int s_skyboxIndices[6 * 2 * 3] = {
			0, 1, 2,
			2, 3, 0,
			4, 5, 6,
			6, 7, 4,
			0, 3, 7,
			7, 4, 0,
			1, 2, 6,
			6, 5, 1,
			3, 2, 6,
			6, 7, 3,
			0, 1, 5,
			5, 4, 0
		};
	};
}
