#pragma once

#include <cstdint>

namespace Elevate::Internal
{
	// The address is passed as an integer because GameObject is declared in a
	// named module and cannot be named from this non-module header.  The bridge
	// translation unit imports both modules and restores the typed pointer.
	void RegisterSoundObject(std::uintptr_t objectAddress);
	void UnregisterSoundObject(std::uintptr_t objectAddress);
	void UpdateSoundObjectPosition(std::uintptr_t objectAddress);
}
