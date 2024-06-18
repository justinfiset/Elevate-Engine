#include "eepch.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ElevateEngine/Core/Application.h"

Elevate::Camera::Camera(float fov) : 
    Elevate::Camera(Transform(), fov) { }

Elevate::Camera::Camera(float fov, float aspectRatio) :
    Elevate::Camera(Transform(), fov, aspectRatio) { }

Elevate::Camera::Camera(Transform transform, float fov)
{
    m_Transform = transform;
    m_FOV = fov;
    Window& window = Application::Get().GetWindow();
    m_AspectRatio = (float)window.GetWidth() / (float)window.GetHeight();

    m_Transform.rotation.y = -90.0f; // TODO ENLEVER AU PC
    m_ProjectionMatrix = GenProjectionMatrix();
}

Elevate::Camera::Camera(Transform transform, float fov, float aspectRatio)
{
    m_Transform = transform;
    m_FOV = fov;
    Window& window = Application::Get().GetWindow();
    m_AspectRatio = aspectRatio;

    m_Transform.rotation.y = -90.0f; // TODO ENLEVER AU PC
    m_ProjectionMatrix = GenProjectionMatrix();
}

glm::mat4 Elevate::Camera::GenViewProjectionMatrix()
{
    return m_ProjectionMatrix * GenViewMatrix();
}

glm::mat4 Elevate::Camera::GenViewMatrix()
{
    // TODO FAIRE QUE LE FRONT CHANGE EN FONCTION DE LA ROTATION;
    // TODO FAIRE QUE LE UP CHANGE EN FONCTION DE LA ROTATION
    glm::vec3 cameraFront = GetFront();
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    return glm::lookAt(m_Transform.position, m_Transform.position + cameraFront, cameraUp);
}

glm::mat4 Elevate::Camera::GenProjectionMatrix()
{
    return glm::perspective(glm::radians(m_FOV), m_AspectRatio, 0.1f, 100.0f);
}

// TODO integrate in the Tranform script???
glm::vec3 Elevate::Camera::GetFront()
{
    glm::vec3 direction;
    glm::vec3* rot = &m_Transform.rotation;
    direction.x = cos(glm::radians(rot->y)) * cos(glm::radians(rot->x));
    direction.y = sin(glm::radians(rot->x));
    direction.z = sin(glm::radians(rot->y)) * cos(glm::radians(rot->x));
    return glm::normalize(direction);
}
