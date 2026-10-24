export module Elevate.Input.Gamepad.Axis;

export namespace Elevate
{
	enum class GamepadAxis : int
	{
		LeftX = 0,
		LeftY = 1,
		RightX = 2,
		RightY = 3,
		LeftTrigger = 4,
		RightTrigger = 5,

		Last = RightTrigger
	};
}
