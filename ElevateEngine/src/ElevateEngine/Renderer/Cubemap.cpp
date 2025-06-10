#include "eepch.h"

#include "Cubemap.h"
#include "ElevateEngine/Renderer/Renderer.h"

#include <glad/glad.h>

#include <stb/stb_image.h>

#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>

Elevate::Cubemap::Cubemap(std::string paths[6], std::string skyboxFilePath)
{
	m_filePath = skyboxFilePath;

	// Creating the Layout and the VertexBuffer
	m_VertexBuffer.reset(Elevate::VertexBuffer::Create(&s_skyboxVertices[0], sizeof(s_skyboxVertices)));
	// Creating the layout sent to the shader and the layout of the buffer
	m_VertexBuffer->SetLayout({ // The layout is based on the Vertex struct (see Vertex.h)
		{ Elevate::ShaderDataType::Float3, "a_Position" },
	});
	m_IndexBuffer.reset(Elevate::IndexBuffer::Create(&s_skyboxIndices[0], sizeof(s_skyboxIndices) / sizeof(unsigned int)));
	m_VertexArray.reset(VertexArray::Create());
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	m_VertexArray->Unbind();


	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

	int width, height, nrChannels;
	for (int i = 0; i < 6; i++)
	{
		unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			EE_CORE_TRACE("Unable to load cubemap texture [{0}] : {1}", i, paths[i].c_str());
		}
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// We load the shaders from the files in the solution as this shader is rarely modified by the user
	std::string vert = 
		#include "ElevateEngine/Renderer/Cubemap/skybox.vert"
	;
	std::string frag = 
		#include "ElevateEngine/Renderer/Cubemap/skybox.frag"
	;
	m_cubemapShader = Elevate::Shader::Create(vert, frag);
}

// TODO USE CUSTOM LOADER INSTEADD OF REPETITIVE CODE
Elevate::Cubemap* Elevate::Cubemap::CreateFromFile(std::string filePath)
{
	///////////////
	FILE* fp = fopen(filePath.c_str(), "r");
	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer,
		sizeof(readBuffer));
	rapidjson::Document doc;
	doc.ParseStream(is);

	fclose(fp);
	////////////////

	// TODO: add error handling to make sure all required members are present in the file
	std::string paths[6] =
	{
		doc["right"].GetString(),
		doc["left"].GetString(),
		doc["up"].GetString(),
		doc["down"].GetString(),
		doc["front"].GetString(),
		doc["back"].GetString()
	};

	return new Cubemap(paths, filePath);
}

const void Elevate::Cubemap::Draw()
{
	this->Draw(m_cubemapShader);
}

void Elevate::Cubemap::Draw(std::shared_ptr<Shader> shader)
{
	bool reActiveCull = glIsEnabled(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);

	shader->Bind();
	glDepthMask(GL_FALSE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
	Renderer::SubmitVertexArray(m_VertexArray);
	glDepthMask(GL_TRUE);

	if (reActiveCull) glEnable(GL_CULL_FACE);
}


// TODO PREVENT CODE REPETITION
void Elevate::Cubemap::SetProjectionMatrix(glm::mat4 proj)
{
	m_cubemapShader->Bind();
	m_cubemapShader->SetUniformMatrix4fv("projection", proj);
}

void Elevate::Cubemap::SetViewMatrix(glm::mat4 view)
{
	m_cubemapShader->Bind();
	m_cubemapShader->SetUniformMatrix4fv("view", view);
}

std::string Elevate::Cubemap::GetFilePath()
{
	return m_filePath;
}
