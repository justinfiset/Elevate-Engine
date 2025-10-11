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
    Renderer::PushRenderState({ false, true, false });
	// Render the lines
    m_lineShader->Bind();
    m_lineShader->UpdateCamera();
	Renderer::DrawArray(m_lineArray, DrawPrimitiveType::Lines);
    m_debugLineArray.clear();
}

void Elevate::DebugRenderer::InitLineRender()
{
    m_lineShader = ShaderManager::LoadShader("Debug", "editor/shaders/debug.vert", "editor/shaders/debug.frag");

	m_lineBuffer.reset(VertexBuffer::Create(m_debugLineArray.data(), (uint32_t)m_debugLineArray.size() * sizeof(DebugVertex)));
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
    DebugRenderer& instance = Get();
    instance.m_debugLineArray.push_back({ line.Start, line.Color });
    instance.m_debugLineArray.push_back({ line.End,   line.Color });

    if (instance.m_lineBuffer)
    {
        instance.m_lineBuffer->SetData(instance.m_debugLineArray.data(), (uint32_t)instance.m_debugLineArray.size() * sizeof(DebugVertex));
    }
}