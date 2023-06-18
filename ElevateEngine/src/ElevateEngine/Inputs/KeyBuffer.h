#pragma once

namespace Hammer
{
	class KeyBuffer
	{
	private:
		short keyStates[512]; // 0 to 3 -> released, down, pressed, up
	public:
		//Getter
		bool IsKeyPressed(int keycode);
		bool IsKeyDown(int keycode);
		bool IsKeyUp(int keycode);

		// Setter
		void SetKeyPress(int keycode, int repeatCount);
		void SetKeyUp(int keycode);

		void ManageMidSates();
	};
}

