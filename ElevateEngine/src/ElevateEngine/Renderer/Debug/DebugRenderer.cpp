#include "eepch.h"
#include "DebugRenderer.h"

#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Renderer/Buffer.h>

void Elevate::DebugRenderer::Init()
{
	Get().InternalInit();
}

void Elevate::DebugRenderer::Render()
{
	Get().InternalRender();
}

void Elevate::DebugRenderer::InternalInit()
{
	InitLineRender();
}

void Elevate::DebugRenderer::InternalRender()
{
	// Render the lines
	m_lineShader->Bind();
    m_lineShader->UpdateCamera();
	Renderer::DrawArray(m_lineArray, DrawPrimitiveType::Lines);
	m_lineShader->Unbind();
}

void Elevate::DebugRenderer::InitLineRender()
{
	m_lineShader = ShaderManager::LoadShader("Debug", "editor/shaders/debug.vert", "editor/shaders/debug.frag");

    AddDebugLine({
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)  // Rouge
    });

    // Ligne verte verticale
    AddDebugLine({
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)  // Vert
        });

    // Ligne bleue diagonale
    AddDebugLine({
        glm::vec3(-0.5f, -0.5f, 0.0f),
        glm::vec3(0.5f, 0.5f, 0.0f),
        glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)  // Bleu
        });

    // Ligne jaune en Z
    AddDebugLine({
        glm::vec3(-0.5f, 0.5f, -0.5f),
        glm::vec3(-0.5f, 0.5f, 0.5f),
        glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)  // Jaune
        });

    // Carré blanc
    float size = 0.3f;
    glm::vec4 white = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    AddDebugLine({
        glm::vec3(-size, -size, 0.0f),
        glm::vec3(size, -size, 0.0f),
glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
        });

    AddDebugLine({
        glm::vec3(size, -size, 0.0f),
        glm::vec3(size, size, 0.0f),
glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
        });

    AddDebugLine({
        glm::vec3(size, size, 0.0f),
        glm::vec3(-size, size, 0.0f),
        {1.0f, 1.0f, 1.0f, 1.0f}
        });

    AddDebugLine({
        glm::vec3(-size, size, 0.0f),
        glm::vec3(-size, -size, 0.0f),
glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
        });

	m_lineBuffer.reset(VertexBuffer::Create(&m_debugLineArray[0], (uint32_t)m_debugLineArray.size() * sizeof(DebugVertex)));
	m_lineBuffer->SetLayout({
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" },
	});

    m_lineArray.reset(VertexArray::Create());
    m_lineArray->AddVertexBuffer(m_lineBuffer);
    m_lineArray->Unbind();
}

void Elevate::DebugRenderer::AddDebugLine(DebugLineData line)
{
	m_debugLineArray.push_back({ line.Start, line.Color });
	m_debugLineArray.push_back({ line.End,   line.Color });

    if (m_lineBuffer)
    {
        m_lineBuffer->Resize((uint32_t)m_debugLineArray.size() * sizeof(DebugVertex));
    }
}