#pragma once

#include "RedHammer/Input.h"

namespace Hammer {
	class WindowsInput : public Input
	{
	protected:
		// Keyboard Impl
		virtual bool IsKeyPressedImpl(int keycode) override;

		//Mouse Impl

		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}

