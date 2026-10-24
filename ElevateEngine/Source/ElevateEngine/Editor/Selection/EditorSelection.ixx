export module Elevate.Editor.Selection;

import Elevate.Core.Scene;

export namespace Elevate::Editor
{
    class Selection
    {
    public:
        static bool IsSelected(const GameObject *gameObject);
    };
}