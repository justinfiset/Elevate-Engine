#pragma once

#include <bitset>

namespace Elevate
{
	class InputBuffer
	{
	private:
		std::bitset<348 * 2> keyStates; // 0 to 3 -> released, down, pressed, up (2 bits per key)
		std::bitset<8 * 2> mouseButtonStates;

	public:
		///Getter
		//Keyboard
		bool IsKeyPressed(int keycode);
		bool IsKeyDown(int keycode);
		bool IsKeyUp(int keycode);
		//Mouse
		bool IsMouseButtonPressed(int mouseButtonCode);
		bool IsMouseButtonDown(int mouseButtonCode);
		bool IsMouseButtonUp(int mouseButtonCode);

		///Setter
		//Keyboard
		void SetKeyPressed(int keycode, int repeatCount);
		void SetKeyReleased(int keycode);
		//Mouse
		void SetMouseButtonPressed(int mouseButtonCode);
		void SetMouseButtonReleased(int mouseButtonCode);

		void ManageMidSates();

	private:
		// Bit packing helper
		std::uint8_t GetKeyState(size_t index) const;
		void SetKeyState(size_t index, std::uint8_t value);
		std::uint8_t GetMouseButtonState(size_t index) const;
		void SetMouseButtonState(size_t index, std::uint8_t value);
	};
}

