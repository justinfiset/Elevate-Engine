#include "eepch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

void Elevate::OpenGLRendererAPI::SetClearColor(const glm::vec4 color) const
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void Elevate::OpenGLRendererAPI::Clear() const
{
	// todo a changer on ne veux pas clear les deux en meme temps
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Elevate::OpenGLRendererAPI::DrawArray(const std::shared_ptr<VertexArray>& vao) const
{
	vao->Bind();
	glDrawElements(GL_TRIANGLES, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	vao->Unbind();
}

void Elevate::OpenGLRendererAPI::DrawStack(const std::shared_ptr<Shader> shader) const
{
	//TODO enable for multiple shader and do batching to match objects corresponding to each shader
	// TODO do the actual stack rendering

	for (size_t i = 0; i < m_MeshStack.size(); ++i) {
		std::shared_ptr<VertexBuffer> vb = m_MeshStack[i].GetVertexBuffer();
		std::shared_ptr<IndexBuffer> ib = m_MeshStack[i].GetIndexBuffer();
		std::shared_ptr<VertexArray> vao(Elevate::VertexArray::Create());
		vao->AddVertexBuffer(vb);
		vao->SetIndexBuffer(ib);
		//std::shared_ptr<VertexArray> vao(Elevate::VertexArray::Create());
		//vao->AddVertexBuffer(m_MeshStack[i]->GetVertexBuffer());
		//vao->SetIndexBuffer(m_MeshStack[i]->GetIndexBuffer());
		DrawArray(vao);
	}

	// Setting back to deafault (good practice)
	glActiveTexture(GL_TEXTURE0);
}

void Elevate::OpenGLRendererAPI::DrawTriangles(const std::shared_ptr<VertexArray>& vao) const
{
	vao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

