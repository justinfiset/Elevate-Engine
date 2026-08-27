#pragma once

#ifdef EE_EDITOR_BUILD
#include <vector>
#include "DebugTypes.h"
#include <ElevateEngine/Renderer/VertexArray.h>
#include <ElevateEngine/Renderer/Shader/ShaderManager.h>
#endif

namespace Elevate
{
	struct DebugVertex
	{
#ifdef EE_EDITOR_BUILD
		glm::vec3 Position;
		glm::vec4 Color;
#endif
	};

	struct DebugLineData
	{
#ifdef EE_EDITOR_BUILD
		glm::vec3 Start;
		glm::vec3 End;
		glm::vec4 Color;
#endif
	};

#ifdef EE_EDITOR_BUILD
	class DebugRenderer
	{
	public:
		static void Init();
		static void Render();

		static void AddDebugLine(DebugLineData line);
		static void AddDebugCone(const glm::vec3& origin, const glm::vec3& direction, float radius, float range, uint16_t segmentCount, const glm::vec4& color);

	private:
		static DebugRenderer& Get()
		{
			static DebugRenderer Instance;
			return Instance;
		}

	private:
		void InternalInit();
		void InternalRender();

		// Line debugger
		void InitLineRender();
		void ClearDebugLines();

	private:
		std::shared_ptr<Shader> m_lineShader;
		std::shared_ptr<VertexArray> m_lineArray;
		std::shared_ptr<VertexBuffer> m_lineBuffer;
		std::vector<DebugVertex> m_debugLineArray;
	};
#else
	// Empty declaration when building the game
	class DebugRenderer
	{
	public:
		static void Init() { }
		static void Render() { }
		static void AddDebugLine(DebugLineData line) { }
	};
#endif
}