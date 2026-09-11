namespace Elevate
{
	namespace UI
	{
		enum class UITheme 
		{
			Dark = 0, 
			Light = 1
		};

		UITheme GetCurrentTheme();

		void SetDarkTheme();
		void SetLightTheme();
	}
}