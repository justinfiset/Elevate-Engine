#include "BoxCollider.h"

#ifdef EE_EDITOR_BUILD
#include <ElevateEngine/Renderer/Debug/DebugRenderer.h>
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
