#pragma once

#include <vector>

#include "DebugTypes.h"

#include <ElevateEngine/Renderer/VertexArray.h>
#include <ElevateEngine/Renderer/Shader/ShaderManager.h>

namespace Elevate
{
	class DebugRenderer
	{
	private:
		static DebugRenderer& Get()
		{
			static DebugRenderer Instance;
			return Instance;
		}
	public:
		struct DebugVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
		};

		struct DebugLineData
		{
			glm::vec3 Start;
			glm::vec3 End;
			glm::vec4 Color;
		};

	public:
		static void Init();
		static void Render();

	private:
		void InternalInit();
		void InternalRender();

	private:
		std::shared_ptr<Shader> m_lineShader;
		std::shared_ptr<VertexArray> m_lineArray;
		std::shared_ptr<VertexBuffer> m_lineBuffer;
		std::vector<DebugVertex> m_debugLineArray;
		void InitLineRender();
	public:
		void AddDebugLine(DebugLineData line);
	};
}