#pragma once

#include <glm/vec2.hpp>

#ifndef ELEVATE_INPUT_IMPLEMENTATION
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

