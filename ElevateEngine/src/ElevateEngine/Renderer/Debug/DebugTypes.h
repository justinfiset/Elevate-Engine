#pragma once

#if defined(EE_EDITOR_BUILD) || defined(EE_DEBUG)
#include <glm/glm.hpp>

namespace Elevate
{
    struct DebugPoint
    {
        glm::vec3 pos;
    };

    struct DebugLine
    {
        glm::vec3 start;
        glm::vec3 end;
        glm::vec4 color;
        float width;
        float lifetime; // duration in seconds, 0 if permanent
    };
}

#endif // EE_EDITOR_BUILD || EE_DEBUG