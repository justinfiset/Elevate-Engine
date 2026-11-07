module;

#include <sstream>
#include <string>

#include "EventMacros.h"

export module Elevate.Core.Events.KeyEvent;

import Elevate.Core.Events.Event;
import Elevate.Input.Keyboard.KeyCode;

export namespace Elevate
{
	class KeyEvent : public Event
	{
	public:
		inline KeyCode GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(KeyCode keycode) : m_KeyCode(keycode) {}

		KeyCode m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount)
		{
		}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << static_cast<int32_t>(m_KeyCode) << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount = 0;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode keycode)
			: KeyEvent(keycode)
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << static_cast<int32_t>(m_KeyCode);
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)

	private:
		int m_RepeatCount = 0;
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(KeyCode keycode)
			: KeyEvent(keycode)
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << static_cast<int32_t>(m_KeyCode);
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)

	private:
		int m_RepeatCount = 0;
	};
}