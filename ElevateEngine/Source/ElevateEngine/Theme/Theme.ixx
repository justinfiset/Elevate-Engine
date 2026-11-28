module;

export module Elevate.Theme;

export namespace Elevate
{
	enum class Theme
	{
		Dark = 0,
		Light = 1
	};

	Theme GetCurrentTheme();

	void ApplyFont();
	void SetDarkTheme();
	void SetLightTheme();
}
