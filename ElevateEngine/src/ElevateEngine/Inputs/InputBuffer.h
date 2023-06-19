#pragma once

namespace Hammer
{
	class InputBuffer
	{
	private:
		short keyStates[348]; // 0 to 3 -> released, down, pressed, up
		short mouseButtonStates[8];
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
	};
}

