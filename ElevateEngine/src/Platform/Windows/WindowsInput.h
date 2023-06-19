#pragma once

#include "ElevateEngine/Inputs/Input.h"
#include "ElevateEngine/Inputs/InputBuffer.h"

namespace Hammer {
	class WindowsInput : public Input
	{
	protected:
		//Mouse Impl
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}

