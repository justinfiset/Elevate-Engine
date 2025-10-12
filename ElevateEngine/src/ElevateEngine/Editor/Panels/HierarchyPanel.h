#pragma once
#include <ElevateEngine/Core/GameObject.h>
#include <ElevateEngine/Editor/EditorWidget.h>

namespace Elevate
{
    class Texture;
}
namespace Elevate::Editor
{
    class HierarchyPanel : public EditorWidget
    {
    public:
        HierarchyPanel();

        void OnImGuiRender() override;
    private:
        void DrawTreeHierarchy(std::shared_ptr<GameObject> object);
        void EndRename(std::shared_ptr<GameObject> object);
    private:
        std::shared_ptr<Texture> m_sceneTexture;
        std::shared_ptr<Texture> m_objectTexture;

        bool m_renaming = false;
        std::shared_ptr<GameObject> m_renamedObject = nullptr;
        char m_renameBuffer[256] = { 0 };
    };
}