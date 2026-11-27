module;

#include "Reflection.h"

export module Elevate.Physics.MeshCollider;

import Elevate.Core.Scene;
export import Elevate.Physics.Collider;

export namespace Elevate
{
	class MeshCollider : public Collider
	{
		BEGIN_COMPONENT(MeshCollider);
		DECLARE_BASE(Collider);
		EECATEGORY("Physics");

	public:
		MeshCollider() = default;

	private:

		END_COMPONENT();
	};
}
