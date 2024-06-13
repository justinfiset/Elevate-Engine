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
	glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);
}

void Elevate::OpenGLRendererAPI::DrawStack(const std::shared_ptr<VertexArray>& vao) const
{
	glDrawElements(GL_TRIANGLES, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}
