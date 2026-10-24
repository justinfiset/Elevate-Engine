#pragma once

#include <glm/vec2.hpp>

import Elevate.Input;

namespace Elevate {
	class GlfwInput : public Input
	{
	protected:
		//Mouse Impl
		virtual glm::vec2 GetMousePositionImpl() override;
	};
}

