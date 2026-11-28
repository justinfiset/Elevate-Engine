module;

#include <memory>

export module Elevate.Editor.Selection;

import Elevate.Core.Objects.Object;
import Elevate.Core.Objects.ObjectPtr;
import Elevate.Core.Scene;

export namespace Elevate::Editor
{
    class Selection
    {
    public:
        static bool IsSelected(const GameObject *gameObject);
        static std::weak_ptr<EEObject> GetSelectedObject();
    };
}