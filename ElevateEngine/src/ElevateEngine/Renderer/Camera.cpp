#include "eepch.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ElevateEngine/Core/Application.h"
#include "ElevateEngine/Core/GameObject.h"

#include "ElevateEngine/Renderer/Camera/CameraManager.h"
// todo : remove when not necessary anymore
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <ElevateEngine/Audio/SoundEngine.h>

Elevate::Camera::Camera(float fov, bool overrideCurrent)
{
    m_FOV = fov;
    Window& window = Application::Get().GetWindow();
    m_AspectRatio = (float)window.GetWidth() / (float)window.GetHeight();

    if (overrideCurrent)
    {
        CameraManager::SetCurrent(this);
    }
}

Elevate::Camera::Camera(float fov, float aspectRatio, bool overrideCurrent)
{
    m_FOV = fov;
    Window& window = Application::Get().GetWindow();
    m_AspectRatio = aspectRatio;

    if (overrideCurrent)
    {
        CameraManager::SetCurrent(this);
    }
}

// TODO REMOVE THIS IS ONLY A TEST
#include "ElevateEngine/Renderer/Debug/DebugRenderer.h"

void Elevate::Camera::Init()
{
    UpdateProjectionMatrix();
    UpdateCameraVectors();
    // todo : change this bit of code to be in the camera manager
    SoundEngine::SetDefaultListener(gameObject);
}

void Elevate::Camera::RenderWhenSelected()
{
    DrawDebugFrustum();
}

const void Elevate::Camera::UpdateAspectRatio(float aspectRatio)
{
    if (m_AspectRatio != aspectRatio)
    {
        m_AspectRatio = aspectRatio;
        UpdateProjectionMatrix();
    }
}

glm::mat4 Elevate::Camera::GenViewProjectionMatrix()
{
    return m_ProjectionMatrix * GenViewMatrix();
}

glm::mat4 Elevate::Camera::GenViewMatrix()
{
    return glm::lookAt(gameObject->GetPosition(), gameObject->GetPosition() + m_Front, m_Up);
}

inline void Elevate::Camera::SetFOV(float fov)
{
    if (m_FOV != fov)
    {
        m_FOV = fov;
        UpdateProjectionMatrix();
    }
}

inline void Elevate::Camera::SetNear(float nearPlane)
{
    if (m_Near != nearPlane)
    {
        m_Near = nearPlane;
        UpdateProjectionMatrix();
    }
}

inline void Elevate::Camera::SetFar(float farPlane)
{
    if (m_Far != farPlane)
    {
        m_Far = farPlane;
        UpdateProjectionMatrix();
    }
}

glm::mat4 Elevate::Camera::GenProjectionMatrix()
{
    return glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);
}

void Elevate::Camera::UpdateProjectionMatrix()
{
    m_ProjectionMatrix = GenProjectionMatrix();
}

void Elevate::Camera::UpdateCameraVectors()
{
    float pitch = glm::radians(gameObject->GetRotation().x);
    float yaw = glm::radians(gameObject->GetRotation().y);

    glm::vec3 front;
    front.x = cos(yaw) * cos(pitch);
    front.y = sin(pitch);
    front.z = sin(yaw) * cos(pitch);
    m_Front = glm::normalize(front);

    m_Right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_Front));

    m_Up = glm::normalize(glm::cross(m_Front, m_Right));
}

// ONLY IN THE EDITOR
#ifdef EE_ENGINE_BUILD
void Elevate::Camera::DrawDebugFrustum()
{
    auto corners = CalculateFrustumCorners();

    glm::vec4 color = glm::vec4(0.96f, 0.47f, 0.12f, 1.0f);

    glm::vec3 origin = gameObject->GetGlobalPosition();
    for (int i = 0; i < 8; i++) {
        DebugRenderer::AddDebugLine({ origin, corners[i], glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) }); // Magenta debug lines
    }

    // Then draw the frustum (your existing code)
    // Draw near plane
    DebugRenderer::AddDebugLine({ corners[0], corners[1], color });
    DebugRenderer::AddDebugLine({ corners[1], corners[2], color });
    DebugRenderer::AddDebugLine({ corners[2], corners[3], color });
    DebugRenderer::AddDebugLine({ corners[3], corners[0], color });

    // Draw far plane
    DebugRenderer::AddDebugLine({ corners[4], corners[5], color });
    DebugRenderer::AddDebugLine({ corners[5], corners[6], color });
    DebugRenderer::AddDebugLine({ corners[6], corners[7], color });
    DebugRenderer::AddDebugLine({ corners[7], corners[4], color });

    // Draw connecting lines
    DebugRenderer::AddDebugLine({ corners[0], corners[4], color });
    DebugRenderer::AddDebugLine({ corners[1], corners[5], color });
    DebugRenderer::AddDebugLine({ corners[2], corners[6], color });
    DebugRenderer::AddDebugLine({ corners[3], corners[7], color });
}

std::array<glm::vec3, 8> Elevate::Camera::CalculateFrustumCorners(float visualFarScale)
{
    std::array<glm::vec3, 8> corners;

    glm::vec3 position = gameObject->GetGlobalPosition();
    glm::vec3 front = GetFrontVec();
    glm::vec3 right = GetRightVec();
    glm::vec3 up = GetUpVec();

    float tanHalfFOV = tan(glm::radians(m_FOV * 0.5f));

    float visualFar = m_Far * visualFarScale;
    float nearHeight = 2.0f * tanHalfFOV * m_Near;
    float nearWidth = nearHeight * m_AspectRatio;

    float farHeight = 2.0f * tanHalfFOV * visualFar;
    float farWidth = farHeight * m_AspectRatio;

    glm::vec3 nearCenter = position + front * m_Near;
    glm::vec3 farCenter = position + front * visualFar;

    // Near Plane Corners
    corners[0] = nearCenter - (up * (nearHeight * 0.5f)) - (right * (nearWidth * 0.5f)); // near-bottom-left
    corners[1] = nearCenter - (up * (nearHeight * 0.5f)) + (right * (nearWidth * 0.5f)); // near-bottom-right
    corners[2] = nearCenter + (up * (nearHeight * 0.5f)) + (right * (nearWidth * 0.5f)); // near-top-right
    corners[3] = nearCenter + (up * (nearHeight * 0.5f)) - (right * (nearWidth * 0.5f)); // near-top-left

    // Far Plane Corners
    corners[4] = farCenter - (up * (farHeight * 0.5f)) - (right * (farWidth * 0.5f)); // far-bottom-left
    corners[5] = farCenter - (up * (farHeight * 0.5f)) + (right * (farWidth * 0.5f)); // far-bottom-right
    corners[6] = farCenter + (up * (farHeight * 0.5f)) + (right * (farWidth * 0.5f)); // far-top-right
    corners[7] = farCenter + (up * (farHeight * 0.5f)) - (right * (farWidth * 0.5f)); // far-top-left

    return corners;
}
#endif