module;

#include <functional>

export module Elevate.Editor.Widgets.CollapsingHeader;

export namespace Elevate::Editor::Widgets
{
	bool CollapsingHeader(
		const char* label,
		bool canOpen = true,
		const void* icon = nullptr,
		std::function<float()> headerCustomContent = nullptr);
}
