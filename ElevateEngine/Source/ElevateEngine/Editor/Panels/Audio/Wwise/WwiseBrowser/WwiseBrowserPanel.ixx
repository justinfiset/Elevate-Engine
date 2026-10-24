module;

#ifdef EE_EDITOR_BUILD

#include <set>
#include <glm/vec4.hpp>
#include "imgui.h"

#include <ElevateEngine/Audio/Wwise/WwiseItem.h>

#endif // EE_EDITOR_BUILD

export module Elevate.Editor.Panels.Audio.WwiseBrowser;

#ifdef EE_EDITOR_BUILD

// Audio
import Elevate.Audio.Wwise.SoundEngine;
import Elevate.Audio.Wwise.WAAPI.Client;
// Editor
import Elevate.Editor.Widgets.Widget;
import Elevate.Editor.Message;

export namespace Elevate::Editor
{
#ifdef EE_USES_WWISE
	class WwiseBrowserPanel : public Widget
	{
	private:
		char m_searchBuffer[128];
		std::set<WwiseItemPtr> m_selectedItems;

	public:
		WwiseBrowserPanel() = default;
		virtual ~WwiseBrowserPanel() = default;

		virtual void OnImGuiRender() override;

	protected:
		void DrawTreeView(WwiseItemPtr root);
		void DrawItemsRecursive(WwiseItemPtr item);

		EditorMessage GetDiskStatusText(WwiseItemPtr item);
		EditorMessage GetSoundBanksStatusText(WwiseItemPtr item);
	};
#endif // #ifdef EE_USES_WWISE
}

#endif // EE_EDITOR_BUILD