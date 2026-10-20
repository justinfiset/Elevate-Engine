#pragma once

#ifdef EE_EDITOR_BUILD

#include <memory>

#include <ElevateEngine/Editor/EditorWidget.h>

import Elevate.Core.Objects.ObjectPtr;
import Elevate.Core.Scene:GameObject;
import Elevate.Renderer.Textures.Texture;

namespace Elevate::Editor
{
	class HierarchyPanel : public EditorWidget
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