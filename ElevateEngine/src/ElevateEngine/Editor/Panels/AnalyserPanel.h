#pragma once
#include <ElevateEngine/Editor/EditorWidget.h>

namespace Elevate::Editor
{
	class AnalyserPanel : public EditorWidget
	{
	public:
		void OnImGuiRender() override;
	};
}