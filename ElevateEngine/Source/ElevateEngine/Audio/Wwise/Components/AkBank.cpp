module;

// EnTT's iterator operators must be visible in the global module when inline
// Scene code is instantiated here; they otherwise stay attached to the Scene
// module BMI and are not found by ADL.
#include <entt/entt.hpp>

module Elevate.Audio.Wwise.AkBank;

namespace Elevate
{
	void AkBank::Init()
	{
	}
}
