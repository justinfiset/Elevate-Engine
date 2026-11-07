module;

#include <glm/glm.hpp>
#include <ctype.h>

export module Elevate.Input;

import Elevate.Core.Events.KeyEvent;
import Elevate.Core.Events.MouseEvent;

import Elevate.Input.Buffer;

export import Elevate.Input.Keyboard;
export import Elevate.Input.Mouse;
export import Elevate.Input.Gamepad;
export import Elevate.Input.Joystick;

export namespace Elevate
{
	class Input
	{
	public:
		// Keyboard
		inline static bool IsKeyDown(KeyCode keycode) { return s_Instance->inputBuffer.IsKeyDown(keycode); }
		inline static bool IsKeyDown(char key) { return IsKeyDown((int)toupper(key)); }

		inline static bool IsKeyUp(KeyCode keycode) { return s_Instance->inputBuffer.IsKeyUp(keycode); }
		inline static bool IsKeyUp(char key) { return IsKeyUp((int)toupper(key)); }

		inline static bool IsKeyPressed(KeyCode keycode) { return s_Instance->inputBuffer.IsKeyPressed(keycode); }
		inline static bool IsKeyPressed(char key) { return IsKeyPressed((int)toupper(key)); }

		// Mouse
		inline static bool IsMouseButtonDown(MouseButton button) { return s_Instance->inputBuffer.IsMouseButtonDown(button); }
		inline static bool IsMouseButtonPressed(MouseButton button) { return s_Instance->inputBuffer.IsMouseButtonPressed(button); }
		inline static bool IsMouseButtonUp(MouseButton button) { return s_Instance->inputBuffer.IsMouseButtonUp(button); }

		inline static glm::vec2 GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return GetMousePosition().x; }
		inline static float GetMouseY() { return GetMousePosition().y; }

		// Mouse Impl
		virtual glm::vec2 GetMousePositionImpl() = 0;

		// Has to be called before polling events to reset all the intermediates states of the keys (down or up)
		inline static void ManageMidStates() { s_Instance->inputBuffer.ManageMidSates(); }
		inline static void ResetAllStates() { s_Instance->inputBuffer.ResetAllStates(); }

		/// Managed Callbacks // Could be used to simulate inputs to an app by sending events
		// Keyboard
		inline static void OnKeyPressedEvent(KeyPressedEvent& e)
		{
			s_Instance->inputBuffer.SetKeyPressed(e.GetKeyCode(), e.GetRepeatCount());
		}
		inline static void OnKeyReleasedEvent(KeyReleasedEvent& e)
		{
			s_Instance->inputBuffer.SetKeyReleased(e.GetKeyCode());
		}
		// Mouse
		inline static void OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
		{
			s_Instance->inputBuffer.SetMouseButtonPressed(e.GetMouseButton());
		}
		inline static void OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
		{
			s_Instance->inputBuffer.SetMouseButtonReleased(e.GetMouseButton());
		}

	private:
		Internal::InputBuffer inputBuffer;
		static Input* s_Instance;
	};
}
