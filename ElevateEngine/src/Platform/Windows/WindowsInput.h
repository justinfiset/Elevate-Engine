#pragma once

#include "ElevateEngine/Inputs/Input.h"
#include "ElevateEngine/Inputs/InputBuffer.h"

namespace Elevate {
	class WindowsInput : public Input
	{
	protected:
		//Mouse Impl
		virtual glm::vec2 GetMousePositionImpl() override;
	};
}

