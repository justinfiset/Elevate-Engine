module;

#include <ElevateEngine/Core/Scene/SceneEnTTOperators.h>

module Elevate.Editor.Selection;

#ifdef EE_EDITOR_BUILD

import Elevate.Editor.Layer;

namespace Elevate::Editor
{
	bool Selection::IsSelected(const GameObject* gameObject)
	{
		return EditorLayer::Get().GetSelectedObject().lock().get() == gameObject;
	}

	std::weak_ptr<EEObject> Selection::GetSelectedObject()
	{
		return EditorLayer::Get().GetSelectedObject();
	}
}

#endif
