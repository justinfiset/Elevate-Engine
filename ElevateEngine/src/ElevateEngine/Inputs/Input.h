#pragma once

#include "ElevateEngine/Events/KeyEvent.h"
#include <ElevateEngine/Events/MouseEvent.h>

#include "ElevateEngine/Inputs/inputBuffer.h"

#include <glm/glm.hpp>

#define KEYBOARD_SIZE   512;
#define MOUSE_SIZE      16;

// In order of execution when a key / button is pressed
#define EE_STATE_RELEASED 0;
#define EE_STATE_DOWN     1; // The instant moment when a key is pressed
#define EE_STATE_PRESSED  2;
#define EE_STATE_UP       3; // The instant moment when a key is released

namespace Elevate {
	class EE_API Input
	{
		friend class Application;
	public:
		// Keyboard
		inline static bool IsKeyDown(int keycode) { return s_Instance->inputBuffer.IsKeyDown(keycode); }
		inline static bool IsKeyDown(char key) { return IsKeyDown((int) toupper(key)); }

		inline static bool IsKeyUp(int keycode) { return s_Instance->inputBuffer.IsKeyUp(keycode); }
		inline static bool IsKeyUp(char key) { return IsKeyUp((int)toupper(key)); }

		inline static bool IsKeyPressed(int keycode) { return s_Instance->inputBuffer.IsKeyPressed(keycode); }	
		inline static bool IsKeyPressed(char key) { return IsKeyPressed((int)toupper(key)); }
		// Mouse
		inline static bool IsMouseButtonDown(int button) { return s_Instance->inputBuffer.IsMouseButtonDown(button); }
		inline static bool IsMouseButtonPressed(int button) { return s_Instance->inputBuffer.IsMouseButtonPressed(button); }
		inline static bool IsMouseButtonUp(int button) { return s_Instance->inputBuffer.IsMouseButtonUp(button); }

		inline static glm::vec2 GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return GetMousePosition().x; }
		inline static float GetMouseY() { return GetMousePosition().y; }

	protected:
		// Mouse Impl
		virtual glm::vec2 GetMousePositionImpl() = 0;

		// Has to be called before polling events to reset all the intermediates states of the keys (down or up)
		inline static void ManageMidStates() { s_Instance->inputBuffer.ManageMidSates(); }
			
		/// Managed Callbacks // Could be used to simulate inputs to an app by sending events
		// Keyboard
		inline static void OnKeyPressedEvent(KeyPressedEvent& e) 
		{
			s_Instance->inputBuffer.SetKeyPressed(e.GetKeyCode(), e.GetRepeatCount());
		}
		inline static void OnKeyReleasedEvent(KeyReleasedEvent& e) 
		{
			s_Instance->inputBuffer.SetKeyReleased(e.GetKeyCode());
		}
		// Mouse
		inline static void OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
		{
			s_Instance->inputBuffer.SetMouseButtonPressed(e.GetMouseButton());
		}
		inline static void OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
		{
			s_Instance->inputBuffer.SetMouseButtonReleased(e.GetMouseButton());
		}

	private:
		InputBuffer inputBuffer;
		static Input* s_Instance;
	};
}