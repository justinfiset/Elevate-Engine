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

glm::mat4 Elevate::Camera::GenViewProjectionMatrix()
{
    return m_projectionMatrix * GenViewMatrix();
}

glm::mat4 Elevate::Camera::GenViewMatrix()
{
    // TODO FAIRE QUE LE FRONT CHANGE EN FONCTION DE LA ROTATION;
    // TODO FAIRE QUE LE UP CHANGE EN FONCTION DE LA ROTATION
    return glm::lookAt(m_Transform.position, m_Transform.position + m_front, m_up);
}

glm::mat4 Elevate::Camera::GenProjectionMatrix()
{
    return glm::perspective(glm::radians(m_FOV), m_AspectRatio, 0.1f, 100.0f);
}

void Elevate::Camera::UpdateCameraVectors()
{
    glm::vec3 front;
    glm::vec3* rot = &m_Transform.rotation;
    front.x = cos(glm::radians(rot->y)) * cos(glm::radians(rot->x));
    front.y = sin(glm::radians(rot->x));
    front.z = sin(glm::radians(rot->y)) * cos(glm::radians(rot->x));
    m_front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    m_right = glm::normalize(glm::cross(m_front, {0,1,0}));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
