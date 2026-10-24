export module Elevate.Input.State;

export namespace Elevate
{
	enum class InputState
	{
		Released = 0,
		Down = 1, // The instant moment when a key is pressed
		Pressed = 2,
		Up = 3 // The instant moment when a key is released
	};
}
