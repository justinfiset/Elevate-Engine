#pragma once

#include "Frustum.h"
#include "ElevateEngine/Core/Transform.h"

namespace Elevate
{
	class BoundingVolume
	{
	public:
		virtual bool IsOnFrustum(const Frustum& frustum, const Transform& transform) const = 0;
	};

	class SphereBoundingVolume : public BoundingVolume
	{
	public:
		virtual bool IsOnFrustum(const Frustum& frustum, const Transform& transform) const override;
	};
}