#include "eepch.h"
#include "DirectionalLight.h"

#include <glm/gtc/constants.hpp>
#include "SpotLight.h"
#include <glm/geometric.hpp>

#include <ElevateEngine/Core/GameContext.h>
#include <ElevateEngine/Renderer/Light/SceneLighting.h>

#ifdef EE_EDITOR_BUILD
#include <ElevateEngine/Renderer/Debug/DebugRenderer.h>
#include <ElevateEngine/Editor/Renderer/EditorRenderer.h>
#endif

namespace Elevate
{
    void DirectionalLight::Init()
    {
        gameObject->GetScene()->GetSceneLighting()->AddDirectionalLight(this);
    }

    void DirectionalLight::Destroy()
    {
        gameObject->GetScene()->GetSceneLighting()->RemoveDirectionalLight(this);
    }

    const glm::vec3 DirectionalLight::CalculateDirection() const
	{
        return gameObject->GetTransform().GetForward();
	}

#ifdef EE_EDITOR_BUILD
    void DirectionalLight::RenderWhenSelected()
    {
        DrawDebugDirection();
        EditorRenderer::DrawBillboard(gameObject->GetPosition(), GetEditorIcon(), 0.7f);
    }

    void DirectionalLight::DrawDebugDirection()
    {
        const auto& transform = gameObject->GetTransform();

        glm::vec3 position = gameObject->GetGlobalPosition();
        glm::vec3 forward = glm::normalize(transform.GetForward());
        glm::vec3 right = glm::normalize(transform.GetRight());
        glm::vec3 up = glm::normalize(transform.GetUp());

        const float rayLength = 3.0f;
        const float circleRadius = 0.6f;
        const int rayCount = 12;

        const glm::vec4 rayColor = glm::vec4(1.0f, 0.85f, 0.1f, 1.0f);

        DebugRenderer::AddDebugLine({ position, position + forward * rayLength, rayColor });

        constexpr float TWO_PI = glm::two_pi<float>();

        for (int i = 0; i < rayCount; i++)
        {
            float angle = TWO_PI * static_cast<float>(i) / rayCount;
            glm::vec3 offset = (right * std::cos(angle) + up * std::sin(angle)) * circleRadius;

            glm::vec3 start = position + offset;
            glm::vec3 end = start + forward * rayLength;

            DebugRenderer::AddDebugLine({start, end, rayColor});
        }

        for (int i = 0; i < rayCount; i++)
        {
            float angle0 = TWO_PI * static_cast<float>(i) / rayCount;
            float angle1 = TWO_PI * static_cast<float>(i + 1) / rayCount;

            glm::vec3 p0 = position + (right * std::cos(angle0) + up * std::sin(angle0)) * circleRadius;
            glm::vec3 p1 = position + (right * std::cos(angle1) + up * std::sin(angle1)) * circleRadius;

            DebugRenderer::AddDebugLine({ p0, p1, rayColor });
        }
    }

#endif
}