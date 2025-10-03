#pragma once

#include <vector>

#include "DebugTypes.h"

namespace Elevate
{

	class DebugRenderer
	{
	public:
		void AddDebugLine(DebugLine line);

	private:
		void InitLineRender();

		std::vector<glm::vec3> m_debugLinePoints;
		std::vector<glm::vec3> m_debugLineColors;
	};
}