#include "eepch.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

glm::mat4 Elevate::Camera::GetViewProjectionMatrix()
{
    return GetViewMatrix() * GetProjectionMatrix();
}

glm::mat4 Elevate::Camera::GetViewMatrix()
{
    // TODO FAIRE UN LOOKAT
    return glm::mat4(1.0F);
}

glm::mat4 Elevate::Camera::GetProjectionMatrix()
{
    return glm::perspective(glm::radians(m_FOV), (float)m_Width / (float)m_Weight, 0.1f, 100.0f);
}
