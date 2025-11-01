#pragma once
#ifdef EE_EDITOR_BUILD

#include <ElevateEngine/Editor/EditorWidget.h>
#include <ElevateEngine/Audio/Wwise/WwiseItem.h>

namespace Elevate::Editor
{
	class WwiseBrowserWidget : public EditorWidget
	{
	public:
		WwiseBrowserWidget() = default;
		virtual ~WwiseBrowserWidget() = default;
		virtual void OnImGuiRender() override;

	protected:
		void DrawTreeView(WwiseItemPtr root);
		void DrawItemsRecursive(WwiseItemPtr item);
	};
}

#endif // EE_EDITOR_BUILD