export module Elevate.Editor.Panels.StatisticsPanel;

import Elevate.Editor.Widgets.Widget;

export namespace Elevate::Editor
{
	class StatisticsPanel : public Widget
	{
	public:
		void OnImGuiRender() override;
	};
}