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

	enum class DepthFunction
	{
		Less,
		LessEqual,
		Equal,
		Always,
		Never,
		Greater,
		GreaterEqual
	};

	struct RenderState
	{
		CullFace CullMode = CullFace::Back;
		bool DepthWrite = true; // Should we write in the depth buffer?
		bool DepthTest = true; // Should we test the depth?
		DepthFunction DepthFunc = DepthFunction::Less;
		BlendModeType BlendMode = BlendModeType::None;
	};
}