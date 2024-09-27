#include "eepch.h"
#include "Frustum.h"

#include "ElevateEngine/Renderer/Camera.h"
#include "ElevateEngine/Core/GameObject.h"

namespace Elevate
{
    // TODO OPTIMISER LE CODE ET RÉDUIRE L'USAGE DE LA MÉMOIRE
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

        glm::vec3 halfNearHeight = up * (nearHeight / 2.0f);
        glm::vec3 halfNearWidth = right * (nearWidth / 2.0f);
        glm::vec3 halfFarHeight = up * (farHeight / 2.0f);
        glm::vec3 halfFarWidth = right * (farWidth / 2.0f);

        glm::vec3 nearTopLeft = nearCenter + halfNearHeight - halfNearWidth;
        glm::vec3 nearTopRight = nearCenter + halfNearHeight + halfNearWidth;
        glm::vec3 nearBottomLeft = nearCenter - halfNearHeight - halfNearWidth;
        glm::vec3 nearBottomRight = nearCenter - halfNearHeight + halfNearWidth;

        glm::vec3 farTopLeft = farCenter + halfFarHeight - halfFarWidth;
        glm::vec3 farTopRight = farCenter + halfFarHeight + halfFarWidth;
        glm::vec3 farBottomLeft = farCenter - halfFarHeight - halfFarWidth;
        glm::vec3 farBottomRight = farCenter - halfFarHeight + halfFarWidth;

        glm::vec3 topNormal = glm::cross(nearTopRight - position, farTopLeft - position);
        glm::vec3 bottomNormal = glm::cross(farBottomLeft - position, nearBottomRight - position);
        glm::vec3 leftNormal = glm::cross(nearBottomLeft - position, farTopLeft - position);
        glm::vec3 rightNormal = glm::cross(farTopRight - position, nearBottomRight - position);

        planes[FrustumPlane::NEAR_PLANE].Set(front, nearCenter);
        planes[FrustumPlane::FAR_PLANE].Set(-front, farCenter);
        planes[FrustumPlane::TOP_PLANE].Set(topNormal, nearTopLeft);
        planes[FrustumPlane::BOTTOM_PLANE].Set(bottomNormal, nearBottomLeft);
        planes[FrustumPlane::LEFT_PLANE].Set(leftNormal, nearBottomLeft);
        planes[FrustumPlane::RIGHT_PLANE].Set(rightNormal, nearBottomRight);
    }
}