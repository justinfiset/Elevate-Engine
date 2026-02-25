#pragma once
#ifdef EE_EDITOR_BUILD

#include <ElevateEngine/Editor/EditorWidget.h>
#include <ElevateEngine/Audio/Wwise/WwiseItem.h>

#include <set>
#include <glm/vec4.hpp>

namespace Elevate::Editor
{
	// Todo move this somewhere else, can be very usefull for UI messages
	struct EditorMessage
	{
		enum Type : uint8_t
		{
			Error,
			Warning,
			Message,
			Success,
			Information
		};

		std::string message;
		Type type;

		EditorMessage(std::string message, Type type)
			: message(message), type(type) { }

		inline glm::vec4 GetColor()
		{
			// todo remove the hardcoded colors from here
			switch (type)
			{
			case Error:
				return { 166, 25, 46, 255 };
			case Warning:
				return { 255, 153, 0, 255 };
			case Message:
				return { 255, 255, 255, 255 };
			case Success:
				return { 4, 179, 9, 255 };
			case Information:
				return { 0, 87, 184, 255 };
			}
		}
	};

	class WwiseBrowserWidget : public EditorWidget
	{
	public:
		WwiseBrowserWidget() = default;
		virtual ~WwiseBrowserWidget() = default;
		virtual void OnImGuiRender() override;

	protected:
		void DrawTreeView(WwiseItemPtr root);
		void DrawItemsRecursive(WwiseItemPtr item);

		EditorMessage GetDiskStatusText(WwiseItemPtr item);
		EditorMessage GetSoundBanksStatusText(WwiseItemPtr item);

	private:
		char m_searchBuffer[128];

		std::set<WwiseItemPtr> m_selectedItems;
	};
}

#endif // EE_EDITOR_BUILD