export module Elevate.Input.Gamepad.Hat;

export namespace Elevate
{
	enum class GamepadHat : int
	{
		Centered = 0,
		Up = 1,
		Right = 2,
		Down = 4,
		Left = 8,

		RightUp = Right | Up,
		RightDown = Right | Down,
		LeftUp = Left | Up,
		LeftDown = Left | Down
	};

	constexpr GamepadHat operator|(GamepadHat lhs, GamepadHat rhs)
	{
		return static_cast<GamepadHat>(
			static_cast<int>(lhs) | static_cast<int>(rhs)
			);
	}
}
