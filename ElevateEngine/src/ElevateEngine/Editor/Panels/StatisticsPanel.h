#pragma once

#include <ElevateEngine/Editor/EditorWidget.h>

namespace Elevate::Editor 
{
	class StatisticsPanel : public EditorWidget
	{
	public:
		void OnImGuiRender() override;
	};
}