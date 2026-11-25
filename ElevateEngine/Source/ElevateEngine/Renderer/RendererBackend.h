#pragma once

// Keep this header free of standard library includes: it is pulled into the
// global module fragment of several module interface units, where duplicated
// libc++ headers confuse Clang's module ODR checks.
namespace Elevate::RendererBackend
{
	// Generic backend hooks. Declarations live in the engine layer; the OpenGL
	// implementations live in Platform/OpenGL. RendererAPI is registered through
	// Renderer::SetAPIFactory instead of being forward declared here, because a
	// global-module forward declaration conflicts with the imported module on
	// GCC/Clang.
	void RegisterRendererAPIFactory();

	void RegisterBufferFactories();
	void RegisterVertexArrayFactory();
	void RegisterFramebufferFactory();
	void RegisterShaderFactories();
	void RegisterTextureFactory();

	bool CreateCubemapTexture(const char* const paths[6], unsigned int& textureID, const char* skyboxFilePath);
	void BindCubemapTexture(unsigned int textureID, unsigned char slot);
}
