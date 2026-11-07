export module Elevate.Editor.Widgets.Widget;

export namespace Elevate::Editor
{
	class Widget
	{
	public:
		Widget() = default;
		virtual ~Widget() = default;
		virtual void OnImGuiRender() {};
		virtual void OnUpdate() {}
	};
}