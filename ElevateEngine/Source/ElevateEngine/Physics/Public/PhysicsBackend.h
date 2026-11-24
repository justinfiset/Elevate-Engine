#pragma once

namespace Elevate
{
	// Implemented by the active physics backend. Registering here keeps the
	// backend selection out of the public module graph while still forcing the
	// backend translation unit to be linked when a physics system is created.
	void RegisterPhysicsSystemBackend();
}
