#pragma once

#include "ElevateEngine/Core.h"
#include "ElevateEngine/Events/KeyEvent.h"
#include "ElevateEngine/Inputs/KeyBuffer.h"

#define KEYBOARD_SIZE   512;
#define MOUSE_SIZE      16;

// In order of execution when a key is pressed
#define EE_KEY_RELEASED 0;
#define EE_KEY_DOWN     1; // The instant moment when a key is pressed
#define EE_KEY_PRESSED  2;
#define EE_KEY_UP       3; // The instant moment when a key is released

namespace Hammer {
	class EE_API Input
	{
		friend class Application;
	public:
		// Keyboard
		inline static bool IsKeyDown(int keycode) { return s_Instance->keyBuffer.IsKeyDown(keycode); }
		inline static bool IsKeyUp(int keycode) { return s_Instance->keyBuffer.IsKeyUp(keycode); }
		inline static bool IsKeyPressed(int keycode) { return s_Instance->keyBuffer.IsKeyPressed(keycode); }	

		// Mouse
		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }

		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); } // Implemented by the platform
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

	protected:
		// Mouse Impl
		virtual bool IsMouseButtonPressedImpl (int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

		// Has to be called before polling events to reset all the intermediates states of the keys (down or up)
		inline static void ManageMidKeyStates() { s_Instance->keyBuffer.ManageMidSates(); }

		// Managed Callbacks // Could be used to simulate inputs to an app by sending events
		inline static void OnKeyPressedEvent(KeyPressedEvent& e) 
		{
			s_Instance->keyBuffer.SetKeyPress(e.GetKeyCode(), e.GetRepeatCount());
		}
		inline static void OnKeyReleasedEvent(KeyReleasedEvent& e) 
		{
			s_Instance->keyBuffer.SetKeyUp(e.GetKeyCode());
		}

	private:
		KeyBuffer keyBuffer;
		static Input* s_Instance;
	};
}