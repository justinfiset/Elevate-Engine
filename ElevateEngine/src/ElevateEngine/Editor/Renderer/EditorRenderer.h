#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <ElevateEngine/Renderer/Texture/Texture.h>

namespace Elevate
{
    class EditorRenderer
    {
    public:
        static void DrawBillboard(const glm::vec3& position, TexturePtr icon, float scale = 0.5f, glm::vec4 colorModifier = glm::vec4(1.0f));
    };
}