module;

#include <memory>

export module Elevate.Editor.Panels.HierarchyPanel;

#ifdef EE_EDITOR_BUILD

// Core
import Elevate.Core.Objects.ObjectPtr;
import Elevate.Core.Scene;
// Renderer
import Elevate.Renderer.Texture;
// Editor
import Elevate.Editor.Widgets.Widget;

export namespace Elevate::Editor
{
	class HierarchyPanel : public Widget
	{
	public:
		HierarchyPanel();

		void OnImGuiRender() override;
	private:
		void DrawTreeHierarchy(const EEObjectPtr<GameObject>& object);
		void EndRename(const EEObjectPtr<GameObject>& object);
	private:
		std::shared_ptr<Texture> m_sceneTexture;
		std::shared_ptr<Texture> m_objectTexture;

		bool m_renaming = false;
		EEObjectPtr<GameObject> m_renamedObject = nullptr;
		char m_renameBuffer[256] = { 0 };
	};
}

#endif // EE_EDITOR_BUILD