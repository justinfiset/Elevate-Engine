module Elevate.Editor.Selection;

#ifdef EE_EDITOR_BUILD

import Elevate.Editor.Layer;

namespace Elevate::Editor
{
	bool Selection::IsSelected(const GameObject* gameObject)
	{
		return EditorLayer::Get().GetSelectedObject().lock().get() == gameObject;
	}
}

#endif
