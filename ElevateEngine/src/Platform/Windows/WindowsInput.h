#pragma once

#include "ElevateEngine/Input.h"
#include "ElevateEngine/Inputs/KeyBuffer.h"

namespace Hammer {
	class WindowsInput : public Input
	{
	protected:
		//Mouse Impl
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}

