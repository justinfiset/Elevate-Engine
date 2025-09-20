#pragma once
#include <ElevateEngine/Editor/EditorWidget.h>
#include <ElevateEngine/Editor/Serialization/ComponentLayout.h>

namespace Elevate
{
	class Component;
}

namespace Elevate::Editor
{
	class AnalyserPanel : public EditorWidget
	{
	public:
		void OnImGuiRender() override;
	private:
		void RenderComponent(Component* component) const;
		void RenderComponentLayout(ComponentLayout& layout, Component* component = nullptr) const;
		void RenderField(const ComponentField& field) const;
	};
}