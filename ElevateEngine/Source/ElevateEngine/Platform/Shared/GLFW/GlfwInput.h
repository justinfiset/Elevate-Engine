#pragma once

#ifndef ELEVATE_INPUT_IMPLEMENTATION
#include <glm/vec2.hpp>

import Elevate.Input;
#endif

namespace Elevate {
	class GlfwInput : public Input
	{
	protected:
		//Mouse Impl
		virtual glm::vec2 GetMousePositionImpl() override;
	};
}

