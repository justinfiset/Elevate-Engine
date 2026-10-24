module Elevate.Editor.Selection;

import Elevate.Editor.Layer;

namespace Elevate::Editor
{
	bool Selection::IsSelected(const GameObject* gameObject)
	{
		return EditorLayer::Get().GetSelectedObject().lock().get() == gameObject;
	}
}
