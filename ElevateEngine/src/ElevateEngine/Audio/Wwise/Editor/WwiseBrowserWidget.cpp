#include "WwiseBrowserWidget.h"

#include <ElevateEngine/Audio/Wwise/WwiseSoundEngine.h>
#include "imgui.h"

void Elevate::Editor::WwiseBrowserWidget::OnImGuiRender()
{
	ImGui::Begin("Wwise Browser");

	WwiseSoundEngine* wwiseEngine = static_cast<WwiseSoundEngine*>(SoundEngine::GetImpl());
	DrawTreeView(wwiseEngine->GetFileDataSource()->GetItems().lock());

	ImGui::End();
}

void Elevate::Editor::WwiseBrowserWidget::DrawTreeView(WwiseItemPtr root)
{
	for (const auto& child : root->Children)
	{
		DrawItemsRecursive(child);
	}
}

void Elevate::Editor::WwiseBrowserWidget::DrawItemsRecursive(WwiseItemPtr item)
{
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
    if (!item->HasChildren())
    {
        nodeFlags |= ImGuiTreeNodeFlags_Leaf;
    }
    // todo impl.
    //if (object == EditorLayer::Get().GetSelectedObject().lock())
    //{
    //    nodeFlags |= ImGuiTreeNodeFlags_Selected;
    //}

    bool open = false;
    // todo impl.
    //ImGui::Image((ImTextureID)m_objectTexture->GetNativeHandle(), ImVec2(16, 16));
    //ImGui::SameLine();
    open = ImGui::TreeNodeEx((item->Name + "##" + item->GUID).c_str(), nodeFlags);

    if (ImGui::BeginPopupContextItem())
    {
        // todo impl.
        //if (ImGui::MenuItem("Delete")) { object->Destroy(); }
        //if (ImGui::MenuItem("Rename"))
        //{
        //    m_renaming = true;
        //    m_renamedObject = object;
        //    std::strncpy(m_renameBuffer, object->GetName().c_str(), sizeof(m_renameBuffer) - 1);
        //    m_renameBuffer[sizeof(m_renameBuffer) - 1] = '\0';
        //}
        ImGui::EndPopup();
    }

    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
    {
        if (item->HasValidShortID())
        {
            SoundEngine::PostEvent(item->ShortID);
        }
        //EditorLayer::Get().SelectObject(object);         // todo impl.
    }

    if (open)
    {
        for (const auto& child : item->Children)
        {
            DrawItemsRecursive(child);
        }
        ImGui::TreePop();
    }
}