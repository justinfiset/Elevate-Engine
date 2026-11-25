module;

#include <bitset>
#include <cstddef>

export module Elevate.Input.Buffer;

import Elevate.Input.State;

import Elevate.Input.Keyboard.KeyCode;
import Elevate.Input.Mouse.Button;

export namespace Elevate::Internal
{
	class InputBuffer
	{
	private:
		std::bitset<349 * 2> keyStates; // 0 to 3 -> released, down, pressed, up (2 bits per key)
		std::bitset<8 * 2> mouseButtonStates;

	public:
		///Getter
		//Keyboard
		bool IsKeyPressed(KeyCode keycode);
		bool IsKeyDown(KeyCode keycode);
		bool IsKeyUp(KeyCode keycode);
		//Mouse
		bool IsMouseButtonPressed(MouseButton mouseButtonCode);
		bool IsMouseButtonDown(MouseButton mouseButtonCode);
		bool IsMouseButtonUp(MouseButton mouseButtonCode);

		///Setter
		//Keyboard
		void SetKeyPressed(KeyCode keycode, int repeatCount);
		void SetKeyReleased(KeyCode keycode);
		//Mouse
		void SetMouseButtonPressed(MouseButton mouseButtonCode);
		void SetMouseButtonReleased(MouseButton mouseButtonCode);
		void ResetAllStates();

		void ManageMidSates();

	private:
		// Bit packing helper
		InputState GetKeyState(KeyCode keyCode) const;
		void SetKeyState(KeyCode keyCode, InputState value);
		InputState GetKeyState(std::size_t index) const;
		void SetKeyState(std::size_t index, InputState value);

		InputState GetMouseButtonState(MouseButton button) const;
		void SetMouseButtonState(MouseButton button, InputState value);
		InputState GetMouseButtonState(std::size_t index) const;
		void SetMouseButtonState(std::size_t index, InputState value);
	};
}

