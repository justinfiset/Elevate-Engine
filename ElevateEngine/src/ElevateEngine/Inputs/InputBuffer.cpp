#include "eepch.h"

#include "ElevateEngine/Inputs/Input.h"
#include "ElevateEngine/Core/Log.h"
#include "InputBuffer.h"

namespace Elevate
{
	/// Getter
	// Keyboard
	bool InputBuffer::IsKeyPressed(int keycode)
	{
		return keyStates[keycode] == EE_STATE_PRESSED;
	}
	bool InputBuffer::IsKeyDown(int keycode)
	{
		return keyStates[keycode] == EE_STATE_DOWN;
	}
	bool InputBuffer::IsKeyUp(int keycode)
	{
		return keyStates[keycode] == EE_STATE_UP;
	}
	// Mouse
	bool InputBuffer::IsMouseButtonPressed(int mouseButtonCode)
	{
		return mouseButtonStates[mouseButtonCode] == EE_STATE_PRESSED;
	}
	bool InputBuffer::IsMouseButtonDown(int mouseButtonCode)
	{
		return mouseButtonStates[mouseButtonCode] == EE_STATE_DOWN;
	}
	bool InputBuffer::IsMouseButtonUp(int mouseButtonCode)
	{
		return mouseButtonStates[mouseButtonCode] == EE_STATE_UP;
	}
	// Mouse


	/// Internal - State Machine
	// Keyboard
	void InputBuffer::SetKeyPressed(int keycode, int repeatCount)
	{
		// if key is not down and not repeating -> down
		if (keyStates[keycode] != 1 && repeatCount == 0)
		{
			keyStates[keycode] = EE_STATE_DOWN;
		}
		else // else -> pressed
		{
			keyStates[keycode] = EE_STATE_PRESSED;
		}
	}
	void InputBuffer::SetKeyReleased(int keycode)
	{
		// if key is down or pressed -> up
		if (keyStates[keycode] == 1 || keyStates[keycode] == 2)
		{
			keyStates[keycode] = EE_STATE_UP;
		}
	}

	// Mousse
	void InputBuffer::SetMouseButtonPressed(int mouseButtonCode)
	{
		// if key is not down
		if (keyStates[mouseButtonCode] != 1)
		{
			mouseButtonStates[mouseButtonCode] = EE_STATE_DOWN;
		}
		else // else -> pressed
		{
			mouseButtonStates[mouseButtonCode] = EE_STATE_PRESSED;
		}
	}
	void InputBuffer::SetMouseButtonReleased(int mouseButtonCode)
	{
		// if key is down or pressed -> up
		if (mouseButtonStates[mouseButtonCode] == 1 || mouseButtonStates[mouseButtonCode] == 2)
		{
			mouseButtonStates[mouseButtonCode] = EE_STATE_UP;
		}
	}

	void InputBuffer::ManageMidSates()
	{
		// Keyboard
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
		// Mouse
		for (int i = 0; i < sizeof(mouseButtonStates); i++)
		{
			if (mouseButtonStates[i] == 1)
			{
				mouseButtonStates[i] = 2;
				continue;
			}
			else if (mouseButtonStates[i] == 3)
			{
				mouseButtonStates[i] = 0;
			}
		}
	}
}