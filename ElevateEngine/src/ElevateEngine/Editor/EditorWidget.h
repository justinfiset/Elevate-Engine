#pragma once

namespace Elevate::Editor
{
	class EditorWidget
	{
	public:
		EditorWidget() = default;
		virtual ~EditorWidget() = default;
		virtual void OnImGuiRender() = 0;
	};
}