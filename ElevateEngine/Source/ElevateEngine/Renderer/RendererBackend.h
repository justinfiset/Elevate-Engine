#pragma once

namespace Elevate::RendererBackend
{
	// Implemented by the active renderer backend. These hooks let module
	// interfaces create backend objects without owning backend headers.
	void RegisterBufferFactories();
	void RegisterVertexArrayFactory();
	void RegisterFramebufferFactory();
	void RegisterShaderFactories();
	void RegisterTextureFactory();
}
