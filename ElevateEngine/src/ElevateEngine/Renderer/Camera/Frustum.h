#include "glm/glm.hpp"

namespace Elevate
{
    class Camera;

    struct Plane
    {
        glm::vec3 normal = { 0.f, 1.f, 0.f };
        float distance = 0.0f;         

        void Set(const glm::vec3& normal, const glm::vec3& point)
        {
            this->normal = glm::normalize(normal);
            this->distance = -glm::dot(this->normal, point);
        }

        float DistanceToPoint(const glm::vec3& point) const
        {
            return glm::dot(normal, point) + distance;
        }
    };

    enum FrustumPlane
    {
        NEAR_PLANE = 0,
        FAR_PLANE,
        TOP_PLANE,
        BOTTOM_PLANE,
        RIGHT_PLANE,
        LEFT_PLANE
    };

    struct Frustum
    {
        Frustum() = default;
        Frustum(Camera& camera);

        Plane planes[6];
    };
}

