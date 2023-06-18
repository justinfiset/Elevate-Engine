#include "eepch.h"

#include "ElevateEngine/Inputs/Input.h"
#include "ElevateEngine/Core/Log.h"
#include "KeyBuffer.h"

namespace Hammer
{
	bool KeyBuffer::IsKeyPressed(int keycode)
	{
		return keyStates[keycode] == EE_KEY_PRESSED;
	}
	bool KeyBuffer::IsKeyDown(int keycode)
	{
		return keyStates[keycode] == EE_KEY_DOWN;
	}
	bool KeyBuffer::IsKeyUp(int keycode)
	{
		return keyStates[keycode] == EE_KEY_UP;
	}

	// Internal - State Machine
	void KeyBuffer::SetKeyPress(int keycode, int repeatCount)
	{
		// if key is not down and not repeating -> down
		if (keyStates[keycode] != 1 && repeatCount == 0)
		{
			keyStates[keycode] = EE_KEY_DOWN;
		}
		else // else -> pressed
		{
			keyStates[keycode] = EE_KEY_PRESSED;
		}
	}

	void KeyBuffer::SetKeyUp(int keycode)
	{
		// if key is down or pressed -> up
		if (keyStates[keycode] == 1 || keyStates[keycode] == 2)
		{
			keyStates[keycode] = EE_KEY_UP;
		}
	}

	void KeyBuffer::ManageMidSates()
	{
		for (int i = 0; i < sizeof(keyStates); i++)
		{
			if (keyStates[i] == 1)
			{
				keyStates[i] = 2;
				continue;
			}
			else if (keyStates[i] == 3)
			{
				keyStates[i] = 0;
			}
		}
	}
}