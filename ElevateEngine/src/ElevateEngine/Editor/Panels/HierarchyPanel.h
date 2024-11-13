#pragma once
#include <ElevateEngine/Core/GameObject.h>

namespace Elevate::Editor {
    class HierarchyPanel
    {
    public:
        HierarchyPanel() = default;

	    void OnImGuiRender();

    private:
        void DrawTreeHierarchy(GameObjectPtr object);
    };
}