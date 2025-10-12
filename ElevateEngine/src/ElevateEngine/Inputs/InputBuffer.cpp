#include "eepch.h"

#include "ElevateEngine/Inputs/Input.h"
#include "ElevateEngine/Core/Assert.h"
#include "ElevateEngine/Core/Log.h"
#include "InputBuffer.h"

namespace Elevate
{
	/// Getter
	// Keyboard
	bool InputBuffer::IsKeyPressed(int keycode)
	{
		return GetKeyState(keycode) == EE_STATE_PRESSED;
	}
	bool InputBuffer::IsKeyDown(int keycode)
	{
		return GetKeyState(keycode) == EE_STATE_DOWN;
	}
	bool InputBuffer::IsKeyUp(int keycode)
	{
		return GetKeyState(keycode) == EE_STATE_UP;
	}
	// Mouse
	bool InputBuffer::IsMouseButtonPressed(int mouseButtonCode)
	{
		return GetMouseButtonState(mouseButtonCode) == EE_STATE_PRESSED;
	}
	bool InputBuffer::IsMouseButtonDown(int mouseButtonCode)
	{
		return GetMouseButtonState(mouseButtonCode) == EE_STATE_DOWN;
	}
	bool InputBuffer::IsMouseButtonUp(int mouseButtonCode)
	{
		return GetMouseButtonState(mouseButtonCode) == EE_STATE_UP;
	}

	/// Internal - State Machine
	// Keyboard
	void InputBuffer::SetKeyPressed(int keycode, int repeatCount)
	{
		// if key is not down and not repeating -> down
		if (GetKeyState(keycode) != EE_STATE_DOWN && repeatCount == 0)
		{
			SetKeyState(keycode, EE_STATE_DOWN);
		}
		else // else -> pressed
		{
			SetKeyState(keycode, EE_STATE_PRESSED);
		}
	}
	void InputBuffer::SetKeyReleased(int keycode)
	{
		// if key is down or pressed -> up
		if (GetKeyState(keycode) == EE_STATE_DOWN || GetKeyState(keycode) == EE_STATE_PRESSED)
		{
			SetKeyState(keycode, EE_STATE_UP);
		}
	}

	// Mousse
	void InputBuffer::SetMouseButtonPressed(int mouseButtonCode)
	{
		// if key is not down
		if (GetMouseButtonState(mouseButtonCode) != EE_STATE_DOWN)
		{
			SetMouseButtonState(mouseButtonCode, EE_STATE_DOWN);
		}
		else // else -> pressed
		{
			SetMouseButtonState(mouseButtonCode, EE_STATE_PRESSED);
		}
	}
	void InputBuffer::SetMouseButtonReleased(int mouseButtonCode)
	{
		// if key is down or pressed -> up
		if (GetMouseButtonState(mouseButtonCode) == EE_STATE_DOWN || GetMouseButtonState(mouseButtonCode) == EE_STATE_PRESSED)
		{
			SetMouseButtonState(mouseButtonCode, EE_STATE_UP);
		}
	}

	void InputBuffer::ManageMidSates()
	{
		// Keyboard
		for (size_t i = 0; i < keyStates.size() / 2; i++)
		{
			uint8_t state = GetKeyState(i);
			switch (state)
			{
			case EE_STATE_DOWN:
				SetKeyState(i, EE_STATE_PRESSED);
				break;
			case EE_STATE_UP:
				SetKeyState(i, EE_STATE_RELEASED);
				break;
			}
		}

		// Mouse
		for (size_t i = 0; i < mouseButtonStates.size() / 2; i++)
		{
			uint8_t state = GetMouseButtonState(i);
			switch (state)
			{
			case EE_STATE_DOWN:
				SetMouseButtonState(i, EE_STATE_PRESSED);
				break;
			case EE_STATE_UP:
				SetMouseButtonState(i, EE_STATE_RELEASED);
				break;
			}
		}
	}

	// Bitset operation helper
	std::uint8_t InputBuffer::GetKeyState(size_t index) const
	{
		size_t bitPos = index * 2;
		EE_CORE_ASSERT(index >= 0 && bitPos < keyStates.size(), "InputBuffer::GetKeyState() : Invalid key index.");

		return (keyStates[bitPos] ? 2 : 0) | (keyStates[bitPos + 1] ? 1 : 0);
	}

	void InputBuffer::SetKeyState(size_t index, std::uint8_t value)
	{
		size_t bitPos = index * 2;
		EE_CORE_ASSERT(index >= 0 && bitPos < keyStates.size(), "InputBuffer::SetKeyState() : Invalid key index.");

		keyStates[bitPos] = (value & 0x02) != 0;
		keyStates[bitPos + 1] = (value & 0x01) != 0;
	}

	std::uint8_t InputBuffer::GetMouseButtonState(size_t index) const
	{
		size_t bitPos = index * 2;
		EE_CORE_ASSERT(index >= 0 && bitPos < keyStates.size(), "InputBuffer::GetMouseButtonState() : Invalid buton index.");

		return (mouseButtonStates[bitPos] ? 2 : 0) | (mouseButtonStates[bitPos + 1] ? 1 : 0);
	}
	void InputBuffer::SetMouseButtonState(size_t index, std::uint8_t value)
	{
		size_t bitPos = index * 2;
		EE_CORE_ASSERT(index >= 0 && bitPos < keyStates.size(), "InputBuffer::SetMouseButtonState() : Invalid buton index.");

		mouseButtonStates[bitPos] = (value & 0x02) != 0;
		mouseButtonStates[bitPos + 1] = (value & 0x01) != 0;
	}
}