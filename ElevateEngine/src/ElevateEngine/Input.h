#pragma once

#include "ElevateEngine/Core.h"

namespace Hammer {
	class EE_API Input
	{
	public:
		// Keyboard
		inline static bool IsKeyDown(int keycode) { return s_Instance->IsKeyDownImpl(keycode); }
		inline static bool IsKeyUp(int keycode) { return s_Instance->IsKeyUpImpl(keycode); }
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }	

		// Mouse
		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

	protected:
		// Keybaord Impl
		virtual bool IsKeyDownImpl(int keycode) = 0;
		virtual bool IsKeyUpImpl(int keycode) = 0;
		virtual bool IsKeyPressedImpl(int keycode) = 0;

		// Mouse Impl
		virtual bool IsMouseButtonPressedImpl (int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Input* s_Instance;
	};
}