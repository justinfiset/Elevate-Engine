#pragma once

#include "Collider.h"

namespace Elevate
{
    class MeshCollider : public Collider
    {
        BEGIN_COMPONENT(MeshCollider)
        DECLARE_BASE(Collider)
        EECATEGORY("Physics")

    public:
        MeshCollider() = default;

    private:

        END_COMPONENT()
    };
}