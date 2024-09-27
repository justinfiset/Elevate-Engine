#pragma once

#include "Frustum.h"
#include "ElevateEngine/Core/Transform.h"

namespace Elevate
{
	class FrustumVolume
	{
	public:
		virtual bool IsOnFrustum(const Frustum& frustum, const Transform& transform) const = 0;
	};

	class SphereFrustumVolume : public FrustumVolume
	{
	public:
		bool IsOnFrustum(const Frustum& frustum, const Transform& transform) const override;
	};
}