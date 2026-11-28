#pragma once

// Keep this header free of standard library includes: it is pulled into the
// global module fragment of several module interface units, where duplicated
// libc++ headers confuse Clang's module ODR checks.
namespace Elevate::RendererBackend
{
	// Single registration point for the active backend. It is idempotent and
	// registers the RendererAPI factory as well as every resource factory.
	// Declarations live in the engine layer; the OpenGL implementation lives in
	// Platform/OpenGL. Typed objects are deliberately absent from this header:
	// it is included from global module fragments, where forward declarations
	// would conflict with the imported modules on GCC/Clang.
	void Register();

	bool CreateCubemapTexture(const char* const paths[6], unsigned int& textureID, const char* skyboxFilePath);
	void BindCubemapTexture(unsigned int textureID, unsigned char slot);
}
