export module Elevate.Input.Mouse.Button;

export namespace Elevate
{
	enum class MouseButton : int
	{
		Button1 = 0,
		Button2 = 1,
		Button3 = 2,
		Button4 = 3,
		Button5 = 4,
		Button6 = 5,
		Button7 = 6,
		Button8 = 7,

		Last = Button8,

		// Common aliases
		Left = Button1,
		Right = Button2,
		Middle = Button3
	};
}
