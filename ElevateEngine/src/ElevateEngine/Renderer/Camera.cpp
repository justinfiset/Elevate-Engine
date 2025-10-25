#include "eepch.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ElevateEngine/Core/Application.h"
#include "ElevateEngine/Core/GameObject.h"

#include "ElevateEngine/Renderer/Camera/CameraManager.h"
#include <ElevateEngine/Audio/SoundEngine.h>

Elevate::Camera::Camera(float fov, bool overrideCurrent)
{
    m_FOV = fov;
    Window& window = Application::Get().GetWindow();
    m_aspectRatio = (float)window.GetWidth() / (float)window.GetHeight();

    m_canBeMainCamera = overrideCurrent;
}

Elevate::Camera::Camera(float fov, float aspectRatio, bool overrideCurrent)
{
    m_FOV = fov;
    Window& window = Application::Get().GetWindow();
    m_aspectRatio = aspectRatio;

    m_canBeMainCamera = overrideCurrent;
}

void Elevate::Camera::Init()
{
    UpdateProjectionMatrix();
    UpdateCameraVectors();

    if (m_canBeMainCamera)
    {
        CameraManager::SetCurrent(this);
    }

    // todo : change this bit of code to be in the camera manager
    SoundEngine::SetDefaultListener(gameObject);
}

void Elevate::Camera::Destroy()
{
    CameraManager::NotifyDestruction(this);
}

const void Elevate::Camera::UpdateAspectRatio(float aspectRatio)
{
    if (m_aspectRatio != aspectRatio)
    {
        m_aspectRatio = aspectRatio;
        UpdateProjectionMatrix();
    }
}

glm::mat4 Elevate::Camera::GenViewProjectionMatrix()
{
    return m_projectionMatrix * GenViewMatrix();
}

glm::mat4 Elevate::Camera::GenViewMatrix()
{
    return glm::lookAt(gameObject->GetPosition(), gameObject->GetPosition() + m_front, m_up);
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
    if (m_near != nearPlane)
    {
        m_near = nearPlane;
        UpdateProjectionMatrix();
    }
}

inline void Elevate::Camera::SetFar(float farPlane)
{
    if (m_far != farPlane)
    {
        m_far = farPlane;
        UpdateProjectionMatrix();
    }
}

glm::mat4 Elevate::Camera::GenProjectionMatrix()
{
    return glm::perspective(glm::radians(m_FOV), m_aspectRatio, m_near, m_far);
}

void Elevate::Camera::UpdateProjectionMatrix()
{
    m_projectionMatrix = GenProjectionMatrix();
}

void Elevate::Camera::UpdateCameraVectors()
{
    float pitch = glm::radians(gameObject->GetRotation().x);
    float yaw = glm::radians(gameObject->GetRotation().y);

    glm::vec3 front;
    front.x = cos(yaw) * cos(pitch);
    front.y = sin(pitch);
    front.z = sin(yaw) * cos(pitch);
    m_front = glm::normalize(front);

    m_right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_front));

    m_up = glm::normalize(glm::cross(m_front, m_right));
}

// ONLY IN THE EDITOR
#ifdef EE_EDITOR_BUILD
#include <ElevateEngine/Renderer/Debug/DebugRenderer.h>

void Elevate::Camera::RenderWhenSelected()
{
    UpdateCameraVectors();
    DrawDebugFrustum();
}

void Elevate::Camera::DrawDebugFrustum()
{
    auto corners = CalculateFrustumCorners();

    glm::vec4 color = glm::vec4(0.96f, 0.47f, 0.12f, 1.0f);

    // Draw the lines from the near corners to the far corners
    for (int i = 0; i < 4; i++) 
    {
        DebugRenderer::AddDebugLine({ corners[i], corners[i + 4], color });
    }

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
}

std::array<glm::vec3, 8> Elevate::Camera::CalculateFrustumCorners(float visualFarScale)
{
    std::array<glm::vec3, 8> corners;

    glm::vec3 position = gameObject->GetGlobalPosition();
    glm::vec3 front = GetFrontVec();
    glm::vec3 right = GetRightVec();
    glm::vec3 up = GetUpVec();

    float tanHalfFOV = tan(glm::radians(m_FOV * 0.5f));

    float visualFar = m_far * visualFarScale;
    float nearHeight = 2.0f * tanHalfFOV * m_near;
    float nearWidth = nearHeight * m_aspectRatio;

    float farHeight = 2.0f * tanHalfFOV * visualFar;
    float farWidth = farHeight * m_aspectRatio;

    glm::vec3 nearCenter = position + front * m_near;
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