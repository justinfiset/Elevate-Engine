#pragma once

namespace Elevate
{
	enum class CullFace
	{
		None,
		Front,
		Back,
		FrontAndBack
	};

	struct RenderState
	{
		CullFace CullMode = CullFace::Back;
		bool DepthWrite = true; // Should we write in the depth buffer?
		bool DepthTest = true; // Should we test the depth?
		bool BlendEnable = false;
	};
}