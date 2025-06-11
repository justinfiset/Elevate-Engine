#pragma once
#include <ElevateEngine/Editor/EditorWidget.h>
#include <ElevateEngine/Editor/Serialization/ComponentLayout.h>

namespace Elevate::Editor
{
	class AnalyserPanel : public EditorWidget
	{
	public:
		void OnImGuiRender() override;
	private:
		void RenderComponentLayout(ComponentLayout& layout) const;
		void RenderField(const ComponentField& field) const;
	};
}