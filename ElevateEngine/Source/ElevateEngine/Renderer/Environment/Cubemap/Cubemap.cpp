module;

#include <cstddef>
#include <string>

#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>

#include <ElevateEngine/Renderer/RendererBackend.h>

module Elevate.Renderer.Cubemap;

import Elevate.Foundations.CoreLogger;
import Elevate.Foundations.Paths;

Elevate::Cubemap::Cubemap(std::string paths[6], std::string skyboxFilePath)
{
	m_renderState.CullMode = CullFace::None;
	m_renderState.DepthWrite = false;
	m_renderState.DepthTest = false;

	m_filePath = skyboxFilePath;

	// Vertex & Index Buffers
	m_VertexBuffer.reset(Elevate::VertexBuffer::Create(reinterpret_cast<const std::byte*>(s_skyboxVertices), sizeof(s_skyboxVertices)));
	m_VertexBuffer->SetLayout({
		{Elevate::ShaderDataType::Float3, "a_Position"},
		});

	m_IndexBuffer.reset(Elevate::IndexBuffer::Create(reinterpret_cast<const std::byte*>(s_skyboxIndices), sizeof(s_skyboxIndices) / sizeof(unsigned int)));
	m_VertexArray.reset(VertexArray::Create());
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	m_VertexArray->Unbind();

	// Shaders
	std::string vert = std::string(Internal::ShaderHeader) +
#include "ElevateEngine/Renderer/Environment/Cubemap/skybox.vert"
		;
	std::string frag = std::string(Internal::ShaderHeader) +
#include "ElevateEngine/Renderer/Environment/Cubemap/skybox.frag"
		;
	m_cubemapShader = Elevate::Shader::Create(vert, frag);

	// Texture
	const char* cubemapPaths[6] = {
		paths[0].c_str(), paths[1].c_str(), paths[2].c_str(),
		paths[3].c_str(), paths[4].c_str(), paths[5].c_str()
	};
	if (!RendererBackend::CreateCubemapTexture(cubemapPaths, m_textureID, skyboxFilePath.c_str()))
	{
		return;
	}
}

Elevate::Cubemap* Elevate::Cubemap::CreateFromFile(const std::string& filePath)
{
	auto resolvedPath = PathResolver::Resolve(filePath);

	FILE* fp = fopen(resolvedPath.string().c_str(), "r");
	if (!fp)
	{
		CoreLogger::Error("Error : Cannot open cubemap config file ({})", resolvedPath.string());
		return nullptr;
	}

	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	rapidjson::Document doc;
	doc.ParseStream(is);
	fclose(fp);

	// todo add more error catching, check if member exists or return
	std::string paths[6] =
	{
		PathResolver::Resolve(doc["right"].GetString()).string(),
		PathResolver::Resolve(doc["left"].GetString()).string(),
		PathResolver::Resolve(doc["up"].GetString()).string(),
		PathResolver::Resolve(doc["down"].GetString()).string(),
		PathResolver::Resolve(doc["front"].GetString()).string(),
		PathResolver::Resolve(doc["back"].GetString()).string()
	};

	return new Cubemap(paths, resolvedPath.string());
}

std::string Elevate::Cubemap::GetFilePath() const
{
	return m_filePath;
}
