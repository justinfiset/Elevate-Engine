module;

#include <cstdint>

export module Elevate.Renderer.Commands.RenderBucketType;

export namespace Elevate
{
	enum class RenderBucketType : uint8_t
	{
		ShadowMap,
		GBuffer,
		Transparent,
		UI,
		Editor,
		Debug,
		Count // Only used to keep the number of bucket at compile time
	};
}
