export module Elevate.Editor.Theme;

export namespace Elevate::Editor
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
