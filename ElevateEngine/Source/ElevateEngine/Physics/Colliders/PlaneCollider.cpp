#include "PlaneCollider.h"

#ifdef EE_EDITOR_BUILD
#include <ElevateEngine/Renderer/Debug/DebugRenderer.h>
#endif

namespace Elevate
{
#ifdef EE_EDITOR_BUILD
	void PlaneCollider::RenderWhenSelected()
	{
		glm::vec3 front = gameObject->GetTransform().GetForward() * m_Shape.GetSize().y;
		glm::vec3 left = gameObject->GetTransform().GetLeft() * m_Shape.GetSize().x;
		glm::vec3 origin = GetWorldCenter();

		glm::vec3 upperLeft = origin + left + front;
		glm::vec3 lowerLeft = origin + left - front;
		glm::vec3 upperRight = origin - left + front;
		glm::vec3 lowerRight = origin - left - front;
		
		DebugRenderer::AddDebugLine({ upperLeft, upperRight, s_OutlineColor });
		DebugRenderer::AddDebugLine({ upperRight, lowerRight, s_OutlineColor });
		DebugRenderer::AddDebugLine({ lowerRight, lowerLeft, s_OutlineColor });
		DebugRenderer::AddDebugLine({ lowerLeft, upperLeft, s_OutlineColor });
	}
#endif
}
