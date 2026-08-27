#pragma once
#include <ElevateEngine/Renderer/Texture/TextureManager.h>
#include <glm/glm.hpp>

namespace Elevate
{
    class EditorRenderer
    {
    public:
        static void DrawBillboard(const glm::vec3& position, TexturePtr icon, float scale = 0.5f);
    };
}