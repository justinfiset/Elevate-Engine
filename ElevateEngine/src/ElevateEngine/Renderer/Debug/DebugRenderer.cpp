#include "eepch.h"

#ifdef EE_EDITOR_BUILD

#include "DebugRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <ElevateEngine/Core/Core.h>
#include <ElevateEngine/Core/Application.h>
#include <ElevateEngine/Renderer/Shader/Shader.h>
#include <ElevateEngine/Renderer/Renderer.h>
#include <ElevateEngine/Renderer/Buffer.h>

void Elevate::DebugRenderer::Init()
{
	Get().InternalInit();
}

void Elevate::DebugRenderer::Render()
{
	if (Application::GetGameState() == GameContextState::EditorMode)
	{
		Get().InternalRender();
	}
}

void Elevate::DebugRenderer::AddDebugCone(const glm::vec3& origin, const glm::vec3& direction, float radius, float range, uint16_t segmentCount, const glm::vec4& color)
{
	constexpr float TWO_PI = glm::two_pi<float>();

	glm::vec3 forward = glm::normalize(direction);
	glm::vec3 reference = (std::abs(forward.y) < 0.999f) ? glm::vec3(0.0f, 1.0f, 0.0f) : glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 center = origin + direction * range;
	glm::vec3 right = glm::normalize(glm::cross(forward, reference));
	glm::vec3 up = glm::normalize(glm::cross(right, forward));

	for (int i = 0; i < segmentCount; i++)
	{
		float angle0 = TWO_PI * static_cast<float>(i) / segmentCount;
		float angle1 = TWO_PI * static_cast<float>(i + 1) / segmentCount;

		glm::vec3 p0 = center + (right * std::cos(angle0) + up * std::sin(angle0)) * radius;
		glm::vec3 p1 = center + (right * std::cos(angle1) + up * std::sin(angle1)) * radius;

		AddDebugLine({ p0, p1, color });
		AddDebugLine({ p0, origin, color });
	}
}

void Elevate::DebugRenderer::InternalInit()
{
	InitLineRender();
}

void Elevate::DebugRenderer::InternalRender()
{
	RenderState state;
	state.BlendEnable = false;
	state.CullMode = CullFace::None;
	state.DepthTest = true;
	state.DepthWrite = true;
	Renderer::PushRenderState(state);
	// Render the lines
	Renderer::ApplySystemUniforms(m_lineShader);
	Renderer::DrawArray(m_lineArray, DrawPrimitiveType::Lines);
	ClearDebugLines();
}

void Elevate::DebugRenderer::InitLineRender()
{
	m_lineShader = ShaderManager::LoadShader(
		"Debug",
		"editor://Shaders/debug.vert",
		"editor://Shaders/debug.frag",
		EE_SHADER_HEADER,
		EE_SHADER_HEADER
	);

	m_lineBuffer.reset(VertexBuffer::Create(m_debugLineArray.data(), (uint32_t)m_debugLineArray.size() * sizeof(DebugVertex)));
	m_lineBuffer->SetLayout({
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" },
	});

	m_lineArray.reset(VertexArray::Create());
	m_lineArray->AddVertexBuffer(m_lineBuffer);
	m_lineArray->Unbind();
}

void Elevate::DebugRenderer::ClearDebugLines()
{
	if (m_debugLineArray.size() > 0)
	{
		m_debugLineArray.clear();

		if (m_lineBuffer)
		{
			m_lineBuffer->Resize(0);
		}
	}
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

#endif
