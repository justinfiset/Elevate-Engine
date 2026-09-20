#include "BoxCollider.h"

#ifdef EE_EDITOR_BUILD
#include <ElevateEngine/Renderer/Debug/DebugRenderer.h>
#endif

namespace Elevate
{
#ifdef EE_EDITOR_BUILD
	void BoxCollider::RenderWhenSelected()
	{
		DebugRenderer::AddDebugCube(GetWorldCenter(), m_Shape.GetHalfExtents(), s_OutlineColor);
	}
#endif
}
