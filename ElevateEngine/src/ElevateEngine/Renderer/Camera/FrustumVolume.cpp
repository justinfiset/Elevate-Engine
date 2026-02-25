#include "eepch.h"
#include "FrustumVolume.h"


// SPHERE VOLUME
bool Elevate::SphereBoundingVolume::IsOnFrustum(const Frustum& frustum, const Transform& transform) const
{
    //glm::vec3 center { 0.0f, 0.0f, 0.0f };// TODO SET SOMEHOW WITH TRANSFORM;
    //float radius = 0.0f; // TODO SET SOMEHOW WITH TRANSFORM;

    //const glm::vec3 globalScale = transform.GetGlobalScale();
    ////Get our global center with process it with the global model matrix of our transform
    //const glm::vec3 globalCenter{ transform.GetModelMatrix()* glm::vec4(center, 1.f) };

    ////To wrap correctly our shape, we need the maximum scale scalar.
    //const float maxScale = std::max(std::max(globalScale.x, globalScale.y), globalScale.z);

    ////Max scale is assuming for the diameter. So, we need the half to apply it to our radius
    //Sphere globalSphere(globalCenter, radius * (maxScale * 0.5f));

    ////Check Firstly the result that have the most chance
    ////to faillure to avoid to call all functions.
    //return (globalSphere.isOnOrForwardPlane(camFrustum.leftFace) &&
    //    globalSphere.isOnOrForwardPlane(camFrustum.rightFace) &&
    //    globalSphere.isOnOrForwardPlane(camFrustum.farFace) &&
    //    globalSphere.isOnOrForwardPlane(camFrustum.nearFace) &&
    //    globalSphere.isOnOrForwardPlane(camFrustum.topFace) &&
    //    globalSphere.isOnOrForwardPlane(camFrustum.bottomFace));
    return true;
}
