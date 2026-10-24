#pragma once

#include "Frustum.h"

import Elevate.Core.Transform.Transform;

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