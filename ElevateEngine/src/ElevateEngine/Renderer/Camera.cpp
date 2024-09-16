#include "eepch.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ElevateEngine/Core/Application.h"

Elevate::Camera::Camera(float fov) : 
    Elevate::Camera(Transform(), fov) { }

Elevate::Camera::Camera(float fov, float aspectRatio) :
    Elevate::Camera(Transform(), fov, aspectRatio) { }

// TODO simplifier en un seul constructeur unique
Elevate::Camera::Camera(Transform transform, float fov)
{
    m_Transform = transform;
    m_FOV = fov;
    Window& window = Application::Get().GetWindow();
    m_AspectRatio = (float)window.GetWidth() / (float)window.GetHeight();

    m_Transform.rotation.y = -90.0f; // TODO ENLEVER AU PC
    m_projectionMatrix = GenProjectionMatrix();
    UpdateCameraVectors();
}

Elevate::Camera::Camera(Transform transform, float fov, float aspectRatio)
{
    m_Transform = transform;
    m_FOV = fov;
    Window& window = Application::Get().GetWindow();
    m_AspectRatio = aspectRatio;

    m_Transform.rotation.y = -90.0f; // TODO ENLEVER AU PC
    m_projectionMatrix = GenProjectionMatrix();
    UpdateCameraVectors();
}

const void Elevate::Camera::UpdateAspectRatio(float aspectRatio)
{
    m_AspectRatio = aspectRatio;
    m_projectionMatrix = GenProjectionMatrix();
}

glm::mat4 Elevate::Camera::GenViewProjectionMatrix()
{
    return m_projectionMatrix * GenViewMatrix();
}

glm::mat4 Elevate::Camera::GenViewMatrix()
{
    return glm::lookAt(m_Transform.position, m_Transform.position + m_front, m_up);
}

glm::mat4 Elevate::Camera::GenProjectionMatrix()
{
    return glm::perspective(glm::radians(m_FOV), m_AspectRatio, 0.1f, 100.0f);
}

void Elevate::Camera::UpdateCameraVectors()
{
    float pitch = glm::radians(m_Transform.rotation.x);
    float yaw = glm::radians(m_Transform.rotation.y);

    glm::vec3 front;
    front.x = cos(yaw) * cos(pitch);
    front.y = sin(pitch);
    front.z = sin(yaw) * cos(pitch);
    m_front = glm::normalize(front);

    m_right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_front));

    m_up = glm::normalize(glm::cross(m_front, m_right));
}
