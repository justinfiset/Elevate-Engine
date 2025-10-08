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

void Elevate::Camera::Init()
{
    UpdateProjectionMatrix();
    UpdateCameraVectors();
    // todo : change this bit of code to be in the camera manager
    SoundEngine::SetDefaultListener(gameObject);
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

//Elevate::Component* Elevate::Camera::Clone()
//{
//    Camera* cam = new Camera(m_FOV, m_AspectRatio);
//
//    cam->m_Front = m_Front;
//    cam->m_Right = m_Right;
//    cam->m_Up = m_Up;
//    cam->m_ProjectionMatrix = m_ProjectionMatrix;
//    cam->m_Near = m_Near;
//    cam->m_Far = m_Far;
//
//    return cam;
//}

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
