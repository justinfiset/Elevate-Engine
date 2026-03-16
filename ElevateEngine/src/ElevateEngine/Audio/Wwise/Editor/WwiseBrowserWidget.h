#pragma once
#ifdef EE_EDITOR_BUILD

#include <ElevateEngine/Editor/EditorWidget.h>
#include <ElevateEngine/Audio/Wwise/WwiseItem.h>
#include <ElevateEngine/Audio/Wwise/WwiseSoundEngine.h>
#include <ElevateEngine/Audio/Wwise/WAAPI/WAAPIClient.h>

#include <set>
#include <glm/vec4.hpp>
#include "imgui.h"

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
			return { 0, 87, 184, 255 };
		}
	};

#ifdef EE_USES_WWISE
	class WwiseBrowserWidget : public EditorWidget
	{
	private:
		char m_searchBuffer[128];
		std::set<WwiseItemPtr> m_selectedItems;

	public:
		WwiseBrowserWidget() = default;
		virtual ~WwiseBrowserWidget() = default;

		virtual void OnImGuiRender() override
		{
			// todo : prevent from being fetched each frame and only update when there is a refresh.
			WwiseSoundEngine* wwiseEngine = WwiseSoundEngine::Get();
			if (!wwiseEngine) // Protection in case the SoundEngine is misssconfigured
			{
				return;
			}

			ImGui::Begin("Wwise Browser");

			ImGui::BeginGroup();
			ImGui::Text(WAAPIClient::IsConnected() ? "Conntected to WAAPI." : "Not Connected to WAAPI");
			ImGui::EndGroup();

			ImGui::BeginGroup();
			if (ImGui::Button("Expand All"))
			{
			}
			ImGui::SameLine();
			if (ImGui::Button("Collapse All"))
			{
			}
			ImGui::SameLine();
			ImGui::InputText("##", m_searchBuffer, sizeof(m_searchBuffer) / sizeof(char));
			ImGui::SameLine();
			if (ImGui::Button("Refresh"))
			{
				wwiseEngine->GetDataSource()->RefreshSource();
				// todo add a notifier here
			}
			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImGui::Button("Generate SoundBanks"))
			{
			}
			ImGui::EndGroup();
			ImGui::Separator();
			if (ImGui::BeginTable("WwiseBrowserTreeView", 3, ImGuiTableFlags_Resizable))
			{
				ImGui::TableSetupColumn("Wwise Item");
				ImGui::TableSetupColumn("State on Disk");
				ImGui::TableSetupColumn("State in SoundBanks");
				ImGui::TableHeadersRow();

				DrawTreeView(wwiseEngine->GetDataSource()->GetItems().lock());

				ImGui::EndTable();
			}

			ImGui::End();
		}

	protected:
		void DrawTreeView(WwiseItemPtr root)
		{
			for (const auto& child : root->Children)
			{
				DrawItemsRecursive(child);
			}
		}

		void DrawItemsRecursive(WwiseItemPtr item)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
			if (!item->HasChildren())
			{
				nodeFlags |= ImGuiTreeNodeFlags_Leaf;
			}

			if (m_selectedItems.contains(item))
			{
				nodeFlags |= ImGuiTreeNodeFlags_Selected;
			}

			bool open = false;
			// todo impl.
			//ImGui::Image((ImTextureID)m_objectTexture->GetNativeHandle(), ImVec2(16, 16));
			//ImGui::SameLine();
			open = ImGui::TreeNodeEx((item->Name + "##" + item->GUID).c_str(), nodeFlags);

			if (ImGui::BeginPopupContextItem())
			{
				ImGui::SeparatorText("Playback");
				ImGui::BeginDisabled(item->ShortID == 0 || !item->IsEvent());
				if (ImGui::MenuItem("Play Event"))
				{
					SoundEngine::PostEvent(item->ShortID);
				}
				ImGui::EndDisabled();
				ImGui::EndPopup();
			}

			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				if (!ImGui::IsKeyDown(ImGuiKey::ImGuiKey_LeftCtrl))
				{
					m_selectedItems.clear();
				}
				m_selectedItems.insert(item);
			}

			if (open)
			{
				for (const auto& child : item->Children)
				{
					DrawItemsRecursive(child);
				}
				ImGui::TreePop();
			}

			// We do not draw in the other columns if the item is a parent
			if (!item->IsDirectory())
			{
				ImGui::TableSetColumnIndex(1);
				EditorMessage msg = GetDiskStatusText(item);
				glm::vec4 color = msg.GetColor();
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(color.r, color.g, color.b, color.a));
				ImGui::Text(msg.message.c_str());
				ImGui::PopStyleColor();

				ImGui::TableSetColumnIndex(2);
				msg = GetSoundBanksStatusText(item);
				color = msg.GetColor();
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(color.r, color.g, color.b, color.a));
				ImGui::Text(msg.message.c_str());
				ImGui::PopStyleColor();
			}
		}

		EditorMessage GetDiskStatusText(WwiseItemPtr item)
		{
			if (item->IsOnDisk)
			{
				return EditorMessage("On Disk", EditorMessage::Type::Message);
			}
			else
			{
				return EditorMessage("Not on Disk", EditorMessage::Type::Warning);
			}
		}

		EditorMessage GetSoundBanksStatusText(WwiseItemPtr item)
		{
			if (item->IsInBank)
			{
				return EditorMessage("SoundBank up to date", EditorMessage::Type::Message);
			}
			else
			{
				return EditorMessage("Missing in SoundBank", EditorMessage::Type::Warning);
			}
		}
	};
#endif // #ifdef EE_USES_WWISE
}

#endif // EE_EDITOR_BUILD