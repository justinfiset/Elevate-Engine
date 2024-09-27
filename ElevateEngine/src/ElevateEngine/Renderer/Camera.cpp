#include "eepch.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ElevateEngine/Core/Application.h"
#include "ElevateEngine/Core/GameObject.h"

// TODO ADD PROTECTION WITH SETTER TO MAKE SURE IF OLD = NEW !GENERATENEW MATRIX FOR PROJECTION
// (GOAL IS TO PREVENT USELESS MATRIX CALCULATION)
Elevate::Camera::Camera(float fov)
{
    m_FOV = fov;
    Window& window = Application::Get().GetWindow();
    m_AspectRatio = (float)window.GetWidth() / (float)window.GetHeight();
}

Elevate::Camera::Camera(float fov, float aspectRatio)
{
    m_FOV = fov;
    Window& window = Application::Get().GetWindow();
    m_AspectRatio = aspectRatio;
}


void Elevate::Camera::Init()
{
    gameObject->GetRotation().y = -90.0f; // TODO ENLEVER AU PC
    UpdateProjectionMatrix();
    UpdateCameraVectors();
}


const void Elevate::Camera::UpdateAspectRatio(float aspectRatio)
{
    m_AspectRatio = aspectRatio;
    UpdateProjectionMatrix();
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
    m_FOV = fov;
    UpdateProjectionMatrix();
}

inline void Elevate::Camera::SetNear(float nearPlane)
{
    m_Near = nearPlane;
    UpdateProjectionMatrix();
}

inline void Elevate::Camera::SetFar(float farPlane)
{
    m_Far = farPlane;
    UpdateProjectionMatrix();
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
