#pragma once
#include <ElevateEngine/Core/GameObject.h>
#include <ElevateEngine/Editor/EditorWidget.h>

namespace Elevate::Editor {
    class HierarchyPanel : public EditorWidget
    {
    public:
        void OnImGuiRender() override;
    private:
        void DrawTreeHierarchy(GameObjectPtr object);
        void EndRename(GameObjectPtr object);
    private:
        bool m_renaming = false;
        GameObjectPtr m_renamedObject = nullptr;
        char m_renameBuffer[256] = { 0 };
    };
}