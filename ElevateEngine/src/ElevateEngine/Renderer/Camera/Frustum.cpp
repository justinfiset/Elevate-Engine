#include "eepch.h"
#include "Frustum.h"

#include "ElevateEngine/Renderer/Camera.h"
#include "ElevateEngine/Core/GameObject.h"

namespace Elevate
{
    Frustum::Frustum(Camera& cam)
    {
        float zFar = cam.GetFar();
        float zNear = cam.GetNear();
        float aspectRatio = cam.GetAspectRatio();
        float fov = cam.GetFOV();

        glm::vec3 front = cam.GetFrontVec();
        glm::vec3 right = cam.GetRightVec();
        glm::vec3 up = cam.GetUpVec();
        glm::vec3 position = cam.gameObject->GetPosition();

        float tanFovHalf = tan(fov / 2.0f);

        float nearHeight = 2.0f * tanFovHalf * zNear;
        float nearWidth = nearHeight * aspectRatio;
        float farHeight = 2.0f * tanFovHalf * zFar;
        float farWidth = farHeight * aspectRatio;

        glm::vec3 nearCenter = position + front * zNear;
        glm::vec3 farCenter = position + front * zFar;

        const glm::vec3 nearTopLeft = nearCenter + up * (nearHeight * 0.5f) - right * (nearWidth * 0.5f);
        const glm::vec3 nearTopRight = nearCenter + up * (nearHeight * 0.5f) + right * (nearWidth * 0.5f);
        const glm::vec3 nearBottomLeft = nearCenter - up * (nearHeight * 0.5f) - right * (nearWidth * 0.5f);
        const glm::vec3 nearBottomRight = nearCenter - up * (nearHeight * 0.5f) + right * (nearWidth * 0.5f);

        const glm::vec3 farTopLeft = farCenter + up * (farHeight * 0.5f) - right * (farWidth * 0.5f);
        const glm::vec3 farTopRight = farCenter + up * (farHeight * 0.5f) + right * (farWidth * 0.5f);
        const glm::vec3 farBottomLeft = farCenter - up * (farHeight * 0.5f) - right * (farWidth * 0.5f);
        const glm::vec3 farBottomRight = farCenter - up * (farHeight * 0.5f) + right * (farWidth * 0.5f);

        planes[FrustumPlane::NEAR_PLANE].Set(front, nearCenter);
        planes[FrustumPlane::FAR_PLANE].Set(-front, farCenter);
        planes[FrustumPlane::TOP_PLANE].Set(glm::cross(nearTopRight - position, farTopLeft - position), nearTopLeft);
        planes[FrustumPlane::BOTTOM_PLANE].Set(glm::cross(farBottomLeft - position, nearBottomRight - position), nearBottomLeft);
        planes[FrustumPlane::LEFT_PLANE].Set(glm::cross(nearBottomLeft - position, farTopLeft - position), nearBottomLeft);
        planes[FrustumPlane::RIGHT_PLANE].Set(glm::cross(farTopRight - position, nearBottomRight - position), nearBottomRight);
    }
}