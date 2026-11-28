module;

#include <glm/glm.hpp>

module Elevate.Physics.BoxCollider;

#ifdef EE_EDITOR_BUILD
import Elevate.Core.Scene;
import Elevate.Renderer.Debug.DebugRenderer;
#endif

namespace Elevate
{
#ifdef EE_EDITOR_BUILD
	void BoxCollider::RenderWhenSelected()
	{
		const auto& transform = gameObject->GetTransform();
		DebugRenderer::AddDebugCube(GetWorldCenter(), m_Shape.GetHalfExtents(), s_OutlineColor, transform.GetUp(), transform.GetLeft());
	}
#endif
}
