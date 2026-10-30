module;

#include <cstdint>
#include <cstddef>

module Elevate.Input.Buffer;

import Elevate.Foundations.Assert;

namespace Elevate::Internal
{
	/// Getter
	// Keyboard
	bool InputBuffer::IsKeyPressed(KeyCode keycode)
	{
		return GetKeyState(keycode) == InputState::Pressed;
	}
	bool InputBuffer::IsKeyDown(KeyCode keycode)
	{
		return GetKeyState(keycode) == InputState::Down;
	}
	bool InputBuffer::IsKeyUp(KeyCode keycode)
	{
		return GetKeyState(keycode) == InputState::Up;
	}
	// Mouse
	bool InputBuffer::IsMouseButtonPressed(MouseButton mouseButtonCode)
	{
		return GetMouseButtonState(mouseButtonCode) == InputState::Pressed;
	}
	bool InputBuffer::IsMouseButtonDown(MouseButton mouseButtonCode)
	{
		return GetMouseButtonState(mouseButtonCode) == InputState::Down;
	}
	bool InputBuffer::IsMouseButtonUp(MouseButton mouseButtonCode)
	{
		return GetMouseButtonState(mouseButtonCode) == InputState::Up;
	}

	/// Internal - State Machine
	// Keyboard
	void InputBuffer::SetKeyPressed(KeyCode keycode, int repeatCount)
	{
		// if key is not down and not repeating -> down
		if (GetKeyState(keycode) != InputState::Down && repeatCount == 0)
		{
			SetKeyState(keycode, InputState::Down);
		}
		else // else -> pressed
		{
			SetKeyState(keycode, InputState::Pressed);
		}
	}
	void InputBuffer::SetKeyReleased(KeyCode keycode)
	{
		// if key is down or pressed -> up
		if (GetKeyState(keycode) == InputState::Down || GetKeyState(keycode) == InputState::Pressed)
		{
			SetKeyState(keycode, InputState::Up);
		}
	}

	// Mousse
	void InputBuffer::SetMouseButtonPressed(MouseButton mouseButtonCode)
	{
		// if key is not down
		if (GetMouseButtonState(mouseButtonCode) != InputState::Down)
		{
			SetMouseButtonState(mouseButtonCode, InputState::Down);
		}
		else // else -> pressed
		{
			SetMouseButtonState(mouseButtonCode, InputState::Pressed);
		}
	}
	void InputBuffer::SetMouseButtonReleased(MouseButton mouseButtonCode)
	{
		// if key is down or pressed -> up
		if (GetMouseButtonState(mouseButtonCode) == InputState::Down || GetMouseButtonState(mouseButtonCode) == InputState::Pressed)
		{
			SetMouseButtonState(mouseButtonCode, InputState::Up);
		}
	}

	void InputBuffer::ResetAllStates()
	{
		keyStates.reset();
		mouseButtonStates.reset();
	}

	void InputBuffer::ManageMidSates()
	{
		// Keyboard
		for (std::size_t i = 0; i < keyStates.size() / 2; i++)
		{
			auto state = GetKeyState(i);
			switch (state)
			{
			case InputState::Down:
				SetKeyState(i, InputState::Pressed);
				break;
			case InputState::Up:
				SetKeyState(i, InputState::Released);
				break;
			default:
				break;
			}
		}

		// Mouse
		for (std::size_t i = 0; i < mouseButtonStates.size() / 2; i++)
		{
			auto state = GetMouseButtonState(i);
			switch (state)
			{
			case InputState::Down:
				SetMouseButtonState(i, InputState::Pressed);
				break;
			case InputState::Up:
				SetMouseButtonState(i, InputState::Released);
				break;
			default:
				break;
			}
		}
	}

#pragma region Bitset Operation Helper

	InputState InputBuffer::GetKeyState(KeyCode keyCode) const
	{
		return GetKeyState(static_cast<std::size_t>(keyCode));
	}

	void InputBuffer::SetKeyState(KeyCode keyCode, InputState value)
	{
		SetKeyState(static_cast<std::size_t>(keyCode), value);
	}

	InputState InputBuffer::GetKeyState(std::size_t index) const
	{
		std::size_t bitPos = index * 2;
		Assert::That(bitPos + 1 < keyStates.size(), "InputBuffer::GetKeyState() : Invalid key index.");

		return static_cast<InputState>((keyStates[bitPos] ? 2 : 0) | (keyStates[bitPos + 1] ? 1 : 0));
	}

	void InputBuffer::SetKeyState(std::size_t index, InputState value)
	{
		uint8_t intValue = (uint8_t)value;
		std::size_t bitPos = index * 2;
		Assert::That(index >= 0 && bitPos < keyStates.size(), "InputBuffer::SetKeyState() : Invalid key index.");

		keyStates[bitPos] = (intValue & 0x02) != 0;
		keyStates[bitPos + 1] = (intValue & 0x01) != 0;
	}

	InputState InputBuffer::GetMouseButtonState(MouseButton button) const
	{
		return GetMouseButtonState(static_cast<std::size_t>(button));
	}

	void InputBuffer::SetMouseButtonState(MouseButton button, InputState value)
	{
		SetMouseButtonState(static_cast<std::size_t>(button), value);
	}

	InputState InputBuffer::GetMouseButtonState(std::size_t index) const
	{
		std::size_t bitPos = index * 2;
		Assert::That(bitPos + 1 < mouseButtonStates.size(), "InputBuffer::GetMouseButtonState() : Invalid button index.");

		return static_cast<InputState>((mouseButtonStates[bitPos] ? 2 : 0) | (mouseButtonStates[bitPos + 1] ? 1 : 0));
	}

	void InputBuffer::SetMouseButtonState(std::size_t index, InputState value)
	{
		uint8_t intValue = (uint8_t)value;
		std::size_t bitPos = index * 2;
		Assert::That(index >= 0 && bitPos < keyStates.size(), "InputBuffer::SetMouseButtonState() : Invalid buton index.");

		mouseButtonStates[bitPos] = (intValue & 0x02) != 0;
		mouseButtonStates[bitPos + 1] = (intValue & 0x01) != 0;
	}

#pragma endregion
}