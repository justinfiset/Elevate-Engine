#include "eepch.h"

#include "Cubemap.h"
#include <ElevateEngine/Core/PathResolver.h>
#include <ElevateEngine/Renderer/GraphicsAPI.h>
#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Renderer/GLDebug.h>

#include <stb/stb_image.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>

Elevate::Cubemap::Cubemap(std::string paths[6], std::string skyboxFilePath)
{
	m_renderState.Cullface = false;
	m_renderState.DepthWrite = false;
	m_renderState.DepthTest = false;

	m_filePath = skyboxFilePath;

	// Vertex & Index Buffers
	m_VertexBuffer.reset(Elevate::VertexBuffer::Create(&s_skyboxVertices[0], sizeof(s_skyboxVertices)));
	m_VertexBuffer->SetLayout({
		{ Elevate::ShaderDataType::Float3, "a_Position" },
		});

	m_IndexBuffer.reset(Elevate::IndexBuffer::Create(&s_skyboxIndices[0], sizeof(s_skyboxIndices) / sizeof(unsigned int)));
	m_VertexArray.reset(VertexArray::Create());
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	m_VertexArray->Unbind();

	// Texture
	GLCheck(glGenTextures(1, &m_textureID));
	GLCheck(glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID));

	int width, height, nrChannels;
	for (int i = 0; i < 6; i++)
	{
		unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			GLCheck(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
			stbi_image_free(data);
		}
		else
		{
			EE_CORE_ERROR("Error : Unable to load cubemap texture [{}] : {} with the following skybox file : {}", i, paths[i].c_str(), skyboxFilePath);
			return;
		}
	}
	GLCheck(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));

	GLCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

	// Shaders
	std::string vert = std::string(EE_SHADER_HEADER) +
#include "ElevateEngine/Renderer/Cubemap/skybox.vert"
		;
	std::string frag = std::string(EE_SHADER_HEADER) +
#include "ElevateEngine/Renderer/Cubemap/skybox.frag"
		;
	m_cubemapShader = Elevate::Shader::Create(vert, frag);
}

Elevate::Cubemap* Elevate::Cubemap::CreateFromFile(std::string filePath)
{
	std::string resolvedPath = PathResolver::Resolve(filePath);
	FILE* fp = fopen(resolvedPath.c_str(), "r");
	if (!fp)
	{
		EE_CORE_ERROR("Error : Cannot open cubemap config file ({})", resolvedPath);
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
		PathResolver::Resolve(doc["right"].GetString()),
		PathResolver::Resolve(doc["left"].GetString()),
		PathResolver::Resolve(doc["up"].GetString()),
		PathResolver::Resolve(doc["down"].GetString()),
		PathResolver::Resolve(doc["front"].GetString()),
		PathResolver::Resolve(doc["back"].GetString())
	};

	return new Cubemap(paths, resolvedPath);
}

void Elevate::Cubemap::Draw(std::shared_ptr<Shader> shader)
{
	if (!shader) shader = m_cubemapShader;

	// todo send as a render command
	Renderer::PushRenderState(m_renderState);
	Renderer::BindShader(shader);
	GLCheck(glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID));
	Renderer::DrawArray(m_VertexArray);
}

void Elevate::Cubemap::SetProjectionMatrix(glm::mat4 proj)
{
	Renderer::BindShader(m_cubemapShader);
	m_cubemapShader->SetUniformMatrix4fv("projection", proj);
}

void Elevate::Cubemap::SetViewMatrix(glm::mat4 view)
{
	Renderer::BindShader(m_cubemapShader);
	m_cubemapShader->SetUniformMatrix4fv("view", view);
}

std::string Elevate::Cubemap::GetFilePath()
{
	return m_filePath;
}
