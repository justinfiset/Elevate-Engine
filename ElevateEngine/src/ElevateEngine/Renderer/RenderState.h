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

	enum class BlendModeType
	{
		Alpha,
		Additive,
		None
	};

	struct RenderState
	{
		CullFace CullMode = CullFace::Back;
		bool DepthWrite = true; // Should we write in the depth buffer?
		bool DepthTest = true; // Should we test the depth?
		BlendModeType BlendMode = BlendModeType::None;
	};
}